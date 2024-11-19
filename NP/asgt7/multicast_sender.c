#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct in_addr localInterface;
struct sockaddr_in sockaddr;
int sd;
int datalen;
char databuf[1024]; 

int main (int argc, char *argv[]){

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("opening datagram socket");
        exit(1);
    }

    memset((char *) &sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr("225.1.1.1");  
    sockaddr.sin_port = htons(5555);  

 
    char loopch = 0;
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopch, sizeof(loopch)) < 0) {
        perror("setting IP_MULTICAST_LOOP:");
        close(sd);
        exit(1);
    }

    localInterface.s_addr = inet_addr("127.0.0.1");  
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0) {
        perror("setting local interface");
        close(sd);
        exit(1);
    }
    printf("Enter message to send : ");
    fgets(databuf, 1024, stdin);
    datalen = strlen(databuf);  
    if (sendto(sd, databuf, datalen, 0, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        perror("sending datagram message");
        close(sd);
        exit(1);
    }

    printf("Multicast message sent successfully.\n");

    close(sd);
    return 0;
}
