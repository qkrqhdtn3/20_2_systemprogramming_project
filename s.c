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
	char datDir[7] = "./dat/";
	char memFilNam[30] = "./dat/memberFile";
	char chaRooFilNam[30] = "./dat/chatRoomFile";
	char friFilNam[30] = "./dat/friendFile";
	char chaRooPipNam[60];
	char chaRooPipNam2[60];
	char buf[256];
	char buf2[256];
	char memBuf[80];
	char friBuf[256];
	char ind[4];
	char id[20];
	char id2[20];
	char pasWor[20];
	char nicNam[20];
	char chaRooBuf[256];
	char menStr[300] = "-------------------------\n메뉴 목록입니다.\n1~ 4사이의 숫자를 입력하세요\n1 대화방 목록 확인, 대화방 참가\n2 대화방 만들기\n3 친구 목록\n4 친구 추가\n-------------------------\n";
	char chaRooMen1[60] = "-----------------\n대화방 목록\n";
	char chaRooMen2[30] = "-----------------\n";
	char find1[20];
	char find2[20];
	char che[3] = "@";
	char *p;
	char *cp;
	char cp2[2][256];
	char *cp3;
	char *stmp;
	struct sockaddr_in sin, cli;
	int sd, clientlen = sizeof(cli);
	int ns, nsCnt=0;
	int i;
	int exist;
	int d;
	int ok = 1;
	int okMen;
	int menNum;
	int pd;
	int n;
	int filSiz;
	int itmp;
	int cpi;
	pid_t forkN;
	FILE *fp;
	
	//파일들 없을 경우 생성
	//dir(dat), memberFile, chatRoomFile, friendFile
	mkdir("dat",0777);
	fp = fopen(memFilNam,"a+");
	fscanf(fp,"%s",buf);
	if(buf[0]=='\0') fprintf(fp,"id,pasWor,nicNam,0 ");
	fclose(fp);
	fp = fopen(chaRooFilNam,"a+");
	fscanf(fp,"%s",buf);
	if(buf[0]=='\0') fprintf(fp,"id,id,id,... ");
	fclose(fp);
	fp = fopen(friFilNam,"a+");
	fscanf(fp,"%s",buf);
	if(buf[0]=='\0') fprintf(fp,"id,id,id,... ");
	fclose(fp);
	
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("210.115.229.76");
	
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(1);
	}
	
	if (bind(sd, (struct sockaddr *)&sin, sizeof(sin))){
		perror("bind");
		exit(1);
	}
	
	if (listen(sd, 5)){
		perror("listen");
		exit(1);
	}
	
	while(1){
		if ((ns = accept(sd, (struct sockaddr *)&cli, &clientlen)) == -1){
			perror("accept");
			exit(1);
		}
		forkN=fork();
		if(forkN==0)
		{
			printf("loginStart\n"); //
			//login 또는 sing up 하는 메뉴
			ok=2;
			while(ok==2){
				recv(ns,buf,sizeof(buf),0);
				if(strcmp(buf,"1")==0) ok=0;
				else if(strcmp(buf,"2")==0) ok=1;
				else ok=2;
			}
			//sing up 구현
			i = -1 ;
			fp = fopen(memFilNam,"r");
			//i는 회원수
			while(feof(fp)==0){
				fscanf(fp,"%s",buf);
				i++;
			}
			fclose(fp);
			while(ok){
				recv(ns,id,sizeof(id),0);
				if(strcmp(id,"QUIT")==0){
					close(ns);
					close(sd);
					return 0;
				}
				recv(ns,pasWor,sizeof(pasWor),0);
				recv(ns,nicNam,sizeof(nicNam),0);
				
				//들어가면 안되는 특수문자가 있는지 확인
				exist = 0;
				for(int i = 0 ; i < strlen(id) ; i++){
					if(id[i]=='@' || id[i]=='|' || id[i]==' ' || id[i]==','){
						exist=1;
						break;
					}
				}
				if(strcmp(id,"id")==0) exist=1;
				for(int i = 0 ; i < strlen(pasWor) ; i++){
					if(pasWor[i]=='@' || pasWor[i]=='|' || pasWor[i]==' ' || pasWor[i]==','){
						exist=1;
						break;
					}
				}
				for(int i = 0 ; i < strlen(nicNam) ; i++){
					if(nicNam[i]=='@' || nicNam[i]=='|' || nicNam[i]==' ' || nicNam[i]==','){
						exist=1;
						break;
					}
				}
				if(exist==1){
					send(ns," ",1,0);
					continue;
				}
				else send(ns,"ok",2,0);
				recv(ns,buf,sizeof(buf),0);
				
				//is exist id?
				exist = 0;
				fp = fopen(memFilNam,"r");
				while(feof(fp)==0){
					fscanf(fp,"%s",buf);
					cp=strtok(buf,",");
					if(strcmp(id,cp)==0){
						exist = 1;
						break;
					}
				}
				fclose(fp);
				if(exist==0)
				send(ns,"not exist id in memberFile",40,0);
				else if(exist==1) {
					send(ns,"exist id in memberFile",40,0);
					continue;
				}
				//밑은 각 파일에 회원정보를 추가하는 코드입닌다.
				fp = fopen(memFilNam,"a");
				fprintf(fp,"%s,%s,%s,%d ",id,pasWor,nicNam,i);
				fclose(fp);
				fp = fopen(chaRooFilNam,"a");
				fprintf(fp,"%s, ",id);
				fclose(fp);
				fp = fopen(friFilNam,"a");
				fprintf(fp,"%s, ",id);
				fclose(fp);
				ok = 0;
			}
			
			//login 구현
			ok=1;
			while(ok){
				printf("id\n"); //
				//id
				i = 0;
				exist = 0;
				recv(ns,find1,sizeof(find1),0);
				if(strcmp(find1,"QUIT")==0){
					close(ns);
					close(sd);
					return 0;
				}
				fp = fopen(memFilNam,"r");
				while(feof(fp)==0){
					fscanf(fp,"%s",buf);
					strcpy(memBuf,buf);
					cp=strtok(buf,",");
					if(strcmp(find1,cp)==0){
						exist = 1;
						break;
					}
					i++;
				}
				fclose(fp);
				if(exist==1){
					send(ns,"exist",10,0);
				}
				else{
					send(ns,"loginAgain",20,0);
					ok = 1;
					continue;
				}
				
				printf("pasWor\n"); //
				//pasWor
				exist=0;
				cp=strtok(NULL,",");
				recv(ns,find1,sizeof(find1),0);
				if(strcmp(find1,cp)==0){
					exist = 1;
				}
				if(exist==1){
					send(ns,"exist",10,0);
					recv(ns,che,sizeof(che),0);
				}
				else{
					send(ns,"loginAgain",20,0);
					ok = 1;
					continue;
				}
				//회원의 id,pasWor,nicNam,ind 저장
				strcpy(buf,memBuf);
				cp=strtok(buf,",");
				strcpy(id,cp);
				cp=strtok(NULL,",");
				strcpy(pasWor,cp);
				cp=strtok(NULL,",");
				strcpy(nicNam,cp);
				cp=strtok(NULL,",");
				strcpy(ind,cp);
				ok = 0;
			}
			
			//메뉴 선택
			okMen=1;
			while(okMen){
				send(ns,menStr,sizeof(menStr),0);
				recv(ns,buf,sizeof(buf),0);
				menNum = buf[0]-48;
				switch(menNum){
					case 1:
						printf("%s의 대화방 목록 확인, 대화방 참여 시작\n",id); //
						send(ns,"1",1,0);
						//대화방 목록 확인, 대화방 참여
						
						//대화방 목록
						//-----------------\n
						//대화방 목록\n"
						recv(ns,che,sizeof(che),0);
						send(ns,chaRooMen1,sizeof(chaRooMen1),0);
						recv(ns,che,sizeof(che),0);
						fp = fopen(chaRooFilNam,"r");
						while(feof(fp)==0){
							fscanf(fp,"%s",buf);
							strcpy(chaRooBuf,buf);
							cp=strtok(buf,",");
							if(strcmp(id,cp)==0){
								//exist=1;
								break;
							}
						}
						//이 밑과 406번 줄은 421번 줄이 대체할것같아서 지웟음
						/*
					if(exist==0){
						printf("your chat room is not exist");
						break;
					}
					*/
						fclose(fp);
						strcpy(buf,chaRooBuf);
						cp=strtok(buf,"|");
						cp=strtok(NULL,"|");
						if(cp==NULL){
							send(ns,"@N",3,0);
							recv(ns,che,sizeof(che),0);
							continue;
						}
						else send(ns,"@Y",3,0);
						recv(ns,che,sizeof(che),0);
						while(cp!=NULL){
							send(ns,cp,strlen(cp)+1,0);
							recv(ns,che,sizeof(che),0);
							cp=strtok(NULL,"|");
						}
						send(ns,"@N",3,0);
						recv(ns,che,sizeof(che),0);
						send(ns,chaRooMen2,sizeof(chaRooMen2),0);
						//대화방 참여
						//소유자 id (finSrc)
						//대화방 초대할사람 id find2 (finDes)
						recv(ns,find2,sizeof(find2),0); //
						strcpy(buf,chaRooBuf);
						cp=strtok(buf,"|");
						cp=strtok(NULL,"|");
						if(cp==NULL){
							send(ns,"@N",3,0); //
							recv(ns,che,sizeof(che),0); //
							continue;
						}
						else send(ns,"@Y",3,0); //
						recv(ns,che,sizeof(che),0); //
						//printf("%s\n",chaRooBuf); //
						exist=0;
						cpi=1;
						while(cp!=NULL){
							//printf("%s 1234567890\n",cp); //
							i=0;
							strcpy(buf2,cp);
							//cp2[i++]=strtok(buf2,",");
							cp3=strtok(buf2,",");
							while(cp3!=NULL){
								strcpy(cp2[i],cp3);
								// printf("%s 1234567890\n",cp2[i]); //
								cp3=strtok(NULL,",");
								i++;
							}
							i--;
							
							if(strcmp(cp2[0],id)==0&&strcmp(cp2[1],find2)==0){
								exist=1;
							}
							/*
						for(int j = 0; j < i ; j++){
							if(strcmp(cp2[j],id)==0){
								for(int k = 0 ; k < i ; k++){
									if(strcmp(cp2[k],find2)==0){
										exist=1;
										break;
									}
								}
								break;
							}
							else if(strcmp(cp2[j],find2)==0){
								for(int k = 0 ; k < i ; k++){
									if(strcmp(cp2[k],id)==0){
										exist=1;
										break;
									}
								}
								break;
							}
						}
						*/
							if(exist==1) break;
							strcpy(buf,chaRooBuf);
							cp=strtok(buf,"|");
							cp=strtok(NULL,"|");
							for(int i= 0 ; i <cpi;i++){
								cp=strtok(NULL,"|");
							}
							cpi++;
						}
						if(exist==0) {
							send(ns,"@N",3,0); //
							recv(ns,che,sizeof(che),0); //
							continue;
						}
						else if(exist==1){
							send(ns,"@Y",3,0); //
						}
						else{
							printf("exist?");
						}
						recv(ns,che,sizeof(che),0); //
						send(ns,che,sizeof(che),0); //
						strcpy(id2,find2);
						// printf("%s 1234567890\n",id2); //
						//while문 탈출
						okMen=0;
						continue;
						break;
					case 2:
						printf("%s의 대화방 만들기 시작\n",id); //
						send(ns,"2",1,0);
						//대화방 만들기
						//index로 만들예정이에요 (그냥 아이디로 햇음)
						//buf = id 전달받은 것
						//소유자 id (finSrc)
						//대화방 초대할사람 id find2 (finDes)
						recv(ns,find2,sizeof(find2),0);
						//이 사람 id의 친구목록에 있나 확인 이 밑은 함수로 가능 int 로 exist 1, 0 리턴하면 될듯
						fp=fopen(friFilNam,"r");
						exist=0;
						while(feof(fp)==0){
							fscanf(fp,"%s",buf);
							strcpy(friBuf,buf);
							p=strtok(buf,",");
							if(strcmp(id,p)==0){
								p=strtok(NULL,",");
								while(p!=NULL){
									if(strcmp(find2,p)==0){
										exist=1;
										break;
									}
									p=strtok(NULL,",");
								}
								break;
							}
							if(exist==1) break;
						}
						fclose(fp);
						if(exist==0){
							send(ns,"친구목록에 없습니다.",40,0);
						}
						else if(exist==1){
							fp=fopen(chaRooFilNam,"r+");
							while(feof(fp)==0){
								fscanf(fp,"%s",buf);
								strcpy(chaRooBuf,buf);
								p=strtok(buf,",");
								if(strcmp(id,p)==0){
									break;
								}
							}
							strcpy(buf,chaRooBuf);
							exist = 1;
							//itmp = ftell(fp)-1;
							itmp = ftell(fp);
							fseek(fp,0,SEEK_END);
							filSiz = ftell(fp)-itmp;
							stmp = malloc(filSiz+1);
							memset(stmp,0,filSiz+1);
							fseek(fp,itmp,SEEK_SET);
							i=fread(stmp,filSiz,1,fp);
							fseek(fp,itmp,SEEK_SET);
							fwrite("|",1,1,fp);
							fwrite(id,strlen(id),1,fp);
							fwrite(",",1,1,fp);
							fwrite(find2,strlen(find2),1,fp);
							fwrite(stmp,filSiz,1,fp);
							free(stmp);
							fclose(fp);
							
							//상대방에게도 대화방 생성
							fp = fopen(chaRooFilNam,"r+");
							while(feof(fp)==0){
								fscanf(fp,"%s",buf);
								strcpy(chaRooBuf,buf);
								p=strtok(buf,",");
								if(strcmp(find2,p)==0){
									break;
								}
							}
							strcpy(buf,chaRooBuf);
							exist = 1;
							//itmp = ftell(fp)-1;
							itmp = ftell(fp);
							fseek(fp,0,SEEK_END);
							filSiz = ftell(fp)-itmp;
							stmp = malloc(filSiz+1);
							memset(stmp,0,filSiz+1);
							fseek(fp,itmp,SEEK_SET);
							i=fread(stmp,filSiz,1,fp);
							fseek(fp,itmp,SEEK_SET);
							fwrite("|",1,1,fp);
							fwrite(find2,strlen(find2),1,fp);
							fwrite(",",1,1,fp);
							fwrite(id,strlen(id),1,fp);
							fwrite(stmp,filSiz,1,fp);
							free(stmp);
							fclose(fp);
							
							strcpy(chaRooPipNam,datDir);
							strcat(chaRooPipNam,id);
							strcat(chaRooPipNam,",");
							strcat(chaRooPipNam,find2);
							mkfifo(chaRooPipNam, 0666);
							
							strcpy(chaRooPipNam,datDir);
							strcat(chaRooPipNam,find2);
							strcat(chaRooPipNam,",");
							strcat(chaRooPipNam,id);
							mkfifo(chaRooPipNam, 0666);
							
							ok=1;
							send(ns,"대화방 생성 완료!",30,0);
						}
						break;
					case 3:
						printf("%s의 친구 목록 시작\n",id); //
						send(ns,"3",1,0);
						//친구 목록
						recv(ns,che,sizeof(che),0);
						fp = fopen(friFilNam,"r");
						while(feof(fp)==0){
							fscanf(fp,"%s",buf);
							strcpy(friBuf,buf);
							cp=strtok(buf,",");
							if(strcmp(id,cp)==0){
								//exist=1;
								break;
							}
						}
						fclose(fp);
						strcpy(buf,friBuf);
						cp=strtok(buf,",");
						cp=strtok(NULL,",");
						if(cp==NULL) {
							send(ns,"@N",3,0);
							recv(ns,che,sizeof(che),0);
							continue;
						}
						else send(ns,"@Y",3,0);
						recv(ns,che,sizeof(che),0);
						while(cp!=NULL){
							send(ns,cp,strlen(cp)+1,0);
							cp=strtok(NULL,",");
							recv(ns,che,sizeof(che),0);
						}
						send(ns,"@N",3,0);
						ok=1;
						break;
					case 4:
						printf("%s의 친구 추가 시작\n",id); //
						send(ns,"4",1,0);
						// 친구 추가 구현
						recv(ns,find2,sizeof(find2),0);
						//가입된 id가 존재하는지 확인
						exist = 0;
						fp = fopen(memFilNam,"r");
						while(feof(fp)==0){
							fscanf(fp,"%s",buf);
							cp=strtok(buf,",");
							if(strcmp(find2,cp)==0){
								exist = 1;
								break;
							}
						}
						fclose(fp);
						if(exist==0){
							send(ns,"@N",3,0);
							recv(ns,che,sizeof(che),0);
							continue;
						}
						else if(exist==1) {
							send(ns,"@Y",3,0);
						}
						// 친구 추가
						fp=fopen(friFilNam,"r+");
						exist=1;
						ok=0;
						while(feof(fp)==0){
							fscanf(fp,"%s",buf);
							strcpy(friBuf,buf);
							p=strtok(buf,",");
							if(strcmp(id,p)==0){
								p=strtok(NULL,",");
								while(p!=NULL){
									printf("p = %s 1\n",p); //
									if(strcmp(find2,p)==0){
										exist = 1;
										ok = 1;
										break;
									}
									p=strtok(NULL,",");
								}
								if(ok==1) break;
								
								//파일 중간에 친구 끼워넣기
								//함수도 가능할듯 fun(char *id, char *find2, char *friFilNam){}
								itmp = ftell(fp)-1;
								fseek(fp,0,SEEK_END);
								filSiz = ftell(fp)-itmp;
								stmp = malloc(filSiz+1);
								memset(stmp,0,filSiz+1);
								fseek(fp,itmp,SEEK_SET);
								i=fread(stmp,filSiz,1,fp);
								fseek(fp,itmp,SEEK_SET);
								fwrite(",",1,1,fp);
								fwrite(find2,strlen(find2),1,fp);
								fwrite(stmp,filSiz,1,fp);
								free(stmp);
								fclose(fp);
								
								//상대방에게도 친구 추가
								fp=fopen(friFilNam,"r+");
								while(feof(fp)==0){
									fscanf(fp,"%s",buf);
									strcpy(friBuf,buf);
									p=strtok(buf,",");
									if(strcmp(find2,p)==0){
										break;
									}
								}
								p=strtok(NULL,",");
								itmp = ftell(fp)-1;
								fseek(fp,0,SEEK_END);
								filSiz = ftell(fp)-itmp;
								stmp = malloc(filSiz+1);
								memset(stmp,0,filSiz+1);
								fseek(fp,itmp,SEEK_SET);
								i=fread(stmp,filSiz,1,fp);
								fseek(fp,itmp,SEEK_SET);
								fwrite(",",1,1,fp);
								fwrite(id,strlen(id),1,fp);
								fwrite(stmp,filSiz,1,fp);
								free(stmp);
								fclose(fp);
								
								exist=0;
								break;
							}
						}
						fclose(fp);
						recv(ns,che,sizeof(che),0);
						if(exist==1){
							send(ns,"@N",3,0);
						}
						else if(exist==0){
							send(ns,"@Y",3,0);
						}
						recv(ns,che,sizeof(che),0);
						//continue;
						ok=1;
						break;
					default:
						send(ns,"@N",2,0);
						break;
				}
				recv(ns,che,sizeof(che),0);
			}
			
			//대화 시작
			//파이프 사용해서 부모가 클라이언트에게 send, 자식이 클라이언트에게서 recv
			strcpy(chaRooPipNam,datDir);
			strcat(chaRooPipNam,id);
			strcat(chaRooPipNam,",");
			strcat(chaRooPipNam,id2);
			
			strcpy(chaRooPipNam2,datDir);
			strcat(chaRooPipNam2,id2);
			strcat(chaRooPipNam2,",");
			strcat(chaRooPipNam2,id);
			
			forkN=fork();
			if(forkN>0){
				//
				if ((pd = open(chaRooPipNam2, O_RDONLY)) == -1){
					perror("open");
					exit(1);
				}
				while(1){
					n = read(pd, buf, sizeof(buf));
					if (send(ns, buf, 256, 0) == -1){
						perror("send");
						exit(1);
					}
					/*
					if(strstr(buf, "QUIT\n")!=0){
						break;
					}
					*/
					printf("%s: %s",id2,buf); //
				}
				kill(forkN, SIGKILL);
				close(pd);
				close(sd);
				close(ns);
				exit(0);
			}
			else if(forkN==0)
			{
				close(sd); // 대화에 문제가 생긴다면 이거 주석해라 (왜냐면 주석풀고 확인안해봤다)
				if ((pd = open(chaRooPipNam, O_WRONLY)) == -1){
					perror("open");
					exit(1);
				}
				while(1){
					if (recv(ns, buf, 256, 0) == -1){
						perror("recv");
						exit(1);
					}
					/*
					if(ns == 0) {
						break;
					}
					if(strstr(buf, "QUIT\n")!=0) {
						break;
					}
					*/
					n = write(pd, buf, sizeof(buf));
					printf("%s: %s",id,buf); //
				}
				kill(getppid(), SIGKILL);
				close(sd);
				close(pd);
				close(ns);
				exit(0);
			}
		}
		close(ns);
	}
	close(sd);
	fclose(fp);
	
	return 0;
}