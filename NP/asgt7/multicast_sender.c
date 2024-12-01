#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MULTICAST_GROUP "239.0.0.1"  // Multicast group address (Class D)
#define MULTICAST_PORT 12345         // Port to send the message
#define MESSAGE "Hello, Multicast!"

int main() {
    int sockfd;
    struct sockaddr_in multicast_addr;
    char *multicast_message = MESSAGE;
    int loopback = 1;  // Enable loopback for local system to receive the datagrams

    // 1. Create a UDP socket (AF_INET, SOCK_DGRAM)
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Initialize the sockaddr_in structure
    memset(&multicast_addr, 0, sizeof(multicast_addr));
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = inet_addr(MULTICAST_GROUP);  // Set multicast group
    multicast_addr.sin_port = htons(MULTICAST_PORT);              // Set port

    // 3. Set the IP_MULTICAST_LOOP socket option (allows loopback)
    if (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &loopback, sizeof(loopback)) < 0) {
        perror("Setting IP_MULTICAST_LOOP failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 4. Set the IP_MULTICAST_IF socket option to define the local interface to send datagrams
    struct in_addr local_interface;
    local_interface.s_addr = inet_addr("127.0.0.1");  // Local interface IP address

    if (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, &local_interface, sizeof(local_interface)) < 0) {
        perror("Setting IP_MULTICAST_IF failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 5. Send the multicast message
    if (sendto(sockfd, multicast_message, strlen(multicast_message), 0, (struct sockaddr *)&multicast_addr, sizeof(multicast_addr)) < 0) {
        perror("Sendto failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Multicast message sent: %s\n", multicast_message);
    close(sockfd);
    return 0;
}
