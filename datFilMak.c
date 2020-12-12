	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/socket.h>
	#include <unistd.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <signal.h>
	#include <fcntl.h>

	#define PORTNUM 9090

	int main() {
		char buf[256];
		char memBuf[80];
		char *p;
		struct sockaddr_in sin, cli;
		int sd, clientlen = sizeof(cli);
		int ns, nsCnt=0;
		char ind[4];
		char id[20];
		char pasWor[20];
		char nicNam[20];
		pid_t forkN;
		int ok = 1;
		FILE *fp;
		char memFilNam[30] = "./dat/memberFile";
		char chaRooFilNam[30] = "./dat/chatRoomFile";
		char friFilNam[30] = "./dat/friendFile";
		char find1[20];
		char find2[20];
		int i;
		int exist;
		int d;
		
		//파일들 생성
		//dir(dat), memberFile, chatRoomFile, friendFile
		/*
		mkdir("dat",0777);
		fp = fopen(memFilNam,"a+");
		fscanf(fp,"%s",buf);
		if(buf[0]=='\0') {
			fprintf(fp,"id,pasWor,nicNam,0 qqq,www,eee,1 because,im,happy,2 aaa,sss,ddd,3 here,is,why,4 zzz,xxx,ccc,5 ");
		}
		fclose(fp);
		fp = fopen(chaRooFilNam,"a+");
		fscanf(fp,"%s",buf);
		buf[0]=='\0';
		if(buf[0]=='\0') {
			fprintf(fp,"id,id,id,... qqq, beacuse, aaa, here, zzz, ");
		}
		fclose(fp);
		fp = fopen(friFilNam,"a+");
		fscanf(fp,"%s",buf);
		if(buf[0]=='\0') {
			fprintf(fp,"id,id,id,... qqq, beacuse, aaa, here, zzz, ");
		}
		fclose(fp);
		*/
		
		mkdir("dat",0777);
		fp = fopen(memFilNam,"a+");
		fscanf(fp,"%s",buf);
		if(buf[0]=='\0') {
			fprintf(fp,"qqq,www,eee,0 aaa,sss,ddd,1 zzz,xxx,ccc,2 because,im,happy,3 happy,happy,happy,4 ");
		}
		fclose(fp);
		fp = fopen(chaRooFilNam,"a+");
		fscanf(fp,"%s",buf);
		buf[0]=='\0';
		if(buf[0]=='\0') {
			fprintf(fp,"qqq, aaa, zzz, because, happy, ");
		}
		fclose(fp);
		fp = fopen(friFilNam,"a+");
		fscanf(fp,"%s",buf);
		if(buf[0]=='\0') {
			fprintf(fp,"qqq,aaa,zzz,because,happy, aaa,qqq, zzz,qqq, because,qqq, happy,qqq, ");
		}
		fclose(fp);
		
		
		return 0;
	}