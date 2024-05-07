#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <signal.h>

#define MAX 1024

using namespace std;

void handle_tcp(int sockfd) {
    char buff[MAX];
    int n;
    struct sockaddr_in cli;
    socklen_t clilen = sizeof(cli);

    int connfd = accept(sockfd, (struct sockaddr*)&cli, &clilen);
    if (connfd < 0) {
        cerr << "Server: accept failed" << endl;
        return;
    }

    pid_t pid = fork();
    if (pid == 0) { // Child process
        close(sockfd); // Close the listening socket in the child process
        cout << "TCP connection established with client: " << inet_ntoa(cli.sin_addr) << ":" << ntohs(cli.sin_port) << endl;

        while ((n = recv(connfd, buff, sizeof(buff), 0)) > 0) {
            cout << "Received message: " << buff << endl;
            send(connfd, buff, n, 0);  // Echo back the received message
        }

        cout << "TCP client disconnected" << endl;
        close(connfd);
        exit(0);  // Terminate the child process
    }
    close(connfd); // Close the connected socket in the parent process
}

void handle_udp(int sockfd) {
    char buff[MAX];
    int n;
    struct sockaddr_in cli;
    socklen_t clilen = sizeof(cli);

    n = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&cli, &clilen);
    if (n > 0) {
        pid_t pid = fork();
        if (pid == 0) {  // Child process
            cout << "UDP datagram received from " << inet_ntoa(cli.sin_addr) << ":" << ntohs(cli.sin_port) << endl;
            cout << "Received message: " << buff << endl;
            sendto(sockfd, buff, n, 0, (struct sockaddr*)&cli, clilen);  // Echo back the received message
            cout << "Echoed back message to UDP client" << endl;
            exit(0); // Terminate the child process
        }
    }
}

int create_socket(int port, bool is_tcp) {
    int sockfd = socket(AF_INET, is_tcp ? SOCK_STREAM : SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (is_tcp) {
        if (listen(sockfd, 5) < 0) {
            perror("Listen failed");
            exit(EXIT_FAILURE);
        }
    }

    return sockfd;
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 4) {
        cerr << "Usage: " << argv[0] << " <port1> [<port2> <port3>]" << endl;
        exit(EXIT_FAILURE);
    }

    vector<int> ports;
    for (int i = 1; i < argc; ++i) {
        ports.push_back(stoi(argv[i]));
    }

    vector<int> tcpfds, udpfds;
    fd_set rset;
    int maxfdp1 = 0;

    for (int port : ports) {
        int tcpfd = create_socket(port, true);
        int udpfd = create_socket(port, false);
        tcpfds.push_back(tcpfd);
        udpfds.push_back(udpfd);
        maxfdp1 = max({maxfdp1, tcpfd, udpfd});
    }
    maxfdp1 += 1;

    signal(SIGCHLD, SIG_IGN); // Ignore SIGCHLD to prevent zombie processes

    while (true) {
        FD_ZERO(&rset);
        for (int tcpfd : tcpfds) {
            FD_SET(tcpfd, &rset);
        }
        for (int udpfd : udpfds) {
            FD_SET(udpfd, &rset);
        }

        int nready = select(maxfdp1, &rset, NULL, NULL, NULL);
        if (nready < 0) {
            perror("Select error");
            continue;
        }

        for (int tcpfd : tcpfds) {
            if (FD_ISSET(tcpfd, &rset)) {
                handle_tcp(tcpfd);
            }
        }
        for (int udpfd : udpfds) {
            if (FD_ISSET(udpfd, &rset)) {
                handle_udp(udpfd);
            }
        }
    }

    return 0;
}

