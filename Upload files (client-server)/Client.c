#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <arpa/inet.h>  
#include <fcntl.h>  
#include <sys/types.h>  
#include <sys/socket.h>  

#define BUFSIZE 1024

void error_handling(char *message);  

int main(int argc, char **argv)  
{  
   FILE* fd;  
   int sd;  
   int nCheck = 0;
   int recv_len = 0;
   int str_len = 0;

   char buf[BUFSIZE];
   char cbuf[BUFSIZE];  
   char filename[BUFSIZE];
   char uploadname[BUFSIZE];
   int len;  
   struct sockaddr_in serv_addr;  

   if(argc!=3){  
      printf("Usage : %s <IP> <port>\n", argv[0]);  
      exit(1);  
   }  


   if(fd == -1)  
      error_handling("File open error");  

   /* 서버 접속을 위한 소켓 생성 */  
   sd=socket(PF_INET, SOCK_STREAM, 0);     
   if(sd == -1)  
      error_handling("socket() error");  

   memset(&serv_addr, 0, sizeof(serv_addr));  
   serv_addr.sin_family=AF_INET;  
   serv_addr.sin_addr.s_addr=inet_addr(argv[1]);  
   serv_addr.sin_port=htons(atoi(argv[2]));  

   if( connect(sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1 )  
      error_handling("connect() error!");  

   //찾을 파일을 입력함
   printf(" 업로드할 파일 이름 : ");
   fgets(uploadname, 10, stdin);
   uploadname[strlen(uploadname)-1] = 0; //맨마지막 널문자넣고
   fd = fopen( uploadname, "r");
   write(sd, &fd, sizeof(fd)); //파일스크립트 전달
   if(fd == NULL)   //파일이 없다면 
   {
      close(fd); //파일 다시닫고
      error_handling("No File");  //파일 없으면 탈락
   }

   //파일 있으면 
   printf(" 다른이름으로 저장할 이름 입력 : "); 
   fgets(filename, 10, stdin);
   filename[strlen(filename)-1] = 0;
   write(sd, filename, 5);   //파일이름을 서버에게 전달

   //서버에게 파일 데이터 전송 
   while( (len=fread((void*)buf, 1, BUFSIZE, fd)) != 0 )  
   {
      write(sd, buf, len);
   }

   

   fclose(fd);  
   close(sd);

   return 0;  
}  

void error_handling(char *message)  
{  
   fputs(message, stderr);  
   fputc('\n', stderr);  
   exit(1);  
}