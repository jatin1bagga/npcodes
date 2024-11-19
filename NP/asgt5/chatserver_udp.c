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
	struct sockaddr_in servaddr, cliaddr;
	socklen_t len;
	
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_fd < 0){
		perror("Failed!");
		return 1;
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(22000);

	if(bind(sock_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		perror("Failed!");
		return 1;
	}
	
	while(1){
		len = sizeof(cliaddr);
		bzero(msg, SIZE);

		int n = recvfrom(sock_fd, msg, SIZE, 0 , (struct sockaddr *)&cliaddr, &len);
		if(n<0){
			perror("Failed!");
			continue;
		}

		msg[n] = '\0';
		printf("Client : %s", msg);
		
		if(strncmp(msg, "exit", 4) == 0){
			printf("Client has exited the chat.\n");
			break;
		}

		printf("Server : ");
		fgets(msg, SIZE, stdin);

		sendto(sock_fd, msg, strlen(msg), 0, (const struct sockaddr*)&cliaddr, len);
		
		if(strncmp(msg, "exit", 4) == 0){
			printf("Server is exiting the chat.\n");
			break;
		}
	}
	
	close(sock_fd);
	return 0;
}

