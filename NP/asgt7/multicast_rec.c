
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MULTICAST_GROUP "239.0.0.1"  // Multicast group address (Class D)
#define MULTICAST_PORT 12345         // Port to receive the message
#define BUFFER_SIZE 256

int main() {
    int sockfd;
    struct sockaddr_in local_addr;
    struct ip_mreq multicast_request;
    char buffer[BUFFER_SIZE];
    int reuse = 1;

    // 1. Create a UDP socket (AF_INET, SOCK_DGRAM)
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Set SO_REUSEADDR option to allow multiple receivers on the same port
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("Setting SO_REUSEADDR failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 3. Initialize the sockaddr_in structure (local address and port)
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // Receive datagrams for any local IP
    local_addr.sin_port = htons(MULTICAST_PORT);     // Port to receive the multicast

    // Bind the socket to the local address and port
    if (bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 4. Use IP_ADD_MEMBERSHIP to join the multicast group
    multicast_request.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP);  // Multicast group
    multicast_request.imr_interface.s_addr = htonl(INADDR_ANY);           // Local interface

    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicast_request, sizeof(multicast_request)) < 0) {
        perror("Adding to multicast group failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 5. Receive the multicast message
    while (1) {
        int nbytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, 0);
        if (nbytes < 0) {
            perror("Receive failed");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        buffer[nbytes] = '\0';  // Null-terminate the received message
        printf("Received multicast message: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
