// the lock server implementation

#include "lock_server.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;
lock_server::lock_server()
{
    pthread_mutex_init(&m, NULL);
}

lock_protocol::status
lock_server::stat(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;
  printf("stat request from clt %d\n", clt);
  r = Lock_nacquire[lid];
  return ret;
}

lock_protocol::status
lock_server::acquire(int clt, lock_protocol::lockid_t lid, int &r){
       ScopedLock ml(&m);
       Lock_nacquire[lid]++;
       while(Lock_acquired.find(lid)!=Lock_acquired.end())
       {
           assert(pthread_cond_wait(&lock_con_map[lid],&m)==0);
       }
       lock_protocol::status ret = lock_protocol::OK;
       printf("acquire request from clt %d\n",clt);
       if(lock_con_map.find(lid)==lock_con_map.end())
       {
           pthread_cond_t lock_can_acquire;
           pthread_cond_init (&lock_can_acquire, NULL);
           lock_con_map[lid] = lock_can_acquire;
       }
       Lock_acquired.insert(lid);
       return ret;
 }

lock_protocol::status
lock_server::release(int clt, lock_protocol::lockid_t lid, int &r)
{
    ScopedLock ml(&m);
    Lock_acquired.erase(lid);
    pthread_cond_signal(&lock_con_map[lid]);
    lock_protocol::status ret = lock_protocol::OK;
    printf("release request from clt %d\n",clt);
    return ret;
}




