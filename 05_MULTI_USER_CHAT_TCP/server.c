#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>

#define PORT 4950
#define BUFSIZE 1024

int client = 0;

void send_to_all(int j, int i, int sockfd, int nbytes_recvd, char *recv_buf, fd_set *master)
{
    if (FD_ISSET(j, master))
    {
        if (j != sockfd && j != i)
        {
            if (send(j, recv_buf, nbytes_recvd, 0) == -1)
            {
                perror("send");
            }
        }
    }
}

void send_recv(int i, fd_set *master, int sockfd, int fdmax)
{
    int nbytes_recvd, j; // nbytes_recvd: number of bytes received
    char recv_buf[BUFSIZE], buf[BUFSIZE]; 

    if ((nbytes_recvd = recv(i, recv_buf, BUFSIZE, 0)) <= 0) // recv() returns number of bytes received
    {
        if (nbytes_recvd == 0) // If client hung up
        {
            printf("socket %d hung up\n", i);
            client--;
        }
        else
        {
            perror("recv");
        }
        close(i);
        FD_CLR(i, master); // Remove from master set
    }
    else
    {
        //	printf("%s\n", recv_buf);
        for (j = 0; j <= fdmax; j++)
        {
            send_to_all(j, i, sockfd, nbytes_recvd, recv_buf, master);  // send to everyone!
        }
    }
}

void connection_accept(fd_set *master, int *fdmax, int sockfd, struct sockaddr_in *client_addr) // Accept new connection
{
    socklen_t addrlen;
    int newsockfd;

    addrlen = sizeof(struct sockaddr_in); // Length of client address
    if ((newsockfd = accept(sockfd, (struct sockaddr *)client_addr, &addrlen)) == -1) // Accept new connection
    {
        perror("accept");
        exit(1);
    }
    else
    {
        FD_SET(newsockfd, master);  // Add to master set
        if (newsockfd > *fdmax) // Keep track of the max
        {
            *fdmax = newsockfd; // fdmax: maximum file descriptor number
            client++; // Increment number of clients
        }
        printf("new connection from %s on port %d \n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
    }
}

void connect_request(int *sockfd, struct sockaddr_in *my_addr)
{
    int yes = 1;

    if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket");
        exit(1);
    }

    my_addr->sin_family = AF_INET;
    my_addr->sin_port = htons(4950);
    my_addr->sin_addr.s_addr = INADDR_ANY;
    memset(my_addr->sin_zero, '\0', sizeof my_addr->sin_zero);

    if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) // Allow reuse of port
    {
        perror("setsockopt");
        exit(1);
    }

    if (bind(*sockfd, (struct sockaddr *)my_addr, sizeof(struct sockaddr)) == -1) // Bind to port
    {
        perror("Unable to bind");
        exit(1);
    }
    if (listen(*sockfd, 10) == -1) // Listen on port
    {
        perror("listen");
        exit(1);
    }
    printf("\nTCPServer Waiting for client on port 4950\n"); 
    fflush(stdout); // Flush stdout buffer
}
int main()
{
    fd_set master; // master file descriptor list
    fd_set read_fds; // temp file descriptor list for select()
    int fdmax, i;
    int sockfd = 0;
    struct sockaddr_in my_addr, client_addr;

    FD_ZERO(&master); // Clear master and temp sets
    FD_ZERO(&read_fds); // Clear master and temp sets
    connect_request(&sockfd, &my_addr); 
    FD_SET(sockfd, &master); // Add listener to master set

    fdmax = sockfd; // Keep track of the biggest file descriptor
    while (1)
    {
        read_fds = master; // Copy it
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }

        for (i = 0; i <= fdmax; i++) // Run through the existing connections looking for data to read
        {
            if (FD_ISSET(i, &read_fds)) // Is it in the master set?
            {
                if (i == sockfd) // New connection
                    connection_accept(&master, &fdmax, sockfd, &client_addr); // Accept new connection
                else
                    send_recv(i, &master, sockfd, fdmax); // Handle data from a client
                if (client == 0)
                    goto endserver; // If no clients left, quit
            }
        }
    }
endserver: // Close all sockets and exit
    printf("Server-quitted\n");
    close(sockfd);
    return 0;
}
