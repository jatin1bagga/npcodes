#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    char buffer[100];
    int listen_fd, comm_fd;
    struct sockaddr_in servaddr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(22000);

    bind(listen_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listen_fd, 10);

    while(1){
        comm_fd = accept(listen_fd, (struct sockaddr *)NULL, NULL);

        bzero(buffer, 100);
        int n = recv(comm_fd, buffer, 100, 0);
        if(n > 0){
            printf("Received from client: %s\n", buffer);
            send(comm_fd, buffer, n, 0);
        }
	
        close(comm_fd);
        printf("Client disconnected.\n");
	exit(1);
    }

    close(listen_fd);
    return 0;
}
