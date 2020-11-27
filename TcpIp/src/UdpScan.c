#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

/*
 * Return: 0/1/2/3 [Open/Close/Filter(timeout)/unkown error]
 */
int getUdpPortConnectStatus(int port, const char *ip)
{
    struct timeval *tval;    
    char recvbuf[1000];
    int sockfd = 0;

    if (port<=0 || ip==NULL) {
        printf("IP:%s Port:%d\n", ip, port);
        return -1;
    } 
    
    sockfd = socket(AF_INET,SOCK_DGRAM,0); 
    if(sockfd < 0) {
        printf("socket: %s\n", strerror(errno));
        return -1;
    }

    {
        struct sockaddr_in addr;
        int val=1, socklen=sizeof(addr);
        char sendbuf[32];
        struct timeval tv;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip);

        tv.tv_sec = 3;
        tv.tv_usec = 0;
        if(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0) {
            printf("send so_sndtimeo err: %s\n", strerror(errno));
        }
        if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))<0) {
            printf("socket option  SO_RCVTIMEO not support: %s\n", strerror(errno));
        }
        setsockopt(sockfd, IPPROTO_IP, IP_RECVERR , &val,sizeof(int));  
        memset(sendbuf, 0, sizeof(sendbuf));
        //strcpy(sendbuf, "Action:AppNotify\r\nInterCmd:CheckIpConflict\r\nInterface:eth0\r\nIP:192.168.1.200\r\n\r\n");       
        sprintf(sendbuf, "%d", port);       
        if (sendto(sockfd, sendbuf, strlen(sendbuf),0, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            printf("socket option  SO_RCVTIMEO not support: %s\n", strerror(errno));
        }
        sleep(1);
        memset(recvbuf, 0, 1000);
        if (recvfrom(sockfd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr*)&addr, &socklen) < 0)
        {
            printf("%s:%d recvfrom error %d \n", ip, port, errno);
            switch(errno)
            {
                case 11: {
                    printf("Udp Port %d - NO replay -Status: %s\n", port, strerror(errno));
                    return 0;
                }
                case 111: {
                    printf("Udp Port %d - NO open -Status: %s\n", port, strerror(errno));
                    return 1;
                }
                case 113: {
                    printf("Udp Port %d - UnReach or filter-status: %s\n", port, strerror(errno));
                    return 2;
                }
                default: {
                    printf("Udp Port %d OK\n", port);
                    return 3;
                }
            }
        }
		else {
            printf("Port %d buff: %s \n", port, recvbuf);
		}
    }
    close(sockfd);
    return 0;
}

int main(int argc,char **argv) {
    int port= 0;
	if (argc!=3) {
		printf("Usage: %d <port> <ip>\n", argc);
		return 0;
	}
	port = atoi(argv[1]);
	return getUdpPortConnectStatus(port, argv[2]);
}

