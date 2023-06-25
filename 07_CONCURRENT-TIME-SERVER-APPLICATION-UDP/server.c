#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#define PORT 5173

int main() {
    int sockfd;
    struct sockaddr_in server, client;
    char buffer[1024];
    time_t ticks;
    socklen_t len;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    perror("socket creation success");
    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    while (1) {
        len = sizeof(client);
        recvfrom(sockfd, buffer, sizeof(buffer), 0,
                 (struct sockaddr *)&client, &len);
        ticks = time(NULL);
        snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks));
        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&client, len);
    }
    return 0;
}
