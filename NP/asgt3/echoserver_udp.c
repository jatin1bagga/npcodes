#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAXLINE 1024

int main(int argc, char **argv){
	int sockfd, n;
	socklen_t len;
	char msg[1024];
	
	struct sockaddr_in servaddr, cliaddr;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(5035);

	bind(sockfd, (struct sockaddr*)& servaddr, sizeof(servaddr));

	while(1){
		len = sizeof(cliaddr);
		n = recvfrom(sockfd, msg, MAXLINE, 0, (struct sockaddr*)& cliaddr, &len);
		printf("\n Client's message : %s\n", msg);
		if(n<6){
			perror("Error");
		}
		sendto(sockfd, msg, n, 0, (struct sockaddr*)& cliaddr, len);
	}
	close(sockfd);
	return 0;
}
