#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h> 
	
#define BUFSIZE 1024
		
void send_recv(int i, int sockfd)
{
	char send_buf[BUFSIZE];
	char recv_buf[BUFSIZE];
	int nbyte_recvd;
	
	if (i == 0){
		fgets(send_buf, BUFSIZE, stdin);
		if (strcmp(send_buf , "quit\n") == 0) {
			exit(0);
		}else
			send(sockfd, send_buf, strlen(send_buf), 0);
	}else {
		nbyte_recvd = recv(sockfd, recv_buf, BUFSIZE, 0);
		recv_buf[nbyte_recvd] = '\0'; // Add null-terminator
		printf("%s\n" , recv_buf); 
		fflush(stdout); // Flush stdout buffer
	}
}
		
		
void connect_request(int *sockfd, struct sockaddr_in *server_addr)
{
	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}
	server_addr->sin_family = AF_INET; // host byte order
	server_addr->sin_port = htons(4950); // htons: host to network short
	server_addr->sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost
	memset(server_addr->sin_zero, '\0', sizeof server_addr->sin_zero); // Zero the rest of the struct
	
	if(connect(*sockfd, (struct sockaddr *)server_addr, sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
}
	
int main()
{
	int sockfd, fdmax, i;
	struct sockaddr_in server_addr;
	fd_set master; // master file descriptor list
	fd_set read_fds; // temp file descriptor list for select()
	
	connect_request(&sockfd, &server_addr);
	FD_ZERO(&master); // Clear master and temp sets
        FD_ZERO(&read_fds);
        FD_SET(0, &master);
        FD_SET(sockfd, &master);
	fdmax = sockfd; // Keep track of the biggest file descriptor
	
	while(1){
		read_fds = master; // Copy it
		if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){ // Select() blocks until an I/O event occurs
			perror("select");
			exit(4);
		}
		
		for(i=0; i <= fdmax; i++ ) // Run through the existing connections looking for data to read
			if(FD_ISSET(i, &read_fds)) // Is it set in read_fds?
				send_recv(i, sockfd); // Handle data from a client
	}
	printf("client-quited\n");
	close(sockfd);
	return 0;
}
