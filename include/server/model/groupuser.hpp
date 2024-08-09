#ifndef GROUPUSER_H
#define GROUPUSER_H

#include "user.hpp"

// 群组用户，多了一个role角色信息：creator/normal，从User继承过来，可以复用User中的成员，登录时不但返回该用户所在群组，还有所在群组的其他成员的信息。
class GroupUser :public User
{
public:
    void setRole(string role){ this->role = role; }
    string getRole(){ return this->role; }
private:
    string role;
};

#endif