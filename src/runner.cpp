//aim: make a utility that will automatically test solution against all testcases.
#include <bits/stdc++.h>
#include <cstdlib>
using namespace std;

int no_tests(string filename){
    filename.append("in");
    std::ifstream inp;
    inp.open(filename);
    if(inp.is_open()){
        //yayyyyyyyyyyyyyyyyyy
    }
    else {
        cout << "Failed to open file, so returning \n";
    }

    string nt;
    std::getline(inp,nt);

    inp.close();
    return stoi(nt);
}

int main(){
    //Lets begin by trying to call our solution correctly and outputting its result against the various tests.
    string filename;
    cin >> filename;

    int nt = no_tests(filename);
    cout << nt << endl;

    for(int i = 1; i <= nt; ++i){
        //call solution repeatedly.
        string solution = filename;
        //shouldn't need to append .exe for windows atleast. 
        // TODO In Linux, need to prepend ./

        string testname = filename;
        string num = std::to_string(i);
        testname.append(num);

        string ansfile = testname;
        ansfile.append("ans");

        string command = solution;
        command.append("<"); command.append(testname);
        command.append(">"); command.append(ansfile);

        const char * cmdd = command.c_str();

        cout << "command: " << command << endl;
        int calling = std::system(cmdd);
    }

    return 0;
}
