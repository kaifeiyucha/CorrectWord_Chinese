#include "query.h"
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "index.h"
//#include "lib.h"
#include "cache.h"
#include <stdint.h>

using namespace std;

#define ERR_EXIT(m)\
    do{\
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

Query::Query(const string &file)
    :cache_(1024),
     index_(file){

//     wordusr_ = word;//这个可以去掉
    open_lexicon(file);//打开词库
}


void Query::open_lexicon(const string &filename){//打开词库
    ifstream infile;
    infile.open(filename.c_str());
    if(!infile)
        ERR_EXIT("open file fail");
    read_from_lexicon(infile);//从词库中读取数据
    infile.close();
}

string Query::find_to_cache(const string &word){//在cache中寻找
   
    string word_right = cache_.get(word);//从cache中读取
    string str;

   if(!word_right.empty()){//不为空，代表cache中有这个单词
        str = word_right + string("\r\n");
        cout << "find in cache" << endl;
        return str;
   }else{
        cout << "cache isn't exit the word" << endl;
        return string();//返回空对象
   }
}



string Query::find_to_index(const string &word){//在index中寻找
    set<string>  tempset = index_.run_index(word);//返回查询词的所有候选索引
    set<string>::const_iterator loc = tempset.find(word);
    if(loc != tempset.end()){//在索引中找到了
        cache_.set(word, *loc);//放在cache中

        string final = word+"\r\n";

        cout << "find in index" << endl;
        return final;
    }else{
        return string();
    }
}

string Query::final_word(const string &word){//根据最小编辑距离 词频 纠错
    
    priority_queue<pqueue> que;
    string str;
    map<string, int>::const_iterator it = words_.begin();
    for( ; it != words_.end(); ++it){
	int mini = edit_distance(word, it->first);//mini editdistance
        if(mini < 3){
               add_priority(it->first,mini, it->second , que);
        }else{
            continue;
        }

    }
   str =  printf_result(que);

   cache_.get(str);
   return str;
    
}

int Query::get_len_UTF8(unsigned char c){
    int cnt = 0;
    while(c & (1 << (7-cnt))){
        ++cnt;
    }
    return cnt;
}

void Query::to_stringUTF8(const string &s, vector<uint32_t> &vec){
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

int Query::min_dis(int a, int b, int c){
    int ret = (a<b) ? a : b;
    ret = (ret < c) ? ret : c;
    return ret;
}

int Query::edit_distance_uint32(vector<uint32_t> &w1, vector<uint32_t> &w2){
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

int Query::edit_distance(const string &word1, const string &word2){
    vector<uint32_t>  w1, w2;
    to_stringUTF8(word1, w1);
    to_stringUTF8(word2, w2);
    return edit_distance_uint32(w1, w2);
}


string Query::run_query(const string &word){//入口程序，查询
    
    string word_right = find_to_cache(word);//先从cache中查询
    if(!word_right.empty())
        return word_right;
 
    word_right = find_to_index(word);
    if(!word_right.empty())//此时，词库中有这个词，直接输出
        return word_right;

    else
        return final_word(word);//最后根据最小编辑距离和词频，寻找恰当的词
    
    
}

void Query::read_from_lexicon(ifstream &infile){//从词库中读取数据
            
    string line;
    while(getline(infile, line)){
         char buf[100];
         int frequence;
         sscanf(line.c_str() ,"%s %d", buf, &frequence);
         words_[string(buf)] = frequence;
    }
}



void Query::add_priority(const string &word_old, int mini, int frequence,priority_queue<pqueue>  &que){
        
    pqueue q;

    q.word_ = word_old;
    q.mini_distance_ = mini;
    q.frequency_ = frequence;

    que.push(q);
}



string Query::printf_result(priority_queue<pqueue>  que){
  
    string str;
    str = que.top().word_;
    return str;
}
