#include "cache.h"
#include <string>
#include <list>
#include <map>
#include <assert.h>

using namespace std;
/*
bool LRUCache::judgement_cache(const string &key){//判断  错的word是否存在在cache中
        
    map<string, list<pair<string, string> > :: iterator> ::iterator it = cachesMap.find(key) ;
        if (it != cachesMap.end()){//存在
            exit = true;
        }else{
            exit = false;
        }
        

}
*/


string LRUCache::get(string key) {  //从cache中读取，并把数据移到list的最前端
        string retValue = "" ;
        map<string, list<pair<string, string> > :: iterator> ::iterator it = cachesMap.find(key) ;

         //如果在Cache中，将记录移动到链表的最前端
        if (it != cachesMap.end())
          {
                //如何加断言
            
             retValue = it ->second->second ;
            //移动到最前端
            list<pair<string, string> > :: iterator ptrPair = it -> second ;
            pair<string, string> tmpPair = *ptrPair ;
            caches.erase(ptrPair) ;
            caches.push_front(tmpPair) ;

            //修改map中的值
            cachesMap[key] = caches.begin() ;
        }
        return retValue ;    
    }


void LRUCache::set(string key, string value) {//往cache中添加数据

        map<string, list<pair<string, string> > :: iterator> ::iterator it = cachesMap.find(key) ;

        if (it != cachesMap.end()) //已经存在其中,新申请一个变量，更新key所对应的value,同时把它放在最前面
        {
            list<pair<string, string> >::iterator ptrPait = it->second;
            ptrPait->second = value ; 
            pair<string , string > tmpPair = *ptrPait ;
            caches.erase(ptrPait) ;
            caches.push_front(tmpPair) ;

            //更新map
            cachesMap[key] = caches.begin() ;
        }
        else //不存在其中
        {
            pair<string , string > tmpPair = make_pair(key, value) ;

            if (m_capacity == caches.size()) //已经满
            {
                string delKey = caches.back().first ;
                caches.pop_back() ; //删除最后一个

                //删除在map中的相应项
                map<string, list<pair<string, string> > :: iterator> ::iterator delIt = cachesMap.find(delKey) ;
                cachesMap.erase(delIt) ;
            }

            caches.push_front(tmpPair) ;
            cachesMap[key] = caches.begin() ; //更新map，并放在开头
        }
    }
