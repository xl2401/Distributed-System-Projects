#include<iostream>
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<fstream>
#include <unistd.h>
#define BUFF_SIZE 2048

using namespace std;
//TODO display helpler
int main(int argc, char *argv[])
{
    if(argc!=5)
    {
        puts("input the right request");
        return 1;
    }
    char *ip_addr = argv[1];
    char *port = argv[2];
    char *filename = argv[3];
    char *dirpath = argv[4];
    int sock;
    struct sockaddr_in server;

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
        return 1;
    }
     
    server.sin_addr.s_addr = inet_addr(ip_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons( atoi(port));
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    
    if (send(sock, filename, strlen(filename), 0) < 0)
    {
        cerr << "Send failure" << endl;
        return 1;
    }
    char buff[BUFF_SIZE];
    ssize_t recv_cnt;
    ssize_t total_cnt = 0;
    string sep = "/";
    ofstream outputfile(dirpath+sep+filename,ofstream::binary );
    while ((recv_cnt = recv(sock, buff, BUFF_SIZE, 0)) !=0)
    {
        if (recv_cnt < 0) {
            cerr << "socket err" << endl;
            return 1;
        }
        total_cnt += recv_cnt;
        outputfile.write(buff, recv_cnt);
    }  
    if (total_cnt == 0) {
        cout << "File " << filename << " does not exist" << endl;
    }else
    {
        cout<<"file "<<filename<<" saved  "<<endl;
    }
    outputfile.close();
    close(sock);
    return 0;
}
 
