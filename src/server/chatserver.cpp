#include "chatserver.hpp"
#include "json.hpp"
#include "chatservice.hpp"

#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

//网络模块

// 初始化聊天服务器对象
ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    // 注册链接回调
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

    // 注册消息回调
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

    // 设置线程数量
    _server.setThreadNum(4);
}

// 启动服务
void ChatServer::start()
{
    _server.start();
}

// 上报链接相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    // 客户端断开链接
    if (!conn->connected())
    {
        //instance返回一个ChatService 服务对象
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

// 上报读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn,
                           Buffer *buffer,
                           Timestamp time)
{
    string buf = buffer->retrieveAllAsString();

    // 测试，添加json打印代码
    cout << buf << endl;

    // 数据的反序列化:json的序列化和反序列化，序列化是将js对象转换为json,反序列化是将json转换为js对象
    json js = json::parse(buf);
    //cout<<js<<endl;
    // 达到的目的：完全解耦网络模块的代码和业务模块的代码
    // 通过js["msgid"] 获取=》业务handler=》conn  js  time
    //using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;
    //auto msgHandler 相当于 MsgHandler msgHandler，然后在下面进行回调函数的调用
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>()); // 这一步是服务器解析客户端发来的数据得知要进行什么业务，是聊天还是加好友。。。
    // 回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, time); // 这一步是将发来的数据连同连接数据作为参数传给回调函数
}