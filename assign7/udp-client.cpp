// udp client driver program
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>

#define PORT 5000
#define MAXLINE 1000

// Driver code
int main()
{
	char buffer[100];
	char *message = (char *) "Hello Server";
	int sockfd, n;
	unsigned int len;
	struct sockaddr_in servaddr;
	
	// clear servaddr
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);
	servaddr.sin_family = AF_INET;
	
	// create datagram socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	// if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	// {
	//	printf("\n Error : Connect Failed \n");
	//	exit(0);
	// }

	// request to send datagram
	// no need to specify server address in sendto
	// connect stores the peers IP and port
	sendto(sockfd, message, MAXLINE, 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
	
	// waiting for response
	n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&servaddr, &len);
        
	buffer[n] = '\0';
        printf("Server : %s\n", buffer);
	// close the descriptor
	close(sockfd);
}

