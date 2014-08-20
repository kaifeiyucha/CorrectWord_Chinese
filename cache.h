#ifndef CACHE_H_
#define CACHE_H_ 


#include <iostream>
#include <map>
#include <list>
#include <utility>
#include <string>

class LRUCache{
public:
    LRUCache(int capacity) {
        exit = false;
        m_capacity = capacity ;
    }

    std::string get(std::string key);//从cache中直接取出单词，并将数据移到最前端
    void set(std::string key, std::string value);

private:
    size_t  m_capacity ;    //cashe的大小
    bool exit;
    std::list<std::pair<std::string, std::string> > caches ;  //用一个双链表存储cashe的内容
    std::map< std::string, std::list<std::pair<std::string, std::string> > :: iterator> cachesMap ;            //使用map加快查找的速度
};
#endif  /*CACHE_H_*/
