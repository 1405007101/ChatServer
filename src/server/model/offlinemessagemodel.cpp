#include <iostream>
#include "offlinemessagemodel.hpp"
#include "db.h"

using namespace std;

// 存储用户的离线消息
void OfflineMsgModel::insert(int userid,string msg)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql,"insert into OfflineMessage value(%d, '%s')",userid,msg.c_str());

    MySQL mysql;
    if( mysql.connect())
    {
        mysql.update(sql);
    }
}

// 删除用户的离线消息
void OfflineMsgModel::remove(int userid)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql,"delete from OfflineMessage where userid = %d",userid);

    MySQL mysql;
    if(mysql.connect())
    {
        mysql.update(sql);
    }
}
   
// 查询用户的离线消息
vector<string> OfflineMsgModel::query(int userid)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql,"select message from OfflineMessage where userid = %d",userid);

    vector<string> vec;
    MySQL mysql;
    if(mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if(nullptr != res)
        {
            //把userid用户的所有离线消息放入vec中返回
            MYSQL_ROW row;// row指的就是sql语句中所要查询的内容 message
            // 每次调用mysql_fetch_row(res)时，都会从MySQL的结果集中检索下一条数据。当你调用这个函数时，它会移动内部的游标到下一行，和row[0]、row[1]....不同，这些都是每一条数据中的内容
            while(nullptr != (row = mysql_fetch_row(res)))
            {
                vec.push_back(row[0]);
            }
            mysql_free_result(res);
            return vec;
        }
    }
    return vec;
}