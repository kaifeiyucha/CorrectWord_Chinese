#include "index_chinese.h"
#include <iostream>
#include <string>
#include <set>
#include <iterator>


using namespace std;

int main(int argc, const char *argv[])
{
//    Index I("lib_test.txt");  中文测试
    Index I("text_english.txt");//英文测试
    string  word;
    cin >> word; 
     set<string> set_index = I.run_index(word);

     I.printall();  // 测试函数

     set<string>::iterator it = set_index.begin();
     set<string>::iterator itend = set_index.end();
     cout << "main_iteartor" << endl;
     for(;it != itend; ++it)
     { 
         cout << (*it) << endl;
       
     }

    return 0;
}
