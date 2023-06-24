// Implement Concurrent Time Server application using UDP to execute the program at remote
// server. Client sends a time request to the server, server sends its system time back to the
// client. Client displays the result.*  Batch 1- 26-04-2023 AN, Submit before  8.00pm today.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5173

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];
    socklen_t len;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <IP address>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "inet_pton error for %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    sendto(sockfd, buffer, strlen(buffer), 0,
           (const struct sockaddr *)&servaddr, sizeof(servaddr));
    len = sizeof(servaddr);
    recvfrom(sockfd, buffer, sizeof(buffer), 0,
             (struct sockaddr *)&servaddr,&len);
    printf("%s", buffer);
    return 0;
}
