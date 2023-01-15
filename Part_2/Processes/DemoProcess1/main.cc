#include<iostream>  
#include<Windows.h>  

using namespace std;

int main(int argc, char* argv[])
{
    cout << "args_num: " << argc << endl;
    for (int i = 0; i < argc; i++) {
        cout << "arg " << i << " = " << argv[i] << endl;
    }

    return argc % 2;
}