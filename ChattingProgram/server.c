#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define MAX_LINE  511
#define MAX_SOCK 1024

char *EXIT_STRING = "exit"; // 클라이언트의 종료요청 문자열
char *START_STRING = "Connected to chat_server \n"; // 클라이언트 환영 메시지

int max_fdp1;				// 최대 소켓번호 +1
int num_user = 0;			// 채팅 참가자 수
int num_chat = 0;			// 지금까지 오간 대화의 수
int client_sock_list[MAX_SOCK];		// 채팅에 참가자 소켓번호 목록
char ip_list[MAX_SOCK][20];		//접속한 ip 목록
int listen_sock;			// 서버의 리슨 소켓
void addClient(int s, struct sockaddr_in *new_client_addr);    // 새로운 채팅 참가자 처리
int get_max();				// 최대 소켓 번호 찾기
void removeClient(int s);	// 채팅 탈퇴 처리 함수
int tcp_listen(int host, int port, int backlog); // 소켓 생성 및 listen
void err_quit(char *msg) { perror(msg); exit(1); }

time_t ct;
struct tm tm;

//명령어를 처리할 스레드
void *thread_function(void *arg) {
	int i;
	printf("명령어 목록 : help, num_user, num_chat, ip_list\n");
	while (1) {
		char buf_msg[MAX_LINE + 1];
		fprintf(stderr, "\033[1;32m"); //글자색을 녹색으로 변경
		printf("server > ");
		fgets(buf_msg, MAX_LINE, stdin); //명령어 입력
		if (!strcmp(buf_msg, "\n")) continue;   //엔터 무시
		else if (!strcmp(buf_msg, "help\n"))
			printf("help, num_user, num_chat, ip_list\n");
		else if (!strcmp(buf_msg, "num_user\n"))
			printf("현재 참가자 수 = %d\n", num_user);
		else if (!strcmp(buf_msg, "num_chat\n"))
			printf("지금까지의 대화의 수 = %d\n", num_chat);
		else if (!strcmp(buf_msg, "ip_list\n"))
			for (i = 0; i < num_user; i++)
				printf("%s\n", ip_list[i]);
		else //예외 처리
			printf("해당 명령어가 없습니다. help 를 참조하세요.\n");
	}
}

int main(int argc, char *argv[]) {
	struct sockaddr_in client_addr;
	char buf[MAX_LINE + 1]; //클라이언트에서 받은 메시지
	int i, j, nbyte, accp_sock, addrlen = sizeof(struct
		sockaddr_in);
	fd_set read_fds;	//읽기를 감지할 fd_set 구조체
	pthread_t a_thread;

	if (argc != 2) {
		printf("사용법 :%s port\n", argv[0]);
		exit(0);
	}

	// tcp_listen(host, port, backlog) 함수 호출
	listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]), 5);
	//스레드 생성
	pthread_create(&a_thread, NULL, thread_function, (void *)NULL);
	while (1) {
		FD_ZERO(&read_fds);
		FD_SET(listen_sock, &read_fds);
		for (i = 0; i < num_user; i++)
			FD_SET(client_sock_list[i], &read_fds);

		max_fdp1 = get_max() + 1;	// max_fdp1 재 계산
		if (select(max_fdp1, &read_fds, NULL, NULL, NULL) < 0)
			err_quit("select fail");

		if (FD_ISSET(listen_sock, &read_fds)) {
			accp_sock = accept(listen_sock,
				(struct sockaddr*)&client_addr, &addrlen);
			if (accp_sock == -1) err_quit("accept fail");
			addClient(accp_sock, &client_addr);
			send(accp_sock, START_STRING, strlen(START_STRING), 0);
			ct = time(NULL);			//현재 시간을 받아옴
			tm = *localtime(&ct);
			write(1, "\033[0G", 4);		//커서의 X좌표를 0으로 이동
			printf("[%02d:%02d:%02d]", tm.tm_hour, tm.tm_min, tm.tm_sec);
			fprintf(stderr, "\033[33m");//글자색을 노란색으로 변경
			printf("사용자 1명 추가. 현재 참가자 수 = %d\n", num_user);
			fprintf(stderr, "\033[32m");//글자색을 녹색으로 변경
			fprintf(stderr, "server>"); //커서 출력
		}

		// 클라이언트가 보낸 메시지를 모든 클라이언트에게 방송
		for (i = 0; i < num_user; i++) {
			if (FD_ISSET(client_sock_list[i], &read_fds)) {
				num_chat++;				//총 대화 수 증가
				nbyte = recv(client_sock_list[i], buf, MAX_LINE, 0);
				if (nbyte <= 0) {
					removeClient(i);	// 클라이언트의 종료
					continue;
				}
				buf[nbyte] = 0;
				// 종료문자 처리
				if (strstr(buf, EXIT_STRING) != NULL) {
					removeClient(i);	// 클라이언트의 종료
					continue;
				}
				// 모든 채팅 참가자에게 메시지 방송
				for (j = 0; j < num_user; j++)
					send(client_sock_list[j], buf, nbyte, 0);
				printf("\033[0G");		//커서의 X 좌표를 0으로 이동
				fprintf(stderr, "\033[97m");//글자색을 흰색으로 변경
				printf("%s", buf);			//메시지 출력
				fprintf(stderr, "\033[32m");//글자색을 녹색으로 변경
				fprintf(stderr, "server>"); //커서 출력
			}
		}
	}
	return 0;
}

