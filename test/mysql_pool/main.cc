/*
 * @Author: runwiththewind23 19945243142@163.com
 * @Date: 2024-08-08 13:45:07
 * @LastEditors: runwiththewind23 19945243142@163.com
 * @LastEditTime: 2024-08-08 15:13:54
 * @FilePath: /sxd/chat/src/mysql_pool/main.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "mysql_connection_pool.h"
#include <iostream>
#include <thread>
mysql_connection_pool *pool = mysql_connection_pool::GetInstance();
void testConnectionPool() {

  pool->init("localhost", "root", "123", "user_chat", 3306, 10);

  MYSQL *conn = nullptr;
  {
    mysql_connectionRAII mysqlConn(&conn, pool);
    // Perform database operations using conn
    if (conn) {
      std::cout << "Connected to the database successfully!" << std::endl;
    } else {
      std::cerr << "Failed to connect to the database!" << std::endl;
    }
  }
  // Connection is automatically released when mysql_connectionRAII goes out of
  // scope
}

int main() {
  std::thread t1(testConnectionPool);
  std::thread t2(testConnectionPool);
  std::thread t3(testConnectionPool);

  t1.join();
  t2.join();
  t3.join();
  return 0;
}
