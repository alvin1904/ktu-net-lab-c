```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
```

Here, the necessary header files are included. These files provide definitions and declarations for functions and data structures used in the code.

```c
#define PORT 5173
```

A symbolic constant `PORT` is defined with the value 5173. This constant will be used as the port number for communication.

```c
int main(int argc, char **argv) {
```

The program's main function is defined, which takes command-line arguments `argc` (argument count) and `argv` (argument vector).

```c
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];
    socklen_t len;
```

Several variables are declared: `sockfd` for the socket file descriptor, `servaddr` to hold server address information, `buffer` to store data being sent and received, and `len` to store the length of the socket address structure.

```c
    if (argc != 2) {
        fprintf(stderr, "usage: %s <IP address>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
```

This block checks if the program was provided with exactly two command-line arguments: the program name and an IP address. If not, it prints an error message and exits the program with a failure status.

```c
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
```

Here, a UDP socket is created using the `socket()` function. If the creation fails, an error message is printed using `perror()` and the program exits with a failure status.

```c
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "inet_pton error for %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
```

This block sets up the server address structure `servaddr` with the specified IP address and port number. It uses `inet_pton()` to convert the provided IP address from text format to binary format. If the conversion fails, an error message is printed, and the program exits with a failure status.

```c
    sendto(sockfd, buffer, strlen(buffer), 0,
           (const struct sockaddr *)&servaddr, sizeof(servaddr));
```

The `sendto()` function is used to send data over the socket to the specified server address. It sends the data in the `buffer` with a length of `strlen(buffer)` bytes.

```c
    len = sizeof(servaddr);
    recvfrom(sockfd, buffer, sizeof(buffer), 0,
             (struct sockaddr *)&servaddr, &len);
```

The `recvfrom()` function is used to receive data from the socket. It stores the received data in the `buffer` and also updates the server address information in the `servaddr` structure. The received data has a maximum size of `sizeof(buffer)` bytes.

```c
    printf("%s", buffer);
    return 0;
}
```

Finally, the received data stored in the `buffer` is printed to the console. The program returns 0 to indicate successful execution.

Overall, this code creates a UDP socket, sends data to a specified server IP address and port, receives a response, and then prints the response to the console.