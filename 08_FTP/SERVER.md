This code is an implementation of a simple server program in C that listens for incoming connections, receives data from a client, and writes the received data into a file. Let's break down the code line by line to understand its functionality:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SIZE 1024
```
These lines include necessary header files required for the program. These headers provide functions and definitions needed for socket programming, file operations, and memory manipulation.

```c
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
```
This function, `write_file`, receives a file pointer (`FILE *fp`) and a socket descriptor (`int sockfd`). It continuously receives data from the socket (`sockfd`) and writes it to the file (`fp`). It uses a buffer (`buffer`) of size `SIZE` to temporarily hold the received data. The `recv` function reads data from the socket, and the received data is written into the file using the `fwrite` function. After each write, the buffer is cleared using `memset` to prepare it for the next data reception.

```c
int main()
{
    const char *ip = "127.0.0.1";
    const int port = 8080;

    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];
```
The `main` function starts here. It defines the IP address (`ip`) and port number (`port`) for the server. It also declares variables for socket descriptors (`sockfd` and `new_sock`), socket addresses (`server_addr` and `new_addr`), and an address size variable (`addr_size`). A buffer (`buffer`) is also defined to hold data temporarily.

```c
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");
```
A socket is created using the `socket` function. The socket type is set to `SOCK_STREAM` (indicating a TCP socket), and the address family is set to `AF_INET` (IPv4). If the socket creation fails, an error message is printed, and the program exits. Otherwise, a success message is printed.

```c
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);
```
The server's address information is configured. The address family, port number (converted to network byte order using `htons`), and IP address (converted to binary format using `inet_addr`) are set in the `server_addr` structure.

```c
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("[-]Error in bind");
        exit(1);
    }
    printf("[+]Binding successful.\n");
```
The socket is bound to the server's address using the `bind` function. If the binding fails, an error message is printed, and the program exits. If successful, a message indicating successful binding is printed.

```c
    if (listen(sockfd, 10) == 0)
    {
        printf("[+]Listening....\n");
    }
    else
    {
        perror("[-]Error in listening");
        exit(1);
    }
```
The socket starts listening for incoming connections using the `listen` function. If the listening operation is successful, a message is printed. If not, an error message is printed, and the program exits.

```c
    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);
    if (new_sock < 0)
    {
        perror("[-]Error in accepting");
        exit(1);
    }
```
The program waits for an incoming connection using the `accept` function. When a connection is established, a new socket (`new_sock`) is created for communication with the connected client. If the `accept` operation fails, an error message is printed, and the program exits.

```c
    FILE *fp = fopen("hello_received.txt", "w");
    if (fp == NULL)
    {
        perror("[-]Error opening file");
        exit(1);
    }
```
A file named "hello_received.txt" is opened in write mode using the `fopen` function. If the file opening fails, an error message is printed, and the program exits.

```c
    write_file(fp, new_sock);
    printf("[+]Data written to the file successfully.\n");

    fclose(fp);
    close(new_sock);
    close(sockfd);

    return 0;
}
```
The `write_file` function is called with the file pointer (`fp`) and the new socket (`new_sock`) as arguments. This function receives data from the client via the socket and writes it to the file.

After data has been written to the file, the file is closed using `fclose`. Both the new socket (`new_sock`) and the server socket (`sockfd`) are closed using the `close` function.

The `main` function returns 0, indicating successful execution of the program.

In summary, this code sets up a basic server that listens for incoming connections, receives data from clients, and writes the received data to a file named "hello_received.txt".