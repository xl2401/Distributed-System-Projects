#include<iostream>
#include<unordered_map>
#include<list>

using namespace std;

struct CacheEntry{
    string filename;
    char *ptr;
    int filesize;
    CacheEntry(string f,char *p, int s):filename(f), ptr(p),filesize(s){};
};

class LRUCache{
    public:
        LRUCache(){};
        LRUCache(int capacity,string rootpath)
        {
            m_capacity = capacity;
            root = rootpath;
        }
        CacheEntry *get (string filename, int &status);
        char *read_in_memory(string filename,int &size);
   private:
        string root;
        list<CacheEntry*> m_LRU_Cache;
        int m_capacity;
        int used_capacity;
        unordered_map<string, list<CacheEntry *> :: iterator> cache_map;
        void movetoHead(string filename)
        {
            CacheEntry *updatedEntry = *cache_map[filename];
            m_LRU_Cache.erase(cache_map[filename]);
            m_LRU_Cache.push_front(updatedEntry);
            cache_map[filename] = m_LRU_Cache.begin();
        }
};
