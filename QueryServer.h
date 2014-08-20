#ifndef SERVER_H
#define SERVER_H 
#include <echo/TcpServer.h>
#include <echo/ThreadPool.h>
#include "query.h"

class QueryServer:NonCopyable{
    public:
        QueryServer(const InetAddress &addr, const std::string &);

        void start();
    private:
        void onConnection(const TcpConnectionPtr &);
        void onMessage(const TcpConnectionPtr &);
        void runQuery(const std::string &, const TcpConnectionPtr &);
        
        TcpServer server_;
        Query query_;
        ThreadPool pool_;
};
#endif  /*SERVER_H*/
