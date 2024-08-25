/*
 * @Author: runwiththewind23 19945243142@163.com
 * @Date: 2024-08-09 11:28:28
 * @LastEditors: runwiththewind23 19945243142@163.com
 * @LastEditTime: 2024-08-09 16:29:52
 * @FilePath: /sxd/chat/src/handleserver.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once

#include "bitset.h"
#include "lock.h"
#include "mysql_connection_pool.h"
#include "redis_connection_pool.h"
#include "ThreadPool.h"
#include <arpa/inet.h>
//#include <boost/asio.hpp>
//#include <boost/bind.hpp>
#include <fcntl.h>
#include <iostream>
#include <limits.h>
#include <mysql/mysql.h>
#include <netinet/in.h>
#include <pthread.h>
#include <set>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include<netinet/tcp.h>
class handleserver {
private:
  locker mutex;
  locker group_mutex;
  std::unordered_map<std::string, int> name_sock_map;
  std::unordered_map<int, set<int>> group_map;
  std::string m_port;
  mysql_connection_pool *mysql_connPool;
  std::string m_user;
  std::string m_passwd;
  std::string m_databasename;
  int m_mysql_num;
  // redis
  redis_connection_pool *m_redis_connPool;
  std::string m_redis_ip;
  std::string m_redis_port;
  int m_redis_num;
  bitset<UINT_MAX> bloom_filter;

public:
  handleserver(std::string port, std::string user, std::string passwd,
               std::string databasename, int mysql_num, std::string redis_ip,
               std::string redis_port, int redis_num) {
    m_port = port;
    m_user = user;
    m_passwd = passwd;
    m_databasename = databasename;
    m_mysql_num = mysql_num;

    // redis
    m_redis_ip = redis_ip;
    m_redis_port = redis_port;
    m_redis_num = redis_num;
  }
  ~handleserver();
  void run();
  void thread_pool();
  void mysql_pool();
  void handle_all_request(int arg);
  void login_user(std::string str, int cfd, bool &if_login,
                  std::string &login_name);
  void register_user(string str, int cfd);
  void test_redis();
  void exits_user(string str, int cfd);
  size_t hash_func(string name);
  void bloom_init();
};