/*
 * @Author: runwiththewind23 19945243142@163.com
 * @Date: 2024-08-08 16:28:28
 * @LastEditors: runwiththewind23 19945243142@163.com
 * @LastEditTime: 2024-08-08 16:28:33
 * @FilePath: /sxd/chat/src/redis_pool/main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "redis_connection_pool.h"
#include <hiredis/hiredis.h>
#include <iostream>

int main() {
  // Get the singleton instance of the connection pool
  redis_connection_pool *pool = redis_connection_pool::GetInstance();

  // Initialize the connection pool with the desired parameters
  pool->init("127.0.0.1", "6379", 10);

  // Retrieve a connection from the pool
  redisContext *connection;
  redis_connectionRAII connRAII(&connection, pool);

  // Verify the connection is not null
  if (connection == nullptr) {
    std::cerr << "Failed to obtain a connection from the pool." << std::endl;
    return 1;
  }

  // Test the connection with a simple Redis command
  redisReply *reply = (redisReply *)redisCommand(connection, "PING");
  if (reply == nullptr) {
    std::cerr << "Failed to execute command: " << connection->errstr
              << std::endl;
    return 1;
  }

  std::cout << "PING Response: " << reply->str << std::endl;
  freeReplyObject(reply);

  // Use the connection to set a key-value pair
  reply = (redisReply *)redisCommand(connection, "SET %s %s", "test_key",
                                     "Hello, Redis!");
  if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) {
    std::cerr << "Failed to execute SET command: "
              << (reply ? reply->str : connection->errstr) << std::endl;
    return 1;
  }
  std::cout << "SET command executed successfully." << std::endl;
  freeReplyObject(reply);

  // Use the connection to get the value of the key
  reply = (redisReply *)redisCommand(connection, "GET %s", "test_key");
  if (reply == nullptr || reply->type == REDIS_REPLY_ERROR) {
    std::cerr << "Failed to execute GET command: "
              << (reply ? reply->str : connection->errstr) << std::endl;
    return 1;
  }
  std::cout << "GET Response: " << reply->str << std::endl;
  freeReplyObject(reply);

  return 0;
}
