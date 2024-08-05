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
#include <cstring>
#include <semaphore.h>
#include <thread>
#include <unistd.h>
#include <mutex>

// Client
sem_t semTwait;
std::mutex gm;
//check string
bool teststringforclient(std::string data){
    if (data.size()>64)
    {
        std::cout << "message size>64" << std::endl;
        return false;
    }
    for (int i = 0; i < data.length(); i++)
    {
        if (!isdigit(data[i])) {
            std::cout << "simbol "<<data[i]<<" is not digit" << std::endl;
            return false;
        }
    }
    std::cout << "the message is correct" << std::endl;
    return true;
}
//transforme string
void chandestring(std::string &data) {
    std::string tmpdata = "";
    for (int i = 0; i < data.length(); i++)
    {
        if (data[i] % 2 == 0) {
            tmpdata += "KB";
            continue;
        }
        tmpdata += data[i];
    }
    data = std::move(tmpdata);
}
//sort string
void stringsort(std::string &data) {
    std::vector<char> tmpdata;
    for (int i = 0; i < data.length(); i++)
    {
        tmpdata.push_back(data[i]);
    }
    std::sort(tmpdata.begin(), tmpdata.end(), std::greater<char>());
    std::string tmpdatastr = "";
    for (int i = 0; i < data.length(); i++)
    {
        tmpdatastr += tmpdata[i];
    }
    data = std::move(tmpdatastr);
}
//sum digits
int sumdig(std::string data) {
    int sum = 0;
    for (int i = 0; i < data.length(); i++)
    {
        if (isdigit(data[i])) {
            int tmpchar = int(data[i]) - 48;
            sum += tmpchar;
        }
    }
    return sum;
}


std::string getline(const char * buf, int i, int & pos) {
    const int tPos = pos;
    for (; pos<i; ++pos) {
        if (buf[pos] == '\n') break;
    }
    ++pos;
    return std::string(buf, tPos, pos - tPos - 1);
}
//first th
void funforth1(std::string &buf) {
    gm.lock();
    std::string data;
    bool tst = false;
    while (tst == false) {
        std::cout << "input data" << std::endl;
        std::getline(std::cin, data);
        tst = teststringforclient(data);
    }
        stringsort(data);
        //std::cout << data<<std::endl;
        chandestring(data);
        //std::cout << data<<std::endl;
    
    buf = data;
    sem_post(&semTwait);
    gm.unlock();
}
//second th
void funforth2(std::string &buf,int m_socket) {
    sem_wait(&semTwait);
    std::string STR;
    std::string data = buf;
    buf = "";
    std::cout <<"converted string: "<< data<<std::endl;
    std::cout <<"sum of odd digits: "<< sumdig(data) << std::endl;
        data=std::to_string(sumdig(data));
        //std::cout <<"end data: "<<data<<std::endl;
        STR=data;
        STR.push_back('\0');
        //std::cout <<"data for send: "<< STR << std::endl;
        int iResult = send(m_socket, STR.c_str(), STR.size(), 0);
        //std::cout <<"iresult "<<iResult<<std::endl;
        
        
}

int main(int argc, char *argv[]) {
    int port = 54321;
    if ( sem_init(&semTwait, 0, 0) != 0 ) {
        std::cerr << "Semaphore init failed" << std::endl;
        return 1;
    }
    

    std::string ip_addr="127.0.0.1";

    if (argc == 3) {
        port = atoi(argv[1]);
        ip_addr = argv[2];

        std::cout << ip_addr << '\n' << port << '\n';
    }
   



    // Create a socket.
    int m_socket;
    m_socket = socket( AF_INET, SOCK_STREAM, 0 );
    if ( m_socket < 0) {
        printf( "Error at socket\n");
        return 2;
    }

    // Connect to a server.
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());
    addr.sin_port = htons( port );
    if (connect(m_socket, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        printf("Failed to connect\n");
        close(m_socket);
        //return 3;
    }

    std::cout << "Connected to server\n";
    std::string data="";
    char buf[512];
    std::string gbuf;
    std::string STR;
    bool tst = false;
    int iResult=1;
    while (true) {
    if(iResult<=0){
    m_socket = socket( AF_INET, SOCK_STREAM, 0 );
    if ( m_socket < 0) {
        printf( "Error at socket\n");
        //return 2;
    }
    if (connect(m_socket, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        printf("Failed to connect\n");
        shutdown(m_socket,SHUT_RDWR);
        close(m_socket);
        sleep(3);
        //return 3;
    }else{
      printf("Connect restored\n");
    }}
        std::thread th1(funforth1, std::ref(gbuf));
        std::thread th2(funforth2, std::ref(gbuf),m_socket);
        th1.detach();
        th2.detach();


        
        iResult = recv(m_socket, buf, 512, 0);
        if (iResult > 0 ) {
            std::cout<< "messege send."<<std::endl;
            
        }
        if (iResult == 0 ) {
            std::cout<< "Connection Closed."<<std::endl;
            
        }
        else if (iResult < 0) {
            std::cout<<"Connection error:"<<std::endl;
           
        }
        
        
    
	}
    
    
    close(m_socket);
    return 0;
}
