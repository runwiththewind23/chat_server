/*
 * @Author: runwiththewind23 19945243142@163.com
 * @Date: 2024-08-09 12:36:50
 * @LastEditors: runwiththewind23 19945243142@163.com
 * @LastEditTime: 2024-08-09 15:02:56
 * @FilePath: /sxd/chat/src/server.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "handleserver.h"
#include <arpa/inet.h>
#include <chrono>
#include <fcntl.h>
#include <iostream>
#include <queue>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

int main() {

  //需要修改的数据库信息,登录名,密码,库名
  string port = "3306";
  string user = "root";
  string passwd = "123";
  string databasename = "user_chat";
  int mysql_num = 8;

  string redis_port = "6379";
  string redis_ip = "127.0.0.1";
  int redis_num = 8;

  handleserver server(port, user, passwd, databasename, mysql_num, redis_ip,
                      redis_port, redis_num);

  server.run();
}
