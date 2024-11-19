#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char** argv){
	int listen_fd;
	char str[100];
	struct sockaddr_in servaddr;
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));

    	servaddr.sin_family = AF_INET;
    	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    	servaddr.sin_port = htons(22000);

	connect(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));  	

	while(1){
	        printf("Client: ");
	        fgets(str, 100, stdin);
	        send(listen_fd, str, strlen(str), 0);

	        if (strncmp(str, "exit", 4) == 0) {
	            printf("Exiting chat.\n");
	            break;
	        }

	        bzero(str, 100);
	        recv(listen_fd, str, 100, 0);
	        printf("Server: %s", str);

	        if (strncmp(str, "exit", 4) == 0) {
	            printf("Server has exited the chat.\n");
	            break;
	        }
	}
	close(listen_fd);
	return 0;
}
