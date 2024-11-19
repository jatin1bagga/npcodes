#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct sockaddr_in sockaddr;
struct ip_mreq group;
int sd;
int datalen;
char databuf[1024];

int main (int argc, char *argv[]){
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("opening datagram socket");
        exit(1);
    }

    int reuse = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0) {
        perror("setting SO_REUSEADDR");
        close(sd);
        exit(1);
    }

    memset((char *) &sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(5555);
    sockaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sd, (struct sockaddr*)&sockaddr, sizeof(sockaddr))) {
        perror("binding datagram socket");
        close(sd);
        exit(1);
    }

    group.imr_multiaddr.s_addr = inet_addr("225.1.1.1"); 
    group.imr_interface.s_addr = inet_addr("127.0.0.1");  

    if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0) {
        perror("adding multicast group");
        close(sd);
        exit(1);
    }

    while (1) {
        datalen = sizeof(databuf);
        if (read(sd, databuf, datalen) < 0) {
            perror("reading datagram message");
            close(sd);
            exit(1);
        }
        printf("Received : %s", databuf);
        close(sd);
        exit(1);
    }

    setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&group, sizeof(group));
    close(sd);

    return 0;
}
