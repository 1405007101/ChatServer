#include "usermodel.hpp"
#include "db.h"
#include <iostream>
using namespace std;

// User表的增加方法  注册业务就是向MySQL表中插入用户的数据
bool UserModel::insert(User &user)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    //确保sql语句书写正确
    sprintf(sql, "insert into User(name, password, state) values('%s', '%s', '%s')",
            user.getName().c_str(), user.getPwd().c_str(), user.getState().c_str());

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            // 获取插入成功的用户数据生成的主键id  自己生成主键
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }

    return false;
}

// 根据用户号码查询用户信息
User UserModel::query(int id)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select * from User where id = %d", id);

    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);//动态的申请了指针空间，后面就需要被释放
        if (res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
                mysql_free_result(res);
                return user;
            }
        }
    }

    return User();
}

// 更新用户的状态信息
bool UserModel::updateState(User user)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "update User set state = '%s' where id = %d", user.getState().c_str(), user.getId());//c_str()方法将string转为char *类型

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}

// 重置用户状态信息
void UserModel::resetState()
{
    // 1.组装sql语句
    char sql[1024] = "update User set state = 'offline' where state = 'online'";

    MySQL mysql;
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}