// 새로운 채팅 참가자 처리
void addClient(int s, struct sockaddr_in *new_client_addr) {
	char buf[20];
	inet_ntop(AF_INET, &new_client_addr->sin_addr, buf, sizeof(buf));
	write(1, "\033[0G", 4);		//커서의 X 좌표를 0으로 이동
	fprintf(stderr, "\033[33m");	//글자색을 노란색으로 변경
	printf("new client: %s\n", buf); // ip 출력
	// 채팅 클라이언트 목록에 추가
	client_sock_list[num_user] = s;
	strcpy(ip_list[num_user], buf);
	num_user++; //유저 수 증가
}

// 채팅 탈퇴 처리
void removeClient(int s) {
	close(client_sock_list[s]);
	if (s != num_user - 1) { //저장된 리스트 재배열
		client_sock_list[s] = client_sock_list[num_user - 1];
		strcpy(ip_list[s], ip_list[num_user - 1]);
	}
	num_user--; //유저 수 감소
	ct = time(NULL);			//현재 시간을 받아옴
	tm = *localtime(&ct);
	write(1, "\033[0G", 4);		//커서의 X 좌표를 0으로 이동
	fprintf(stderr, "\033[33m");//글자색을 노란색으로 변경
	printf("[%02d:%02d:%02d]", tm.tm_hour, tm.tm_min, tm.tm_sec);
	printf("채팅 참가자 1명 탈퇴. 현재 참가자 수 = %d\n", num_user);
	fprintf(stderr, "\033[32m");//글자색을 녹색으로 변경
	fprintf(stderr, "server>"); //커서 출력
}

// 최대 소켓번호 찾기
int get_max() {
	// Minimum 소켓번호는 가정 먼저 생성된 listen_sock
	int max = listen_sock;
	int i;
	for (i = 0; i < num_user; i++)
		if (client_sock_list[i] > max)
			max = client_sock_list[i];
	return max;
}

// listen 소켓 생성 및 listen
int  tcp_listen(int host, int port, int backlog) {
	int sd;
	struct sockaddr_in server_addr;

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1) {
		perror("socket fail");
		exit(1);
	}
	// server_addr 구조체의 내용 세팅
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(host);
	server_addr.sin_port = htons(port);
	if (bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("bind fail");  exit(1);
	}
	// 클라이언트로부터 연결요청을 기다림
	listen(sd, backlog);
	return sd;
}
