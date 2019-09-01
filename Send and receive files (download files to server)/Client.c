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
   int fd;  
   int sd;  
   int nCheck = 0;

   char buf[BUFSIZE];
   char cbuf[BUFSIZE];  
   char filename[BUFSIZE];
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
   printf(" 찾을 파일 이름 입력 : ");
   fgets(filename, 10, stdin);
   filename[strlen(filename)-1] = 0; //맨마지막 널문자넣고
   write(sd, filename, 5);   //파일이름을 서버에게 전달
   
   //서버에서 보낸 파일스크립터 수신
   len = read(sd, &nCheck, sizeof(nCheck)); //파일스크립터 받고
   if( nCheck == -1) //파일이 없으면 종료
      error_handling("NoFile!");

   //파일 있으면 
   printf(" 다른이름으로 저장할 이름 입력 : "); 
   fgets(filename, 10, stdin);
   filename[strlen(filename)-1] = 0;

   //파일오픈
   fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

   // 데이터를 수신하고 파일에 저장 
   while( (len=read(sd, buf, BUFSIZE )) != 0 )  
   {  
      write(fd, buf, len);   
   }  

   // 잘받았다는 메세지 전달
   write(sd, "Thank you\n", 10);     
   close(fd);  
   close(sd);  
   return 0;  
}  

void error_handling(char *message)  
{  
   fputs(message, stderr);  
   fputc('\n', stderr);  
   exit(1);  
}
