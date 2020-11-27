#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <signal.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <pthread.h> 
#include <signal.h>
#include <unistd.h>

#define SEQ 12345 
#define TCPSIZE sizeof(struct tcphdr) 

int sendSyn(int sockfd,u_long sourceIP,u_short sourcePort,u_long seqNum,struct sockaddr_in * dst); 
void * recv_packet(void * arg); 

int fd;
u_short sourcePort=1234; 

unsigned short in_cksum(unsigned short * addr,int len)
{
    /*计算较验和的*/
    int nleft=len;
    int sum=0;
    unsigned short * w=addr;
    unsigned short answer=0;
 
    while (nleft>1) {
        sum+=*w++;
        nleft-=2;
    }

    if (nleft==1) {
        *(unsigned char *)(&answer)=*(unsigned char *)w;
        sum+=answer;
    }
    sum=(sum>>16)+(sum & 0xffff);
    sum+=(sum>>16);
    answer=~sum;
    return(answer);
}

void Alarm(int sig)
{
    kill(0,SIGHUP);
}

int main(int argc,char **argv) 
{ 
    int startip,hostsums,port, j; 
    struct hostent * phe; 
    struct ifreq if_data;
    struct sockaddr_in dest; 
    pthread_t tid; 
    u_long addr_p; 
    char * addr;
    if  (argc!=4)
        printf("Usage: %s <port> <host or startip> <hostsums> \n",argv[0]),exit(1); 

    port=atoi(argv[1]);
    hostsums=atoi(argv[3]); 
  
   /*
    *在这个fd上发数据,系统会自动给你加个IP头的
    *所以你只要自己构造TCP头就是了
    */
    if ((fd=socket(AF_INET,SOCK_RAW,IPPROTO_TCP))<0) {
        perror("socket");
        exit(1);
    }

    setuid(getuid()); /*放弃特权*/
    strcpy (if_data.ifr_name,"enp3s0");

    /*
    *取名为eth0的的IP地址
    *这是个interface的地址
    */
    if (ioctl (fd, SIOCGIFADDR, &if_data) < 0)  {
        perror("ioctl");
        exit(1);
    }

    memcpy ((void *) &addr_p, (void *) &if_data.ifr_addr.sa_data + 2, 4);
    /*把它放到addr_p中*/
    bzero(&dest,sizeof(dest)); 
    dest.sin_family=AF_INET; 
  
    if (phe=gethostbyname(argv[2])) 
        memcpy(&dest.sin_addr,phe->h_addr,phe->h_length); 
    else if (inet_aton(argv[2],&dest.sin_addr)<0) 
        perror("host"); 

    printf("Now starting scan...\n"); 
    fflush(stdout);
    signal(SIGALRM,Alarm);
    
    if ((errno=pthread_create(&tid,NULL,recv_packet,NULL))<0) {
        perror("pthread:"); 
    }

    for ( j=0 ; j < hostsums; j++ ) 
    { 
        dest.sin_port=htons(port); 
        if (sendSyn(fd,addr_p,sourcePort,SEQ,&dest)<0) 
            perror("send"); 
        dest.sin_addr.s_addr=htonl(ntohl(dest.sin_addr.s_addr)+1);
        if (j==hostsums-1) alarm(8);
    } 
    pthread_join(tid,NULL); /*等待线程结束*/
} 

void * recv_packet(void * arg) 
{ 
    struct sockaddr_in *in1;
    char *srcaddr;
    int loopend;
    int all=0;
    int size;
    u_char readbuff[1600];
    struct sockaddr from;
    int from_len;
    struct tcphdr* tcp=(struct tcphdr *)(readbuff+20);
    /*那个fd中读出的数据包括了IP头的所以+20*/
    for (;;)
    { 
        size = recvfrom(fd, (char *)readbuff, 1600, 0,&from, &from_len);
        if ( size<(20+20) )/*读出的数据小于两个头的最小长度的话continue*/
            continue;
    
        if ( (ntohl(tcp->ack_seq)!=12346)|| (ntohs(tcp->dest)!=sourcePort))
            continue;

        /* RST/ACK  - no service listening on port */ 
        if (tcp->rst && tcp->ack) 
        {  
            all++;
            continue;
        }

        /* SYN/ACK -Service listening on this port */ 
        if (tcp->ack && tcp->syn) 
        {
            /*这些已经没有什么了,就是系统调用*/
            struct servent * sptr; 
            in1=(struct sockaddr_in *)&from;;
            srcaddr=inet_ntoa(in1->sin_addr);
            printf("SERVER: %s ",srcaddr);
            if ((sptr=getservbyport(tcp->source,"tcp"))!=NULL) {
                printf("[32mPort:%5d   [31mServer: %s[37m\n",ntohs(tcp->source),sptr->s_name); 
            }
            else 
                printf("[32mPort:%5d [37m\n",ntohs(tcp->source));
            fflush(stdout); 
            all++;
            continue; 
        } 
    }/* end for */ 
} 

int sendSyn(int sendSocket,u_long sourceIP,u_short sourcePort,u_long seq,struct sockaddr_in * dst) 
{ 
    unsigned char netPacket[TCPSIZE]; 
    struct  tcphdr * tcp; 
    u_char * pPseudoHead; 
    u_char pseudoHead[12+sizeof(struct tcphdr)];
    u_short  tcpHeadLen; 
    memset(netPacket,0,TCPSIZE); 
    tcpHeadLen=htons(sizeof(struct tcphdr)); 
    tcp=(struct tcphdr *)netPacket; 
    tcp->source=htons(sourcePort); 
    tcp->dest=dst->sin_port; 
    tcp->seq=htonl(seq); 
    tcp->ack_seq=0; 
    tcp->doff=5; 
    tcp->syn=1; /*Syn的标志*/
    tcp->window=htons(10052); 
    tcp->check=0; 
    tcp->urg_ptr=0; 
    pPseudoHead=pseudoHead; 
    memset(pPseudoHead,0,12+sizeof(struct tcphdr)); 
    memcpy(pPseudoHead,&sourceIP,4);
    pPseudoHead+=4;
    memcpy(pPseudoHead,&(dst->sin_addr),4);
    pPseudoHead+=5;
    memset(pPseudoHead,6,1); 
    pPseudoHead++; 
    memcpy(pPseudoHead,&tcpHeadLen,2); 
    pPseudoHead+=2; 
    memcpy(pPseudoHead,tcp,sizeof(struct tcphdr));
    tcp->check=in_cksum((u_short *)pseudoHead,sizeof(struct tcphdr)+12);
    return (sendto(sendSocket,netPacket,TCPSIZE,0,(struct sockaddr*)dst,sizeof(struct sockaddr_in))); 
}


