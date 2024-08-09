# ChatServer
基于muduo网络库的集群聊天服务器，使用nginx负载均衡算法为用户分配服务器，redis的发布订阅功能实现跨服务器的通信，使用了连接池提高数据库的存取性能

cmake ..
make

##
./autobuild.sh
