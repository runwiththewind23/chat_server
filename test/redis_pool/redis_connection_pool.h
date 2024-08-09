#include "lock.h"
#include <hiredis/hiredis.h>
#include <iostream>
#include <list>
#include <string>

class redis_connection_pool {
private:
  redis_connection_pool();
  ~redis_connection_pool();
  int m_MaxConn; //最大连接数
  int m_CurConn; //当前已使用的连接数
  int m_FreeConn;
  locker lock;
  std::list<redisContext *> connList; //连接池
  sem reserve;

public:
  static redis_connection_pool *GetInstance();
  void init(std::string ip, std::string port, int MaxConn);
  std::string m_ip;                           //获取主机ip
  std::string m_port;                         //获取主机端口
  redisContext *GetConnection();              //获取数据库连接
  bool ReleaseConnection(redisContext *conn); //释放连接
  int GetFreeConn();                          //获取连接
  void DestroyPool();                         //销毁连接
};

class redis_connectionRAII {
public:
  redis_connectionRAII(redisContext **conn, redis_connection_pool *connPool);
  ~redis_connectionRAII();

private:
  redisContext *connRAII;
  redis_connection_pool *poolRAII;
};