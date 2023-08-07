#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <strings.h> // bzero()
#include <arpa/inet.h> // inet_addr()
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
// Function designed for chat between client and server.
void func(int sockfd) {
    char buff[MAX];
    for (;;) {
        printf("Enter the string : ");
        fgets(buff, sizeof(buff), stdin); 
        write(sockfd, buff, strlen(buff));
        memset(buff, 0, sizeof(buff));
        ssize_t bytesRead = read(sockfd, buff, sizeof(buff));
        if (bytesRead <= 0) {
            printf("Server disconnected...\n");
            break;
        }
        printf("From Server : %s", buff);
        if (strncmp(buff, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main()
{
    int sockfd, connfd; // socket file descriptor
    struct sockaddr_in servaddr, cli; // server and client address

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
}
