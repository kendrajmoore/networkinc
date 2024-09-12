#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>

#define PACKET_SIZE 64
#define TIMEOUT 1
#define ICMP_ECHO 8

unsigned short calculate_checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }
    if (len == 1) {
        sum += *(unsigned char*)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void measure_latency(const char *ip_addr) {
    struct sockaddr_in addr;
    int sockfd;
    struct timeval start, end;
    struct icmp icmp_hdr;
    char packet[PACKET_SIZE];
    ssize_t bytes_sent, bytes_received;
    struct timeval timeout = {TIMEOUT, 0};

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }


    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));


    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip_addr, &addr.sin_addr);


    memset(&icmp_hdr, 0, sizeof(icmp_hdr));
    icmp_hdr.icmp_type = ICMP_ECHO;
    icmp_hdr.icmp_code = 0;
    icmp_hdr.icmp_id = getpid();
    icmp_hdr.icmp_seq = 1;
    icmp_hdr.icmp_cksum = calculate_checksum(&icmp_hdr, sizeof(icmp_hdr));

    memcpy(packet, &icmp_hdr, sizeof(icmp_hdr));


    gettimeofday(&start, NULL);
    bytes_sent = sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr*)&addr, sizeof(addr));
    if (bytes_sent <= 0) {
        perror("Packet sending failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    socklen_t addr_len = sizeof(addr);
    bytes_received = recvfrom(sockfd, packet, sizeof(packet), 0, (struct sockaddr*)&addr, &addr_len);
    gettimeofday(&end, NULL);

    if (bytes_received <= 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("Request timed out\n");
        } else {
            perror("Packet receiving failed");
        }
    } else {
        double time_taken = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
        printf("Reply from %s: time=%.2f ms\n", ip_addr, time_taken);
    }

    close(sockfd);
}

int main(int argc, char *argv[]) {
    const char *default_ip = "8.8.8.8";
    const char *ip_address;

    if (argc == 2) {
        ip_address = argv[1];
    } else {
        printf("No IP address provided, using default: %s\n", default_ip);
        ip_address = default_ip;
    }

    measure_latency(ip_address);
    return 0;
}
