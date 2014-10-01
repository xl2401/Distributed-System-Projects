#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#define FILENAME_MAX_LEN 1024
#define CACHE_CAPACITY 67108864
#include "LRUCache.hpp"
int main(int argc , char *argv[])
{
    if(argc<3)
    {
        perror("please input the right parameters");
        return 1;
    }

    unsigned short port = atoi(argv[1]);
    char *dirpath = argv[2];
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        perror("Could not create socket");
        return 1;
    }
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    //Listen
    listen(socket_desc , 3);
    c = sizeof(struct sockaddr_in);
    LRUCache lrucache(CACHE_CAPACITY, string(dirpath));
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        char filename[FILENAME_MAX_LEN];
        memset(filename, 0, FILENAME_MAX_LEN);
        long recv_size;
        if((recv_size = recv(client_sock, filename, FILENAME_MAX_LEN, 0))<0)
        {
            cout<<"read err"<<endl;
            return 1;
        }
        cout<<"Client "<<inet_ntoa(server.sin_addr)<<" is requesting file "<< filename<<endl;
        int status = -1;
        CacheEntry *entry = lrucache.get(filename,status);
        if(status == 1)
        {
            cout<<"Cache hit File "<<filename<<" sent to the client."<<endl;
        }
        else if(status == 0)
        {
            cout<<"File "<<filename<<" does not exist."<<endl;
        }else if(status == 2)
        {
            cout<<"Cache Miss. File "<<filename<<" sent to client"<<endl;
        }
        if(status == 0 )
        {
            close(client_sock);
            continue;
        }
        int filesize = entry->filesize; 
        if((int)send(client_sock, entry->ptr, filesize,0)!=filesize)
        {
            cout<<"err send file"<<endl;
            continue;
        }
        if(status == 3)
        {
            delete[] entry->ptr;
        }
        close(client_sock);

    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
