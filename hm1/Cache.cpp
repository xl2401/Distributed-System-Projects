#include<iostream>
#include<string.h>
#include<list>
#include<unordered_map>
using namespace std;
struct CacheEntry{
    string filename;
    char *ptr;
    int filesize;
    CacheEntry(string f, char *p, int s):filename(f),ptr(p),filesize(s){};
};

class LRUCache{
    public:
        int capacity = 0;
        LRUCache(int capacity)
        {
            m_capacity = capacity;
        }
        CacheEntry *get(string filename,int &status)
        {
            if(m_map.find(filename)!=m_map.end())
            {
                Movetohead(filename);
                return m_map[]
            }
        }

    private:
        int m_capacity;
        unordered_map<string,list<CacheEntry*>::iterator> m_map;
        list<CacheEntry*> m_LRU_Cache;
        void Movetohead(string filename)
        {
            CacheEntry *updatedEntry
        } 
}
