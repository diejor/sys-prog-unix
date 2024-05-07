#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/select.h>
#include <arpa/inet.h>

#define MAX 1024
#define PORT 8080

void handle_tcp(int sockfd) {
    char buff[MAX];
    int n;
    struct sockaddr_in cli;
    socklen_t clilen = sizeof(cli);

    int connfd = accept(sockfd, (struct sockaddr*)&cli, &clilen);
    if (connfd < 0) {
        std::cerr << "Server: accept failed" << std::endl;
        return;
    }

    std::cout << "TCP connection established with client: " << inet_ntoa(cli.sin_addr) << ":" << ntohs(cli.sin_port) << std::endl;

    while ((n = recv(connfd, buff, sizeof(buff), 0)) > 0) {
        std::cout << "Received message: " << buff << std::endl;
        send(connfd, buff, n, 0);  // Echo back the received message
        std::cout << "Echoed back message to TCP client" << std::endl;
    }

    std::cout << "TCP client disconnected" << std::endl;
    close(connfd);
}

void handle_udp(int sockfd) {
    char buff[MAX];
    int n;
    struct sockaddr_in cli;
    socklen_t clilen = sizeof(cli);

    n = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&cli, &clilen);
    if (n > 0) {
        std::cout << "UDP datagram received from " << inet_ntoa(cli.sin_addr) << ":" << ntohs(cli.sin_port) << std::endl;
        std::cout << "Received message: " << buff << std::endl;
        sendto(sockfd, buff, n, 0, (struct sockaddr*)&cli, clilen);  // Echo back the received message
        std::cout << "Echoed back message to UDP client" << std::endl;
    }
}

int main() {
    int tcpfd, udpfd, nready, maxfdp1;
    struct sockaddr_in servaddr;
    fd_set rset;

    // Create TCP socket
    tcpfd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpfd == -1) {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    } else {
        std::cout << "TCP socket created successfully" << std::endl;
    }

    // Create UDP socket
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpfd == -1) {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    } else {
        std::cout << "UDP socket created successfully" << std::endl;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Bind TCP socket
    if (bind(tcpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("TCP bind failed");
        exit(EXIT_FAILURE);
    } else {
        std::cout << "TCP socket bound to port " << PORT << std::endl;
        listen(tcpfd, 5);
        std::cout << "TCP socket listening" << std::endl;
    }

    // Bind UDP socket
    if (bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("UDP bind failed");
        exit(EXIT_FAILURE);
    } else {
        std::cout << "UDP socket bound to port " << PORT << std::endl;
    }

    FD_ZERO(&rset);
    maxfdp1 = (tcpfd > udpfd ? tcpfd : udpfd) + 1;

    while (1) {
        FD_SET(tcpfd, &rset);
        FD_SET(udpfd, &rset);

        nready = select(maxfdp1, &rset, NULL, NULL, NULL);

        if (nready < 0) {
            perror("Select error");
            continue;
        }

        if (FD_ISSET(tcpfd, &rset)) {
            handle_tcp(tcpfd);
        }

        if (FD_ISSET(udpfd, &rset)) {
            handle_udp(udpfd);
        }
    }

    return 0;
}

