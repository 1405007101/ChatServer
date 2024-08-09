#include "groupmodel.hpp"
#include "db.h"

// 创建群组
bool GroupModel::createGroup(Group &group)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql,"insert into AllGroup(groupname,groupdesc) values('%s','%s')",
    group.getName().c_str(),group.getDesc().c_str());
    MySQL mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            // 根据插入的键值对生成群组id
            group.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}

// 加入群组
void GroupModel::addGroup(int userid,int groupid,string role)
{
    // 组装sql语句
    char sql[1024]= {0};
    sprintf(sql,"insert into GroupUser values(%d,%d,'%s')",groupid,userid,role.c_str());
    MySQL mysql;
    if(mysql.connect())
    {
        mysql.update(sql);
    }
}

// 查询所在群的群组信息
vector<Group> GroupModel::queryGroup(int userid)
{
    /*
    1. 先根据userid在GroupUser表中查询出该用户所属的群组信息
    2. 再根据群组信息，查询属于该群组所有用户的userid，并且和User表进行多表联合查询，查出用户的详细信息 相当于查两次
    */

    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql,"select a.id,a.groupname,a.groupdesc from AllGroup a inner join \
    GroupUser b on b.groupid = a.id where b.userid = %d",userid);//根据用户查询所在群信息

    vector<Group> groupvec;

    MySQL mysql;
    if(mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if(nullptr != res)
        {
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr)
            {
                Group group;
                group.setId(atoi(row[0]));
                group.setName(row[1]);
                group.setDesc(row[2]);
                groupvec.push_back(group);
            }
            mysql_free_result(res);
        }
    }

    // 查询群组的用户信息 在上面查到的群中查询群成员
    for(Group &group : groupvec)
    {
        sprintf(sql, "select a.id,a.name,a.state,b.grouprole from User a inner join \
        GroupUser b on b.userid = a.id where b.groupid = %d",
        group.getId()); // 根据群号查询群成员

        MYSQL_RES *res = mysql.query(sql);
        if (nullptr != res)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                GroupUser user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                user.setRole(row[3]);
                group.getUsers().push_back(user);
            }
            mysql_free_result(res);
        }
    }
    return groupvec;
}

// 根据指定的groupid查询群组用户id列表，除了userid自己除外，主要用户群聊业务给群组其他的成员群发消息
vector<int> GroupModel::queryGroupUsers(int userid,int groupid)
{
    // 组装sql语句
    char sql[1024] = {0};
    sprintf(sql,"select userid from GroupUser where groupid = %d and userid != %d",groupid,userid);
    vector<int> idVec;
    MySQL mysql;
    if(mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if(res != nullptr)
        {
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr)
            {
                idVec.push_back(atoi(row[0]));
            }
            mysql_free_result(res);
        }
    }
    return idVec;   
}