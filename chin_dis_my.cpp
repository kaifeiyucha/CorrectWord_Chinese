#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stdint.h>

using namespace std;

//这是计算string占几位
int get_len_UTF8(unsigned char c){
    int cnt = 0;
    while(c & (1 << (7-cnt))){
        ++cnt;
    }
    return cnt;
}

//把字符串解析成uint32_t 的数组
void to_stringUTF8(const string &s, vector<uint32_t> &vec){
    vec.clear();
    for(string::size_type i = 0; i != s.size(); i++){
        int len = get_len_UTF8(s[i]);
        uint32_t t = (unsigned char)s[i];
        if(len > 1 ){
            --len;
            //拼接字符串
            while(len){
                t = (t << 8)+(unsigned char)s[++i];
                len--;
            }
        }
        vec.push_back(t);
    }
}

int min_dis(int a, int b, int c){
    int ret = (a<b) ? a : b;
    ret = (ret < c) ? ret : c;
    return ret;
}

int edit_distance_uint32(vector<uint32_t> &w1, vector<uint32_t> &w2){
    int len_1 = w1.size();
    int len_2 = w2.size();
    int memo[100][100];
    memset(memo, 0, sizeof(memo));
    for(int i =1; i <= len_1; ++i)
        memo[i][0] = i;
    for(int j =1; j <= len_2; ++j)
        memo[0][j] = j;
    for( int i=1; i <= len_1; ++i){
        for(int j =1; j<= len_2; ++j){
            if(w1[i-1] == w2[j-1]){
                memo[i][j] = memo[i-1][j-1];
            }else{
                memo[i][j] = min_dis(memo[i-1][j-1], memo[i][j-1]+1, memo[i-1][j]+1);
            }
        }
    }

    return memo[len_1][len_2];
}




int edit_distance(const string &word1, const string &word2){
    vector<uint32_t>  w1, w2;
    to_stringUTF8(word1, w1);
    to_stringUTF8(word2, w2);
    return edit_distance_uint32(w1, w2);
}

int main(int argc, const char *argv[])
{
    string c1, c2;
    cin >> c1 >> c2;
    cout << edit_distance(c1, c2) << endl;
    return 0;
}
