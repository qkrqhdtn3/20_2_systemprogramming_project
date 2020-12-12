#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

#define PORTNUM 9090

int main() {
	struct sockaddr_in sin;
	int sd;
	int ok;
	pid_t forkN;
	char buf[256];
	char menStr[300];
	char id[20];
	char fin[20];
	char pasWor[20];
	char nicNam[20];
	int d;
	int menNum;
	char che[3]="@";
	char emot[10][450] = {
        "\n    ....    ..,..   \n   .~~~~,. .-~-~-.  \n  .~,...~,.~-...,~. \n  :.    .---  .. ~. \n .:......,-..  ..,, \n .-..............,, \n  ~..............,, \n  ~,............,~. \n  .-,..........,--  \n   --,.......,,,~.  \n   .-,,.......,-,   \n    ,-,.......,-    \n     ,,,.....,-.    \n      ,-....,-.     \n       ,,..,-,      \n        --,-,       \n         -~,        \n         .,         \n",
        "\n      ...           \n      .,,,..        \n     .,..  ...      \n   .,.       .,.    \n  .,           ,.   \n .,             .   \n ,.             ..  \n.,    :-   -~    .. \n,.   .:~   ~:,   .. \n..   .~,   ,~.    . \n..                . \n..                . \n..   ---------    . \n ,   ,,,,,,,,.   .  \n ..             ..  \n  ,            .,   \n  ..          .,.   \n   .,,       ,,.    \n     .,,,,,,,,.     \n       .,,,..       \n",
        "\n       ...,.        \n      .,,.,,.       \n    .,       ..     \n   ,,         ..    \n  .,           ..   \n  ,             .   \n ..   ..   ..    .  \n .   ~::- ,;:~   .  \n .               .  \n .   .           .  \n .               .  \n     -:::::::-   .  \n  .   ...... .  .   \n  ..            ,   \n   ,.         .,    \n    ,.       .,     \n     .,....,,.      \n",
        "\n        ..          \n      ..,,,,.       \n     ,,     ..      \n    ,.        ,.    \n   ..          ,.   \n  .             ,   \n  .    ,   ..   .   \n  .   .;,  ~:    .  \n       -.  ,,    .  \n                 ,  \n  .              .  \n  .    .-~~-,   ..  \n  .   .-.       .   \n   .            .   \n   ..          ,    \n    .,        ,     \n     .,,. ..,.      \n        ....        \n",
        "\n        ..          \n       .,,,,..      \n      ,.    ...     \n    .,        .,.   \n   .,          .,   \n   ,            .,  \n  ,.             ,  \n  ,    ~~   :,   .  \n  .    :~  .:,    . \n  .    ..   .     . \n ..               . \n  ,        .      . \n  ,        ,-    .. \n  ..      .-,    .  \n   ,       --   ..  \n    ..     ,,  ..   \n    .,.        .    \n      .,. ....      \n        .,,,        \n",
        "\n           .        \n        .....       \n      ......,,.     \n    .,.       ,,    \n   .,          ..   \n   .            .   \n  .              .  \n .,                 \n ..               . \n ..               . \n ..   -~.   --.   . \n  . .:-.     ,~,  . \n  ,              .  \n  .      --     ..  \n   ,    ~~~:.   ,.  \n    .   .   .  ..   \n     ,.      .,     \n      .,,..,,-      \n",
        "\n         ..         \n       ......       \n     .,,....,,      \n    .,        ,,    \n    .          .,   \n  ..            ..  \n  .              ,  \n  .              .  \n      -.    .-      \n     .:-    ,:,   . \n .    -.    .-    . \n  .                 \n  ,     .~~      .  \n  .    ,;;;;,    .  \n   .   ~~~~:~   .   \n    .   .  .   ..   \n    .,.      .,.    \n      .,....,,      \n",
        "\n      ..,,....      \n     ,,.    .,,     \n    ..        .,    \n   ..          ..   \n   .            .   \n  .    ~.  .-    .  \n  ,   .:-  ,:.   .  \n  .    ,    ,    .  \n  .              .  \n  .      ~~      .  \n  .     ~;;~     .  \n   .    -~:~    .   \n   .     --.   .,   \n     .        .,    \n     .,.    .,.     \n       ......       \n",
        "\n      ..,,,,.       \n     ,-.    .-.     \n    .         ..    \n   ,           .,   \n  .,            ,.  \n  ,   .-    -    .  \n  .   -;.  -;,   .  \n  .   ,~   ,:.   .  \n  .       .      .  \n ..              .  \n ..              ,  \n  ,              .  \n  ..            ,   \n   ,           .,   \n    ..        .-    \n    .,,.    .,,.    \n      .,,,,...      \n",
        "\n      ...,,...      \n      .,..  .,,     \n    ...       .,.   \n    ..         ..   \n    .           ..  \n   .    ,   .    .  \n   .   .;,  :-   .. \n   .    ,   ..    . \n   .             .. \n   . -::~---~~:~ .. \n   . ,-.,,-,,.-- .  \n   .. ~;:~~~::~ ..  \n    ,  ---~~~~..,   \n     .  .,,,,  ,.   \n      ,.     .,     \n       ....,,.      \n"
		};
		
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("210.115.229.76");
	
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(1);
	}
	
	if (connect(sd, (struct sockaddr *)&sin, sizeof(sin))){
		perror("connect");
		exit(1);
	}
	
	//login 또는 sign up하는 메뉴
	ok=2;
	while(ok==2){
		printf("1.log in, 2.sign up\n1또는 2를 입력하세요.\n"); //
		scanf("%s",buf);
		send(sd,buf,sizeof(buf),0);
		if(strcmp(buf,"1")==0) ok=0;
		else if(strcmp(buf,"2")==0) ok=1;
		else ok=2;
	}
	
	//sing up 구현
	while(ok){
		printf("Typing id:");
		scanf("%s",id);
		send(sd,id,sizeof(id),0);
		if(strcmp(id,"QUIT")==0){
			close(sd);
			return 0;
		}
		printf("Typing pasWor:");
		scanf("%s",pasWor);
		send(sd,pasWor,sizeof(pasWor),0);
		printf("Typing nicNam:");
		scanf("%s",nicNam);
		send(sd,nicNam,sizeof(nicNam),0);
		recv(sd,buf,sizeof(buf),0);
		if(buf[0]==' ') {
			printf("can't use '@'','' ''|' at first letter\n");
			continue;
		}
		send(sd,buf,sizeof(buf),0);
		recv(sd,buf,sizeof(buf),0);
		if(strcmp(buf,"not exist id in memberFile")!=0) {
			printf("%s\n",buf);
			continue;
		}
		printf("Sign up complete!\nLog in now");
		ok=0;
	}
	
	//login 구현
	ok=1;
	while(ok){
		printf("Id:"); //
		scanf("%s",id);
		send(sd,id,sizeof(id),0);
		if(strcmp(id,"QUIT")==0){
			close(sd);
			return 0;
		}
		recv(sd,buf,sizeof(buf),0);
		if(strcmp(buf,"loginAgain")==0){
			printf("%s\n",buf);
			ok = 1;
			continue;
		}
		printf("PasWor:"); //
		scanf("%s",pasWor);
		send(sd,pasWor,sizeof(pasWor),0);
		recv(sd,buf,sizeof(buf),0);
		if(strcmp(buf,"loginAgain")==0){
			printf("%s\n",buf);
			ok = 1;
			continue;
		}
		printf("%s\n",buf);
		ok = 0;
	}
	
	//메뉴 선택
	ok=1;
	send(sd,che,sizeof(che),0);
	while(ok){
		recv(sd,menStr,sizeof(menStr),0);
		//sleep(1);
		printf("%s메뉴 선택 : ",menStr);
		scanf("%s",buf);
		send(sd,buf,sizeof(buf),0);
		recv(sd,buf,sizeof(buf),0);
		if(strcmp(buf,"@N")==0){
			send(sd,che,sizeof(che),0);
			continue;
		}
		menNum = buf[0]-48;
		switch(menNum){
			case 1:
				//대화방 목록 확인, 대화방 참여
				//
				//대화방 목록
				send(sd,che,sizeof(che),0);
				recv(sd,buf,sizeof(buf),0);
				printf("%s",buf);
				send(sd,che,sizeof(che),0);
				recv(sd,buf,sizeof(buf),0);
				if(strcmp(buf,"@N")==0){
					printf("대화방이 존재하지 않습니다.\n");
					send(sd,che,sizeof(che),0);
					continue;
				}
				send(sd,che,sizeof(che),0);
				while(1){
					recv(sd,fin,sizeof(fin),0);
					if(fin[0]=='@')
					break;
					printf("%s\n",fin);
					send(sd,che,sizeof(che),0);
				}
				send(sd,che,sizeof(che),0);
				recv(sd,buf,sizeof(buf),0);
				printf("%s",buf);
				//대화방 참여
				printf("\n참여할 대화방의 친구 id (19바이트 이내): ");
				scanf("%s",fin);
				printf("\n");
				send(sd,fin,sizeof(fin),0); //
				recv(sd,buf,sizeof(buf),0); //
				if(strcmp(buf,"@N")==0){
					printf("어떤 대화방도 존재하지 않습니다.\n");
					send(sd,che,sizeof(che),0); //
					continue;
				}
				send(sd,che,sizeof(che),0); //
				recv(sd,buf,sizeof(buf),0); //
				if(strcmp(buf,"@N")==0){
					printf("친구와 대화방이 존재하지 않습니다.\n");
					send(sd,che,sizeof(che),0); //
					continue;
				}
				printf("대화방 참가 완료!\n");
				send(sd,che,sizeof(che),0); //
				recv(sd,che,sizeof(che),0); //
				ok=0;
				continue;
				break;
			case 2:
				//대화방 만들기
				printf("대화방만들 친구 id (19byte 이내): ");
				scanf("%s",fin);
				printf("\n");
				send(sd,fin,sizeof(fin),0);
				recv(sd,buf,sizeof(buf),0);
				printf("%s\n",buf);
				break;
			case 3:
				//친구 목록
				send(sd,che,sizeof(che),0);
				recv(sd,buf,sizeof(buf),0);
				if(strcmp(buf,"@N")==0){
					printf("친구가 없습니다. 친구를 추가하세요.\n");
					send(sd,che,sizeof(che),0);
					continue;
				}
				send(sd,che,sizeof(che),0);
				while(1){
					recv(sd,buf,sizeof(buf),0);
					if(strcmp(buf,"@N")==0)
						break;
					else if(buf[0]=='-')
						exit(0);
					printf("%s, ",buf);
					send(sd,che,sizeof(che),0);
				}
				printf("\n");
				break;
			case 4:
				printf("친구 추가할 id (19byte 이내): ");
				scanf("%s",fin);
				printf("\n");
				send(sd,fin,sizeof(fin),0);
				recv(sd,buf,sizeof(buf),0);
				if(strcmp(buf,"@N")==0){
					printf("가입된 id가 존재하지 않습니다.\n");
					send(sd,che,sizeof(che),0);
					continue;
				}
				send(sd,che,sizeof(che),0);
				recv(sd,buf,sizeof(buf),0);
				if(strcmp(buf,"@N")==0){
					printf("이미 친구입니다.\n");
				}
				else if(strcmp(buf,"@Y")==0){
					printf("친구추가 성공!\n");
				}
				send(sd,che,sizeof(buf),0);
				break;
			default:
				
				break;
		}
		send(sd,che,sizeof(che),0);
	}
	//채팅 시작
	printf("채팅 시작------------------------------\n종료를 원하면 @Q 입력\n이모티콘 리스트는 @E 입력\n이모티콘 전달은 @E(0~9사이의 숫자) 입력\n");
	forkN=fork();
	if(forkN > 0){
		while(1){
			fgets(buf, 256, stdin);
			if(buf[0]=='@'){
				if(buf[1]=='E'){
					if(buf[2]=='\n'){
						for(int i = 0 ; i < 10; i ++){
							printf("@E%d\n",i);
							printf("%s",emot[i]);
						}
						continue;
					}
				}
			}
			send(sd, buf, 256, 0);
			if(buf[0]=='@')
				if(buf[1]=='Q'&&buf[2]=='\n')
					break;
			if(strcmp(buf, "@QUIT\n")==0){
				break;
			}
			printf("%s: %s",id,buf);
		}
		kill(forkN,SIGKILL);
	}
	else if(forkN == 0){
		while(1){
			if (recv(sd, buf, 256, 0) == -1){
				perror("recv");
				exit(1);
			}
			if(buf[0]=='@'){
				if(buf[1]=='Q'&&buf[2]=='\n')
					break;
				if(buf[1]=='E'&&buf[2]>='0'&&buf[2]<='9'&&buf[3]=='\n'){
					printf("%s",emot[buf[2]-48]);
				}
			}
			if(strcmp(buf, "@QUIT\n")==0)
				break;
			printf("%s: %s",fin,buf);
		}
		kill(getppid(),SIGKILL);
	}
	close(sd);
	
	return 0;
}