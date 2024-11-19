#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SIZE 100

int main(int argc, char** argv){
	char msg[SIZE];
	int sock_fd;
	struct sockaddr_in servaddr;
	
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_fd < 0){
		perror("Failed!");
		return 1;
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(22000);

	while(1){
		printf("Client: ");
	        fgets(msg, SIZE, stdin);
        	sendto(sock_fd, msg, strlen(msg), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));

		if(strncmp(msg, "exit", 4) == 0){
			printf("Exiting the chat\n");
			break;
		}

		bzero(msg,SIZE);
		int n = recvfrom(sock_fd, msg, SIZE, 0, NULL, NULL);
		if(n<0){
			perror("Failed!");
			continue;
		}

		msg[n] = '\0';
		printf("Server : %s", msg);
		if(strncmp(msg, "exit", 4) == 0){
			printf("Server has exited the chat.\n");
			break;
		}
	}
	
	close(sock_fd);
	return 0;
}

