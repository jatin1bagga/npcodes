#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BROADCAST_IP "127.0.0.255"  // Broadcast IP for the network segment
#define PORT 22000                    // Port number for broadcasting
#define MESSAGE "Broadcast message"    // Message to broadcast

int main() {
    int sockfd;
    struct sockaddr_in broadcastAddr;
    int broadcastPermission = 1;  // Allow broadcast

    // Create socket for sending datagrams
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        exit(1);
    }

    // Set socket options to allow broadcast
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastPermission, sizeof(broadcastPermission)) < 0) {
        perror("setsockopt failed");
        exit(1);
    }

    // Setup the broadcast address structure
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = inet_addr(BROADCAST_IP);
    broadcastAddr.sin_port = htons(PORT);


    // Broadcast the message
    if (sendto(sockfd, MESSAGE, strlen(MESSAGE), 0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr)) < 0) {
        perror("sendto failed");
        exit(1);
    }

    printf("Broadcast message sent: %s\n", MESSAGE);

    // Close the socket
    close(sockfd);
    return 0;
}
