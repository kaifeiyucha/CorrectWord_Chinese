#include "QueryServer.h"
#include <string>
using namespace std;
using namespace std::placeholders;


QueryServer::QueryServer(const InetAddress &addr,
                         const string &filename)
    :server_(addr),
     query_(filename),
     pool_(100,4)
{
    server_.setConnection(bind(&QueryServer::onConnection, this, _1));
    server_.setMessage(bind(&QueryServer::onMessage, this, _1));
}

void QueryServer::onConnection(const TcpConnectionPtr &conn){
    conn->send("please input word:\r\n");
}


void QueryServer::onMessage(const TcpConnectionPtr &conn){
    string word(conn->receive());
    pool_.addTask(bind(&QueryServer::runQuery, this, word, conn));
}

void QueryServer::runQuery(const string &s, const TcpConnectionPtr  &conn){
    string word = s;
    if(word.substr(word.size()-2, 2)=="\r\n"){
        word.erase(word.size()-1);
        word.erase(word.size()-1);
    }
    string res = query_.run_query(word);
    conn->send(res + "\r\n");
}

void QueryServer::start(){
    pool_.start();
    server_.start();
}

