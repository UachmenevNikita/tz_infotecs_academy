#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


void teststringforserver(std::string data)
{
  //std::cout<<data<<std::endl;
    if (data.length() >= 2 && std::stoi(data) % 32 == 0) {
        std::cout << "good, data accept: " << data << std::endl;
        //return true;
    }
    else {
        std::cout << "error data" << std::endl;
        //return false;
    }
}

int main(int argc, char *argv[])
{
 int sock, listener,port;
 if (argc == 2) {
        port = atoi(argv[1]);
    }
    else {
        port = 54321;
    }
 struct sockaddr_in addr,client_addr;
 socklen_t len=sizeof(client_addr);
 char buf[1024];
 int bytes_read;
 listener = socket(AF_INET, SOCK_STREAM, 0);
 if(listener < 0)
 {
 perror("socket");
 return 1;
 }

 addr.sin_family = AF_INET;
 addr.sin_port = htons(port);
 addr.sin_addr.s_addr = htonl(INADDR_ANY);
 if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
 {
 perror("bind");
 return 2;
 }
  printf( "Waiting for a client to connect...\n" );
 listen(listener, 1);

 while(1)
 {
 sock = accept(listener, (sockaddr*)&client_addr,&len );
 //sock = accept(listener, NULL,NULL );
 std::cout << "Client connected: "  <<  std::endl;
 if(sock < 0)
 {
 perror("accept");
 return 3;
 }
 while(1)
 {
 bytes_read = recv(sock, buf, 1024, 0);
 if(bytes_read <= 0) break;
 std::string data=buf;
 //std::cout<<data<<std::endl;
 teststringforserver(data);
 std::string msg="data acept";

 send(sock, msg.c_str(),msg.size(), 0);
 }

 close(sock);
 }
close(listener);
 return 0;
}
