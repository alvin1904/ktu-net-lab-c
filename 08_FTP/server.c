#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SIZE 1024

void write_file(FILE *fp, int sockfd)
{
    int n;
    char buffer[SIZE];

    while ((n = recv(sockfd, buffer, SIZE, 0)) > 0)
    {
        fwrite(buffer, sizeof(char), n, fp);
        memset(buffer, 0, SIZE);
    }
}

int main()
{
    const char *ip = "127.0.0.1";
    const int port = 8080;

    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("[-]Error in bind");
        exit(1);
    }
    printf("[+]Binding successful.\n");

    if (listen(sockfd, 10) == 0)
    {
        printf("[+]Listening....\n");
    }
    else
    {
        perror("[-]Error in listening");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);
    if (new_sock < 0)
    {
        perror("[-]Error in accepting");
        exit(1);
    }

    FILE *fp = fopen("hello_received.txt", "w");
    if (fp == NULL)
    {
        perror("[-]Error opening file");
        exit(1);
    }

    write_file(fp, new_sock);
    printf("[+]Data written to the file successfully.\n");

    fclose(fp);
    close(new_sock);
    close(sockfd);

    return 0;
}
