#pragma once
#include "lock.h"
#include "mysql/mysql.h"
#include <list>
#include <string>
class mysql_connection_pool {
private:
  int m_MaxConn; //最大连接数
  int m_CurConn; //当前已使用的连接数
  int m_FreeConn;
  locker lock;
  std::list<MYSQL *> connList; //连接池
  sem reserve;
  mysql_connection_pool();
  ~mysql_connection_pool();

public:
  std::string m_url;          //主机地址
  std::string m_port;         //数据库端口号
  std::string m_user;         //登录数据库用户名
  std::string m_passwd;       //登录数据库密码
  std::string m_databasename; //使用数据库名
  //单例模式
  static mysql_connection_pool *GetInstance();
  //初始化
  void init(const std::string &url, const std::string &user,
            const std::string &passwd, const std::string &DBName, int port,
            int MaxConn);

  MYSQL *GetConnection();              //获取数据库连接
  int GetFreeConn();                   //获取连接
  bool ReleaseConnection(MYSQL *conn); //释放连接
  void DestroyPool();                  //销毁连接
};

class mysql_connectionRAII {
private:
  MYSQL *connRAII;
  mysql_connection_pool *poolRAII;

public:
  mysql_connectionRAII(MYSQL **conn, mysql_connection_pool *connPool);
  ~mysql_connectionRAII();
};