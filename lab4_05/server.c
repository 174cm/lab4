/*
author: github 174cm
title: server.c
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
// 포트번호를 정의한다.
#define PORTNUM 9000
 
int main(void) {
    char buf[256];
    struct sockaddr_in sin, cli;
    int sd, ns, clientlen = sizeof(cli);
 
    // 서버 주소 구조체에 소켓의 종류를 AF_INMET으로
    // 지정하고 포트번호와 서버의 IP 주소를 설정한다.
    memset((char *)&sin, '\0', sizeof(sin));
    sin.sin_family  = AF_INET;
    sin.sin_port    = htons(PORTNUM);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
 
    // 소켓 생성시 SOCK_STREAM으로 지정해 TCP를 사용한다.
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
 
    // 생성한 소켓을 bind 함수를 사용해 위에서 설정한 IP주소/포트 번호와 연결한다.
    if(bind(sd, (struct sockaddr *)&sin, sizeof(sin))) {
        perror("bind");
        exit(1);
    }
 
    // listen 함수를 호출해 클라이언트의 요청을 받을 준비를 마쳤음을
    // 운영체제에 알린다. 한번에 5개의 클라이언트가 접속할 수 있도록 설정한다.
    if(listen(sd, 5)) {
        perror("listen");
        exit(1);
    }
 
    // while 문으로 무한 반복하게 한다(이 서버는 무한 반복하면서 서비스를 제공)
    while(1) {
        // accept 함수를 사용해 클라이언트의 요청이 올때까지 기다린다.
        // 클라이언트의 접속 요청이 오면 새로운 소켓 기술자 ns가 생성되고
        // 이를 통해 클라이언트와 통신한다.
        if((ns = accept(sd, (struct sockaddr *)&cli, &clientlen)) == -1) {
            perror("accept");
            exit(1);
        }
 
        // accept 함수를 통해 알아낸 클라이언트의 IP 주소를 문자열로
        // 변환해 버퍼에 저장한다.
        sprintf(buf, "%s", inet_ntoa(cli.sin_addr));
        // 어느 클라이언트가 서버로 접속했는지 출력한다.
        printf("*** Send a Message to Client(%s)\n", buf);
 
        // 서버에서 클라이언트로 보낼 간단한 환영 메시지를 작성한다.
        strcpy(buf, "Welcome to Network Server!!!");
        // send 함수를 사용해 클라이언트로 메시지를 보낸다.
        if(send(ns, buf, strlen(buf) + 1, 0) == -1) {
            perror("send");
            exit(1);
        }
        // 클라이언트가 보낸 메시지를 받아서 출력한다.
        if(recv(ns, buf, strlen(buf), 0) == -1) {
            perror("recv");
            exit(1);
        }
        // 작업이 끝나면 클라이언트와 접속할 때 사용한 소켓 기술자를 닫고,
        // 다시 accept 함수를 수행해 클라이언트의 접속을 기다린다.
        printf("** From Client : %s\n", buf);
        close(ns);
    }
    close(sd);
 
    return 0;
}

