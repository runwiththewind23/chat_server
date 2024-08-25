#include "handleclient.h"
handleclient::handleclient() {}
handleclient::~handleclient() {}
void handleclient::init() {
  // 1.创建套接字
  cfd = socket(AF_INET, SOCK_STREAM, 0);
  if (cfd == -1) {
    perror("socket");
    exit(0);
  }
  int on = 1;
int result = setsockopt(cfd, IPPROTO_TCP, TCP_NODELAY, (char *)&on, sizeof(int));
if (result == -1) {
    std::cout << "Close Nagle error" << std::endl;
}
  // 2.连接服务器
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(5080); //大端端口
  inet_pton(AF_INET, "192.168.123.228", &addr.sin_addr.s_addr);
  if (connect(cfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("connect");
    exit(0);
  }
}

void *handleclient::handle_send(void *arg) {
  int sock = *(int *)arg;
  while (1) {
    std::string str;
    std::cin >> str;
    if (str == "exit")
      break;
    if (sock > 0) {
      str = "content: " + str;
      send(sock, str.c_str(), str.length(), 0);
    } else if (sock < 0) {
      str = "gr_message: " + str;
      send(-sock, str.c_str(), str.length(), 0);
    }
  }
}

void *handleclient::handle_recv(void *arg) {
  int sock = *(int *)arg;
  while (1) {
    char recv_buffer[10000];
    memset(recv_buffer, 0, sizeof(recv_buffer));
    if (recv(sock, recv_buffer, sizeof(recv_buffer), 0) == 0)
      continue;
    //else if (recv(sock, recv_buffer, sizeof(recv_buffer), 0) == -1)
    //  break;
    std::cout << std::string(recv_buffer) << std::endl;
  }
}

void handleclient::run() {
  int choice;
  std::string name, pass, pass1;
  bool if_login = false;  //记录是否成功登录
  std::string login_name; //记录成功登录的用户名
  std::ifstream f;
  f.open("cookie.txt", std::ios::in | std::ios::out);
  std::string cookie_str;
  if (f.good()) {
    f >> cookie_str;
    f.close();
    cookie_str = "cookie: " + cookie_str;
    std::cout << cookie_str << std::endl;
    send(cfd, cookie_str.c_str(), cookie_str.length() + 1, 0);
    char cookie_ans[100];
    memset(cookie_ans, 0, sizeof(cookie_ans));
    recv(cfd, cookie_ans, sizeof(cookie_ans), 0);
    std::string ans_str(cookie_ans);
    if (ans_str != "NULL") {
      if_login = true;
      login_name = ans_str;
    }
  } // good函数用于判断流是否能正常读写
  if (!if_login) {
    std::cout << " ------------------\n";
    std::cout << "|                  |\n";
    std::cout << "| 请输入你要的选项:|\n";
    std::cout << "|    0:退出        |\n";
    std::cout << "|    1:登录        |\n";
    std::cout << "|    2:注册        |\n";
    std::cout << "|                  |\n";
    std::cout << " ------------------ \n\n";
  }
  //事务处理
  while (1) {
    if (if_login)
      break;
    std::cin >> choice;
    if (choice == 0)
      break;
    else if (choice == 1 && !if_login) {
      while (1) {
        std::cout << "用户名：";
        std::cin >> name;
        std::cout << "密码：";
        std::cin >> pass;
        std::string str = "login" + name;
        str += "pass:";
        str += pass;
        send(cfd, str.c_str(), str.length(), 0); //发送登录信息
        char buffer[1000];
        memset(buffer, 0, sizeof(buffer));
        recv(cfd, buffer, sizeof(buffer), 0); //接收响应
        std::string recv_str(buffer);
        if (recv_str.substr(0, 2) == "ok") {
          if_login = true;
          login_name = name;
          std::string tmpstr = recv_str.substr(2);
          tmpstr = "cat > cookie.txt <<end \n" + tmpstr + "\nend";
          system(tmpstr.c_str()); // system会调执行系统命令

          std::cout << "登陆成功\n\n";
          break;
        } else {
          std::cout << "密码或用户名错误" << std::endl;
        }
      }
    }
    //注册
    else if (choice == 2) {
      std::cout << "注册的用户名:";
      std::cin >> name;
      while (1) {
        std::cout << "密码:";
        std::cin >> pass;
        std::cout << "确认密码:";
        std::cin >> pass1;
        if (pass == pass1)
          break;
        else
          std::cout << "两次密码不一致!\n\n";
      }
      name = "name:" + name;
      pass = "pass:" + pass;
      std::string str = name + pass;
      send(cfd, str.c_str(), str.length(), 0);
      std::cout << "注册成功！\n";
      std::cout << "\n继续输入你要的选项:";
    }

    if (if_login)
      break; //登陆成功
  }
  while (if_login && 1) {
    if (if_login) {
      system("clear");
      std::cout << "        欢迎回来," << login_name << std::endl;
      std::cout << " -------------------------------------------\n";
      std::cout << "|                                           |\n";
      std::cout << "|          请选择你要的选项：               |\n";
      std::cout << "|              0:退出                       |\n";
      std::cout << "|              1:发起单独聊天               |\n";
      std::cout << "|              2:发起群聊                   |\n";
      std::cout << "|                                           |\n";
      std::cout << " ------------------------------------------- \n\n";
    }
    std::cin >> choice;
    pthread_t send_t, recv_t; //线程ID
    void *thread_return;
    if (choice == 0)
      break;
    if (choice == 1) {
      std::cout << "请输入对方的用户名:";
      std::string target_name, content;
      std::cin >> target_name;
      std::string sendstr("target:" + target_name +
                          "from:" + login_name); //标识目标用户+源用户
      send(cfd, sendstr.c_str(), sendstr.length(),
           0); //先向服务器发送目标用户、源用户
      std::cout << "请输入你想说的话(输入exit退出)：\n";
      auto send_thread = pthread_create(&send_t, NULL, handle_send,
                                        (void *)&cfd); //创建发送线程
      auto recv_thread = pthread_create(&recv_t, NULL, handle_recv,
                                        (void *)&cfd); //创建接收线程
      pthread_join(send_t, &thread_return);
      // pthread_join(recv_t,&thread_return);
      pthread_cancel(recv_t);
    }
    /*
    线程取消的意思就是**在某些特定情况下在一个线程中杀死另一个线程**。使用这个函数杀死一个线程需要分两步：
  ​
  在线程A中**调用线程取消函数pthread_cancel**，指定杀死线程B，这时候线程B是死不了的
  ​
  在线程B中**进程一次系统调用（从用户区切换到内核区）**，否则线程B可以一直运行。
    */
    if (choice == 2) {
      std::cout << "请输入群号:";
      int num;
      std::cin >> num;
      std::string sendstr("group:" + std::to_string(num));
      send(cfd, sendstr.c_str(), sendstr.length(), 0);
      std::cout << "请输入你想说的话(输入exit退出)：\n";
      int cfd1 = -cfd;
      auto send_thread = pthread_create(&send_t, NULL, handle_send,
                                        (void *)&cfd1); //创建发送线程
      auto recv_thread = pthread_create(&recv_t, NULL, handle_recv,
                                        (void *)&cfd); //创建接收线程
      pthread_join(send_t, &thread_return);
      pthread_cancel(recv_t);
    }
  }
  std::cout << 1 << std::endl;
}
