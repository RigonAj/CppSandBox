// Client side implementation of UDP client-server model

#include <bits/stdc++.h>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
using namespace std;

const int port = 8080;
const int maxline = 1024;

class client{
    public:
    client(char *buffer);
    void update(string,char*);

    protected:
        struct sockaddr_in servaddr;
        int sockfd;
};

 client::client(char * buffer) {
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;              // IPv4
    servaddr.sin_port   = htons(port);          // Server port
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP

}

void client::update(string send,char * buffer){
    socklen_t len = sizeof(servaddr);

    sendto(sockfd, send.c_str(), send.length(), MSG_CONFIRM,
           (const struct sockaddr *)&servaddr, sizeof(servaddr));

    int n = recvfrom(sockfd, buffer, maxline, MSG_DONTWAIT,
                     (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';   // Null terminate received data
    printf("Server: %s\n", buffer);


    // Close socket
    close(sockfd);

}
