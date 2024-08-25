#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include<netinet/tcp.h>

class handleclient {
private:
  int cfd;

public:
  handleclient();
  ~handleclient();
  void init();
  static void *handle_send(void *arg);
  static void *handle_recv(void *arg);
  void run();
};