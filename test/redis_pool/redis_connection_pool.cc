#include "redis_connection_pool.h"

redis_connection_pool::redis_connection_pool() {
  m_CurConn = 0;
  m_FreeConn = 0;
}

redis_connection_pool *redis_connection_pool::GetInstance() {
  static redis_connection_pool connPool;
  return &connPool;
}
void redis_connection_pool::init(std::string ip, std::string port,
                                 int MaxConn) {
  m_ip = ip;
  m_port = port;
  for (int i = 0; i < MaxConn; i++) {
    redisContext *conn = redisConnect("127.0.0.1", 6379);
    if (conn->err) {
      redisFree(conn);
      std::cout << "连接redis失败" << std::endl;
    }
    connList.push_back(conn);
    ++m_FreeConn;
  }
  reserve = sem(m_FreeConn);
  m_MaxConn = m_FreeConn;
}

////当有请求时，从数据库连接池中返回一个可用连接，更新使用和空闲连接数
redisContext *redis_connection_pool::GetConnection() {
  redisContext *conn = nullptr;
  if (connList.size() == 0)
    return nullptr;
  reserve.wait();
  lock.lock();
  conn = connList.front();
  connList.pop_front();
  --m_FreeConn;
  ++m_CurConn;
  lock.unlock();
  return conn;
}

//释放当前使用的连接
bool redis_connection_pool::ReleaseConnection(redisContext *conn) {
  if (conn == nullptr)
    return false;
  lock.lock();
  connList.push_back(conn);
  ++m_FreeConn;
  --m_CurConn;
  lock.unlock();
  reserve.post();
  return true;
}

//销毁数据库连接池
void redis_connection_pool::DestroyPool() {
  lock.lock();
  if (connList.size() > 0) {
    std::list<redisContext *>::iterator it;
    for (it = connList.begin(); it != connList.end(); ++it) {
      redisContext *conn = *it;
      redisFree(conn);
    }
    m_CurConn = 0;
    m_FreeConn = 0;
    connList.clear();
  }
  lock.unlock();
}
//当前空闲的连接数
int redis_connection_pool::GetFreeConn() { return this->m_FreeConn; }
redis_connection_pool::~redis_connection_pool() { DestroyPool(); }
redis_connectionRAII::redis_connectionRAII(redisContext **conn,
                                           redis_connection_pool *connPool) {
  *conn = connPool->GetConnection();
  connRAII = *conn;
  poolRAII = connPool;
}
redis_connectionRAII::~redis_connectionRAII() {
  poolRAII->ReleaseConnection(connRAII);
}
