#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <strings.h> // bzero()
#include <arpa/inet.h> // inet_addr()
#define MAX 80
#define PORT 8086
#define SA struct sockaddr
void func(int sockfd)
{
    char buff[MAX], sample[MAX];
    int n, len, i, flag = 0;
    for (;;)
    {
        bzero(buff, sizeof(buff)); // Clear buffer
        printf("\nEnter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff)); // Send buffer to server
        bzero(buff, sizeof(buff)); 
        read(sockfd, buff, sizeof(buff)); // Read response from server
        printf("\nFrom Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) // Exit if "exit" is received
        {
            printf("Client Exit...\n");
            break;
        }
        strcpy(sample, buff); // Copy buffer to sample
        len = strlen(sample) - 1;
        for (i = 0; i < len / 2; i++) // Check if string is palindrome
            if (sample[i] != sample[len - i - 1])
            {
                printf("\nString is not palindrome\n");
                flag = 1;
                break;
            }
        if (!flag)
            printf("\nString is a palindrome\n");
    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli; // servaddr will contain server address
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM for TCP
    if (sockfd == -1) // socket() returns -1 on failure
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost
    servaddr.sin_port = htons(PORT); // Port 
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0) 
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
    func(sockfd); // Send and receive messages
    close(sockfd); // Close socket
}
