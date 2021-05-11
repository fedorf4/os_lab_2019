#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SERV_PORT 13001
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
    int serv_busy[4];
    for (int i=0;i<4;i++)
        serv_busy[i]=0;

    pid_t pid[4];
    for (int i = 0; i < 4; ++i) {
        pid[i] = fork();
        if (pid[i] > 0) {   /* I am the parent, create more children */

              if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket");
                exit(1);
            }

            memset(&servaddr, 0, kSize);
            servaddr.sin_family = AF_INET;
            servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
            servaddr.sin_port = htons(SERV_PORT+i);

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
                write(1, &buf, nread);
                char* st = (char*)malloc(strlen(buf)+1);
                strcpy(st, buf);
                printf("%s\n", st);
                printf("compare: %d, %d, %d\n", strcmp(st, "0"), strlen(st), strlen("0"));
                if (atoi(st)==0)
                {
                    printf("create request\n");
                    for (int i=0;i<4;i++)
                    {
                        if (serv_busy[i]==0)
                        {
                            char buffer[sizeof(i)];
                            memcpy(buffer, &i, sizeof(i));
                            int err = send(cfd, buffer, sizeof(i), 0);
                            if (err < 0) {
                                fprintf(stderr, "Can't send data to client\n");
                                break;
                            }
                            break;
                        }

                    }
                }
                if (strcmp(st, "free")==0)
                {
                    printf("free request\n");
                    for (int i=0;i<4;i++)
                    {
                        if (serv_busy[i]==1)
                            serv_busy[i]=0;
                        break;
                    }
                }

                }

                if (nread == -1) {
                perror("read");
                exit(1);
                }
                close(cfd);
            }
        } else if (pid[i] == 0) { /* I am a child, get to work */
        int sockfd, n;
        struct sockaddr_in servaddr[4];
        struct sockaddr_in cliaddr;
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("socket problem");
            exit(1);
        }
        memset(&servaddr, 0, SLEN);
        servaddr[i].sin_family = AF_INET;
        servaddr[i].sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr[i].sin_port = htons(SERV_PORT+i);

        if (bind(sockfd, (SADDR *)&servaddr[i], SLEN) < 0) {
            perror("bind problem");
            exit(1);
        }
        printf("UDP SERVER starts...\n");
           // break;
        } else {
            printf("fork error\n");
            exit(1);
        }
    }
    

}
