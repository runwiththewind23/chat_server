#include "mysql_connection_pool.h"
#include <iostream>
#include <list>
#include <mysql/mysql.h>
#include <pthread.h>
#include <stdio.h>
#include <string>
mysql_connection_pool::mysql_connection_pool() {}
mysql_connection_pool::~mysql_connection_pool() { DestroyPool(); }

mysql_connection_pool *mysql_connection_pool::GetInstance() {
  static mysql_connection_pool connPool;
  return &connPool;
}

// 初始化连接池
void mysql_connection_pool::init(const std::string &url,
                                 const std::string &user,
                                 const std::string &passwd,
                                 const std::string &DBName, int port,
                                 int MaxConn) {
  m_url = url;
  m_port = port;
  m_user = user;
  m_passwd = passwd;
  m_databasename = DBName;

  for (int i = 0; i < MaxConn; ++i) {
    MYSQL *conn = mysql_init(nullptr);

    if (NULL == conn) {
      printf("mysql_init() 失败了，原因是：%s\n", mysql_error(conn));
      exit(1);
    }

    conn = mysql_real_connect(conn, url.c_str(), user.c_str(), passwd.c_str(),
                              DBName.c_str(), port, nullptr, 0);

    if (NULL == conn) {
      printf("mysql_real_connect() 失败了，原因是：%s\n", mysql_error(conn));
      exit(1);
    }

    connList.push_back(conn);
    ++m_FreeConn;
  }

  reserve = sem(m_FreeConn);
  m_MaxConn = m_FreeConn;
}

// 获取一个数据库连接
MYSQL *mysql_connection_pool::GetConnection() {
  MYSQL *conn = nullptr;

  if (connList.size() == 0) {
    return nullptr;
  }

  reserve.wait();

  lock.lock();

  conn = connList.front();
  connList.pop_front();

  --m_FreeConn;
  ++m_CurConn;

  lock.unlock();
  return conn;
}

// 获取空闲连接数量
int mysql_connection_pool::GetFreeConn() { return this->m_FreeConn; }

// 释放连接
bool mysql_connection_pool::ReleaseConnection(MYSQL *conn) {
  if (conn == nullptr) {
    return false;
  }

  lock.lock();

  connList.push_back(conn);
  ++m_FreeConn;
  --m_CurConn;

  lock.unlock();

  reserve.post();
  return true;
}

// 销毁连接池
void mysql_connection_pool::DestroyPool() {
  lock.lock();

  if (connList.size() > 0) {
    for (MYSQL *conn : connList) {
      mysql_close(conn);
    }
    m_CurConn = 0;
    m_FreeConn = 0;
    connList.clear();
  }

  lock.unlock();
}

// RAII class implementation
mysql_connectionRAII::mysql_connectionRAII(MYSQL **conn,
                                           mysql_connection_pool *connPool) {
  *conn = connPool->GetConnection();
  connRAII = *conn;
  poolRAII = connPool;
}

mysql_connectionRAII::~mysql_connectionRAII() {
  poolRAII->ReleaseConnection(connRAII);
}
