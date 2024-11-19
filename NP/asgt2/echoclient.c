#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
    int sockfd;
    char sendline[100];
    char recvline[100];
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(22000);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("Connected to server.\n Enter your message:\n");

    bzero(sendline, 100);
    fgets(sendline, 100, stdin);

    sendline[strcspn(sendline, "\n")] = 0;

    send(sockfd, sendline, strlen(sendline), 0);

    bzero(recvline, 100);
    recv(sockfd, recvline, 100, 0);

    printf("Server's echo: %s\n", recvline);

    close(sockfd);
    printf("Connection closed. Exiting...\n");

    return 0;
}
