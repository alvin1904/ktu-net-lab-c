#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    struct sockaddr_in client, server;
    int s, n;
    char b1[100], b2[100];
    s = socket(AF_INET, SOCK_DGRAM, 0); // SOCK_DGRAM for UDP
    server.sin_family = AF_INET;
    server.sin_port = 2000;
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost
    printf("\nClient ready....\n");
    n = sizeof(server);

    while (1)
    {
        printf("\nClient: ");
        fgets(b2, sizeof(b2), stdin);
        b2[strcspn(b2, "\n")] = '\0'; // Remove trailing newline character
        sendto(s, b2, strlen(b2), 0, (struct sockaddr *)&server, n);
        if (strcmp(b2, "end") == 0)
            break;
        recvfrom(s, b1, sizeof(b1), 0, NULL, NULL);
        printf("\nServer: %s", b1);
    }

    return 0;
}

