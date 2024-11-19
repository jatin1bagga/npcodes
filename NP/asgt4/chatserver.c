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
	int listen_fd, comm_fd;
	char str[100];
	struct sockaddr_in servaddr;
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
    	servaddr.sin_family = AF_INET;
    	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    	servaddr.sin_port = htons(22000);

    	bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    	listen(listen_fd, 10);
	while(1){
	        comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
	        printf("Client connected.\n");

	        while(1){
		            bzero(str, 100);
		            recv(comm_fd, str, 100, 0);

		            printf("Client: %s", str);
		            if (strncmp(str, "exit", 4) == 0) {
		                printf("Client has exited the chat.\n");
		                break;
		            }
		            printf("Server: ");
		            fgets(str, 100, stdin);
		            send(comm_fd, str, strlen(str), 0);

		            if (strncmp(str, "exit", 4) == 0) {
		                printf("Exiting chat.\n");
		                break;
		            }
	        }

	        close(comm_fd);
	        printf("Connection closed.\n");
	}

    	close(listen_fd);
    	return 0;
}




