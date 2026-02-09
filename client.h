// Client side implementation of UDP client-server model

#include <bits/stdc++.h>
#include <cstddef>
#include <cstring>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
using namespace std;


const int maxline = 1024;

class client{
    public:


    void receive(uint8_t*);
    void client_init(const char *,uint16_t);
    void send(const char*, size_t);

    protected:
        struct sockaddr_in servaddr;
        int sockfd;
        char last_sent[1024] = "";
};

 void client::client_init(const char* ip,uint16_t port) {
    //buffer[maxline + 1]='\0';
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;              // IPv4
    servaddr.sin_port   = htons(port);          // Server port
    servaddr.sin_addr.s_addr = inet_addr(ip); // Server IP

}

void client::receive(uint8_t * buffer) {

    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);

    int n = recvfrom(sockfd, buffer, maxline - 1, MSG_DONTWAIT,
                     (struct sockaddr *)&from_addr, &from_len);

    if (n > 0) {
        buffer[n] = '\0';
        cout << "Client a reçu : " << buffer << endl;
    }

}

void client::send(const char* send_,size_t size){

    if (strcmp(last_sent , send_) ) {
        int sent_status = sendto(sockfd, send_, size, 0,
                                (const struct sockaddr *)&servaddr, sizeof(servaddr));
        if (sent_status >= 0) {
            strcpy(last_sent, send_);
        }
    }

}
