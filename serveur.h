#include <bits/stdc++.h>
#include <cstdint>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;
#define PORT     8080
#define MAXLINE 1024




class Serveur{
    public:

    int receive();
    void init(const char *,uint16_t);
    void send(uint8_t*, size_t);
    inline const uint8_t* GetData()const{return buffer;}

    protected:
        struct sockaddr_in servaddr;
        struct sockaddr_in cliaddr;
        int sockfd;
        bool client_connected = false;
        uint8_t buffer[1024];
};


void Serveur::init(const char *ip,uint16_t port){

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);


    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

int Serveur::receive() {
    socklen_t len = sizeof(cliaddr);

    int n = recvfrom(sockfd, buffer, MAXLINE-1 , MSG_DONTWAIT, (struct sockaddr *)&cliaddr, &len);

    if (n > 0) {
        client_connected = true;
    }
    return n;
}

void Serveur::send(uint8_t* send_,size_t size){

    if (client_connected) {
        int n = sendto(sockfd, send_, size, MSG_CONFIRM,
               (const struct sockaddr *)&cliaddr, sizeof(cliaddr));
    }
}
