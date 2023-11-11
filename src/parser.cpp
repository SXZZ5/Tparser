#include "json.hpp"
#include <bits/stdc++.h>
using namespace std;
using json = nlohmann::json;

struct info {
	string outputs;
	string inputs;
	int test_count;
};
vector<string> convert(string);
void string_tostrings(info&,string);
void json_tostrings(info& test,string filename){
	filename.append(".cpp__tests");
	cout << filename << endl;
	std::ifstream ff(filename);
	if(ff.is_open()){
			cout << "File opened sucess" << endl;
	}
	else {
			cout << "file open failed" << endl;
	}
	json data = json::array();
	data = json::parse(ff);
	json inside = data[0];
	json temp = json::array();
	temp = inside["correct_answers"];
	string tests= inside["test"];
	string correct = temp[0];
	int testcnt = 0;
	int pos = 0;
	for(int i = 0; i<=(int)tests.size(); ++i) if(tests[i] == '\n'){
			pos = i;
			break;
	}

	string tchelp = tests.substr(0,pos);
	testcnt = std::stoi(tchelp);
	test.outputs = correct;
	test.inputs = tests.substr(pos+1,string::npos);
	test.test_count = testcnt;
	test.outputs.push_back('\0');
	test.inputs.push_back('\0');

	ff.close();
}

vector<string> convert(string toconv){
	//flag	0, if this is an answers string
	//flag	1, if this is a sampletests string

	vector<string> lines;
	int ptr = 1;
	// if(flag == 0) ptr = 1;
	// else ptr = 3;
	
	string line = "";
	while(ptr <= (int)toconv.size()){
			line.push_back(toconv[ptr-1]);
			if(toconv[ptr-1] == '\n'){
					lines.push_back(line);
					line.clear();
			}
			ptr++;
	}
	if(toconv[ptr-1] != '\n'){
			if(!line.empty()){
				lines.push_back(line);
			}
	}
	return lines;
}

void string_tostrings(info& test, string filename){
		string inputfilename = filename;
		string outputfilename = filename;
		inputfilename.append("in");
		outputfilename.append("out");
		// cout << inputfilename << endl;
		// cout << outputfilename << endl;
		std::ifstream inp;
		std::ifstream outp;
		inp.open(inputfilename,std::fstream::binary);
		outp.open(outputfilename,std::fstream::binary);
		if(inp.is_open()){
				cout << "File inp read" << endl;
		}
		if(outp.is_open()){
				cout << "File outp read" << endl;
		}

		inp.seekg(0,inp.end);
		int len = inp.tellg(); 
		inp.seekg(0,inp.beg);
		char* tests = new char [len+1];
		inp.read(tests,len);
		char ch = '\0';
		strcat(tests,&ch);
		string tmptests(tests);
		delete[] tests;
		inp.close();

		outp.seekg(0,outp.end);
		len = outp.tellg();
		outp.seekg(0,outp.beg);
		char* answers = new char [len+1];
		outp.read(answers,len);
		strcat(answers,&ch);
		string tmpanswers(answers);
		delete[] answers;
		outp.close();


		tmptests.pop_back();
		tmpanswers.pop_back();
		// cout << tmptests << endl;
		// cout << tmpanswers << endl;
		int pos = 0;
		for(int i = 0; i <= (int)tmptests.size(); ++i) if(tmptests[i] == '\n'){
				pos = i;
				break;
		}
		string tchelp = tmptests.substr(0,pos);
		test.test_count = std::stoi(tchelp);
		test.inputs = tmptests.substr(pos+1, string::npos);
		test.outputs = tmpanswers;
		return;

}

void string_to_file(string& to_write,string testnum,int flag){
		cout << "string to write \n"; 
		cout << to_write << "\n";
		if(flag == 0) testnum.append("o");
		if(flag == 2) testnum.append("inj");
		std::ofstream outp;
		outp.open(testnum,std::ofstream::out);
		outp << to_write;
		outp.close();
}

int main(){
	info testgroup;
	int nlt = 0;
	string filename;
	char mode;
	cin >> mode;
	cin >> filename;
	cin >> nlt;

	if(mode == 'j'){
		json_tostrings(testgroup,filename);
	}
	else if(mode == 't'){
		string_tostrings(testgroup,filename);
	}
	else{
		cout << "bad input";
		return 0;
	}

	cout << "testgroup structure " << "\n";
	cout << ".inputs " << testgroup.inputs << "\n";
	cout << ".outputs " << testgroup.outputs << "\n";
	cout << ".test_count" << testgroup.test_count << endl;

	vector<string> testlines = convert(testgroup.inputs);
	vector<string> answers = convert(testgroup.outputs);
	//this is literally a vector of basic_json objects.
	json newfile = json::array();
	int ansptr = 1, testptr = 1;
	for(int i = 1; i<=testgroup.test_count; ++i){
		string num = std::to_string(i);
		string test = "1\n";
		for(int j = testptr; j < testptr+nlt; ++j){
			test.append(testlines[j-1]);  
		}

		string temp = answers[ansptr-1];
		//prepare json object
		json sample = json::object();
		temp.pop_back();//removing the last newline charcter
		sample["correct_answers"] = temp;
		sample["test"] = test;
		
		//prepare iofiles
		string testnum = filename; testnum.append(num);
		string_to_file(test, testnum, 1);
		string_to_file(temp, testnum, 0);
		// cout << "testnum : " << testnum << endl;

		//proceed
		testptr += nlt;
		ansptr++;
		newfile.push_back(sample);
	}

	string json_tests = newfile.dump();
	string_to_file(json_tests,filename,2);
	return 0;

}


/*
1) move/copy/load the entire json file into a fstream.
2) parse the string contained in fstream = entire json file into 
a single json::array.
3) each json::array contains two elements. Pehla element is the correct_answers, dusra is "test"
4) casted both these objects into two strings (one for both).
5) processed these strings to break into different lines by actually line-breaking at the newline characters only.

RESOURCES
- https://stackoverflow.com/questions/46240194/c-reading-entire-file-come-with-garbage-at-the-end#:~:text=The%20%22garbage%22%20comes%20from%20improperly,to%20point%20to%20a%20string. About the garbage values at the end of the strings.


1) fixed that the strings need to be terminated in nullcharacter.
2) fixed the way of populating test_count information. Although this program can still process only those samples such that
the number of lines per (sub)-test case is fixed and known to the program user.

TODO
- use this extension for yourself for some days.
- think of a way to make an interfacing program that calls the "solution.cpp" file again and again for the various sub-tests and also tells the diff-result.
- diff tools to look into: Kdiff3,Examdiff - visual diff tools these are.
- And then there are terminal based difftools that you can try out too.
*/
