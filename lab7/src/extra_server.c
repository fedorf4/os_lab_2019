#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>



#define SERV_PORT 30001
#define BUFSIZE 100
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char **argv) {
    const size_t kSize = sizeof(struct sockaddr_in);

    int lfd, cfd;
    int nread;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    char buf[BUFSIZE];
    char mesg[BUFSIZE];
    int serv_busy[4];
    for (int i=0;i<4;i++)
        serv_busy[i]=0;

    pid_t pid;
    pid_t pid1;
    pid_t pid2;
        pid = fork();
        if (pid > 0) {   /* Create TCP server */
        //if (i==0)
        //{

              if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket");
                exit(1);
            }

            memset(&servaddr, 0, kSize);
            servaddr.sin_family = AF_INET;
            servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
            servaddr.sin_port = htons(SERV_PORT);

            if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
                perror("bind");
                exit(1);
            }

            if (listen(lfd, 5) < 0) {
                perror("listen");
                exit(1);
            }

            while (1) {
                unsigned int clilen = kSize;
                if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
                perror("accept");
                exit(1);
                }
                printf("connection established\n");

                while ((nread = read(cfd, buf, BUFSIZE)) > 0) {
                //write(1, &buf, nread);
                char* st = (char*)malloc(strlen(buf)+1);
                strcpy(st, buf);
                memset(&buf[0], '\0', sizeof(buf));
                if (strcmp(st, "create\n")==0)
                {
                    printf("create request\n");
                    for (int i=0;i<4;i++)
                    {
                        if (serv_busy[i]==0)
                        {
                            serv_busy[i]=1;
                            int udp_port =SERV_PORT+i+1;
                            char buffer[sizeof(udp_port)];
                            memcpy(buffer, &udp_port, sizeof(udp_port));
                            int err = send(cfd, buffer, sizeof(buffer), 0);   //Send the port of a free udp server to the client to work with it.
                            if (err < 0) {
                                fprintf(stderr, "Can't send data to client\n");
                                break;
                            }
                            break;
                        }

                    }
                }
                if (strcmp(st, "free\n")==0)
                {
                    printf("free request\n");
                    for (int i=0;i<4;i++)
                    {
                        if (serv_busy[i]==1)
                            serv_busy[i]=0;
                        break;
                    }
                }
                free(st);

                }

                if (nread == -1) {
                perror("read");
                exit(1);
                }
                close(cfd);
            }
        //}
        } else if (pid == 0) { /* Create 4 UDP server with different ports */
        int idx;
        pid1 = fork();
        if (pid1>0)
            idx =0;
        else idx = 1;
        pid2 = fork();
        if (pid2>0)
            idx =idx*2+1;
        else idx = (idx+1)*2;
        int sockfd, n;
        struct sockaddr_in servaddr;
        struct sockaddr_in cliaddr;
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("socket problem");
            exit(1);
        }
        memset(&servaddr, 0, SLEN);
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(SERV_PORT+idx);

        if (bind(sockfd, (SADDR *)&servaddr, SLEN) < 0) {
            perror("bind problem");
            exit(1);
        }
        printf("UDP SERVER %d starts...\n", SERV_PORT+idx);
        unsigned int len = SLEN;
        while(1)
        {
        if ((n = recvfrom(sockfd, mesg, BUFSIZE, 0, (SADDR *)&cliaddr, &len)) < 0) {
        perror("recvfrom");
        exit(1);
        }
        mesg[n] = 0;

        printf("UDP SERVER %d REQUEST %s  \n",SERV_PORT+idx, mesg);
           // break;
        }

        } else {
            printf("fork error\n");
            exit(1);
        }

    

}
