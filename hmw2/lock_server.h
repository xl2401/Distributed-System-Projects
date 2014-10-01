// this is the lock server
// the lock client has a similar interface

#ifndef lock_server_h
#define lock_server_h

#include <string>
#include "lock_protocol.h"
#include "lock_client.h"
#include "rpc.h"
#include <map>
#include <set>
class lock_server {
     protected:
          std::map<lock_protocol::lockid_t , int> Lock_nacquire;
          std::set<lock_protocol::lockid_t> Lock_acquired;
          std::map<lock_protocol::lockid_t, pthread_cond_t> lock_con_map;
          pthread_mutex_t m; 
     public:
          lock_server();
          ~lock_server() {};
          lock_protocol::status stat(int clt, lock_protocol::lockid_t lid, int &);
          lock_protocol::status acquire(int clt, lock_protocol::lockid_t lid, int &);
          lock_protocol::status release(int clt, lock_protocol::lockid_t lid, int &);
};

#endif
