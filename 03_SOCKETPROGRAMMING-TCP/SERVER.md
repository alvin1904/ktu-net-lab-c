This code is an example of a simple client-server communication program using sockets in C. It demonstrates a basic chat interaction between a server and a client over a TCP connection. I'll explain the code line by line:

```c
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <sys/types.h>
#include <netinet/in.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
```
Here, several header files are included to provide the necessary functions and data types for socket programming. The headers define functions and structures related to networking, file operations, and basic system operations. `MAX` is defined as the maximum buffer size for messages, and `PORT` is set to the port number on which the server will listen for incoming connections. `SA` is a shorthand for `struct sockaddr`, which is used in socket function calls.

```c
void func(int connfd)
{
    char buff[MAX];
    int n;
    for (;;)
    {
        bzero(buff, MAX);
        read(connfd, buff, sizeof(buff));
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(connfd, buff, sizeof(buff));
        if (strncmp("exit", buff, 4) == 0)
        {
            printf("Server Exit...\n");
            break;
        }
    }
}
```
This function, `func`, is responsible for handling the communication between the server and a connected client. It uses the `connfd` socket descriptor to send and receive data. It repeatedly receives messages from the client using the `read` function, prints the received message, reads a message from the server's console using `getchar`, sends the server's response using the `write` function, and exits if the server's message is "exit".

```c
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
```
This is the main function where the server's functionality is implemented. It first creates a socket using the `socket` function. It specifies the address family (`AF_INET` for IPv4), socket type (`SOCK_STREAM` for TCP), and protocol (0 for default protocol) as arguments.

If the socket creation fails, an error message is printed, and the program exits. If successful, a success message is printed.

```c
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
```
Here, the `servaddr` structure is initialized with server information. The IP address is set to `INADDR_ANY`, indicating that the server can accept connections from any available network interface. The port number is set to `PORT`.

The `bind` function associates the socket with the server's address and port. If binding fails, an error message is printed, and the program exits.

```c
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
```
The `listen` function puts the server socket in listening mode, allowing it to accept incoming connections from clients. The second argument specifies the maximum number of queued connections. If the listen operation fails, an error message is printed, and the program exits.

```c
    len = sizeof(cli);

    connfd = accept(sockfd, (SA *)&cli, &len);
    if (connfd < 0)
    {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");
```
The `accept` function blocks until a client connection request arrives. It returns a new socket descriptor, `connfd`, that is used for communication with the client. The `cli` structure holds the client's address information. If the `accept` operation fails, an error message is printed, and the program exits.

```c
    func(connfd);
    close(sockfd);
}
```
The `func` function is called to handle the communication with the client. Once the communication is finished, the server closes the `connfd` socket using the `close` function and terminates.

In summary, this code sets up a server that listens for incoming connections from clients on a specified port. When a client connects, the server enters a chat loop with the client, exchanging messages until the chat is ended by sending or receiving the "exit" message.