#ifndef QUERY_H
#define QUERY_H

#include <queue>
#include <string>
#include <stdio.h>
#include <iostream>
#include "index.h"
#include "cache.h"
//#include "lib.h"
#include <stdint.h>
#include <vector>

struct pqueue{
    std::string word_;
    int mini_distance_;
    int frequency_;
    friend  bool operator< (const pqueue &a, const pqueue &b){
        if(b.mini_distance_ < a.mini_distance_)
            return true;
        else if(b.mini_distance_ > a.mini_distance_)
            return false;

        else {
            if(b.frequency_ < a.frequency_ || b.frequency_ == a.frequency_)
                return true;
            else 
                return false;
        }
    }
};


class Query{
    public:
        Query(const std::string &);
        std::string run_query(const std::string &);//程序的入口
        std::string printf_result( std::priority_queue<pqueue> );
        void add_priority(const std::string &, int , int, std::priority_queue<pqueue> &);
    
    
    private:
        void open_lexicon(const std::string &);//open lexicon
        void  read_from_lexicon(std::ifstream &);//从词库中读入词条
        std::string find_to_cache(const std::string &);//到cache中寻找
        std::string find_to_index(const std::string &);//到index中寻找
        std::string final_word(const std::string &);//根据MiniDistance 和 词频 寻找

        int min_dis(int a, int b, int c);
    	int get_len_UTF8(unsigned char c);//这是计算string占几位
	    void to_stringUTF8(const std::string &, std::vector<uint32_t> &);//把字符串解析成uint32_t 的数组
	    int edit_distance_uint32(std::vector<uint32_t> &, std::vector<uint32_t> &);
	    int edit_distance(const std::string &word1, const std::string &word2);
    
        LRUCache cache_;
        Index index_;
        std::map<std::string, int>  words_;//词库<apple 3>
        
};
#endif
