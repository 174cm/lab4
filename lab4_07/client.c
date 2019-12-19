//
// Created by tt on 19. 12. 15..
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MYPORT 3490
#define MAXBUF 100

main(int argc, char* argv[]) {
/* 클라이언트 소켓 정보 획득 */
    int csock, numbytes;
    struct sockaddr_in serv_addr;
    char buf[MAXBUF];
    int len;

    if(argc != 3){
        fprintf(stderr, "Usage : tcp_client <SERVER IP> <ECHO STRING> \n");
        exit(1);
    }

    if ((csock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(MYPORT);

    if(connect(csock, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1){
        perror("connect");
        exit(1);
    }

    memset(buf, 0, MAXBUF);
    strcpy(buf, argv[2]);
    len = strlen(buf);

    if (send(csock, buf, len, 0) != len) {
        fprintf(stderr, "send failed...\n");
        exit(1);
    }
    memset(buf, 0, MAXBUF);

    if((numbytes = recv(csock, buf, MAXBUF, 0)) == -1){
        perror("recv");
        exit(1);
    }
    printf("Received : %s\n", buf);

    close(csock);
}