#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Function designed to echo messages back to the client.
void echo_func(int connfd)
{
    char buff[MAX];
    int n;
    // Infinite loop for handling client communication
    while (1) {
        bzero(buff, MAX);

        // Read the message from client and copy it into the buffer
        n = recv(connfd, buff, sizeof(buff), 0);
        if (n <= 0) {
            printf("Read error or connection closed by client\n");
            break; // Break the loop if client closes the connection or error occurs
        }
        
        // Print buffer which contains the client contents
        printf("From client: %s", buff);

        // Send that buffer back to client
        send(connfd, buff, n, 0);
    }
}

// Driver function
int main()
{
    int sockfd, connfd;
    unsigned int len;
    struct sockaddr_in servaddr, cli;

    // Socket creation and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("Socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Server is ready to listen
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client
    while (1) {
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0) {
            printf("Server accept failed...\n");
            continue; // Continue to accept next connection
        }
        else
            printf("Server accepted the client..\n");

        // Function for echoing back to client
        echo_func(connfd);

        // Close the client socket after echoing
        close(connfd);
    }

    // After chatting close the server socket
    close(sockfd);
}
