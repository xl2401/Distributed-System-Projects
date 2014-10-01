#include<iostream>
#include<list>
#include<unordered_map>
#include<fstream>
#include "LRUCache.hpp"


char *LRUCache::read_in_memory(string filename,int &size)
{
    filename = root+"/"+filename;
    size = -1;
    char *buffer = NULL;
    std::ifstream is(filename, std::ifstream::binary);
    if(is)
    {
        is.seekg(0,is.end);
        size = is.tellg();
        is.seekg(0,is.beg);
        buffer = new char[size];
        is.read(buffer,size);
    }
    is.close();
    return buffer;
}

CacheEntry *LRUCache::get(string filename, int &status)
{
    //If we can find the file in the cache
    if(cache_map.find(filename)!=cache_map.end())
    {
        status = 1;//HIT
        movetoHead(filename);
        return *cache_map[filename];
    }
    else
    {
        //else we go to the directory and read into memory
        int size = 0;
        char *buffer = read_in_memory(filename,size);
        //if there is no file there
        if(buffer==NULL||size==-1)
        {
            status = 0;//NONEXIST
            return NULL;
        }
        else
        {

            CacheEntry *newEntry = new CacheEntry(filename,buffer,size);
            //if the size of file is too large
            if(size>m_capacity)
            {
                status = 3;//TOO BIG 
            }
            else
            {
                //if it can fit in the cache.
                status = 2;//MISS
                //make sure there is enough free size for the newItem.
                while(size>m_capacity-used_capacity)
                {
                    CacheEntry *todelete = m_LRU_Cache.back();
                    used_capacity -= todelete->filesize;
                    delete[] todelete->ptr;
                    cache_map.erase(todelete->filename);
                    m_LRU_Cache.pop_back();
                    delete todelete;
                }
                m_LRU_Cache.push_front(newEntry);
                cache_map[filename] = m_LRU_Cache.begin();
            }
            return newEntry;
        }
    }
}
