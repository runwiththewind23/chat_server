/*
 * @Author: runwiththewind23 19945243142@163.com
 * @Date: 2024-08-08 20:14:58
 * @LastEditors: runwiththewind23 19945243142@163.com
 * @LastEditTime: 2024-08-09 11:24:55
 * @FilePath: /sxd/chat/src/client.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "handleclient.h"
#include <iostream>
int main() {
  // Create a handleclient object
  handleclient client;

  // Initialize the client (create socket and connect to server)
  client.init();

  // Run the client (handle login/registration and chat)
  client.run();

  return 0;
}
