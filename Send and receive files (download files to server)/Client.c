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

   /* ���� ������ ���� ���� ���� */  
   sd=socket(PF_INET, SOCK_STREAM, 0);     
   if(sd == -1)  
      error_handling("socket() error");  

   memset(&serv_addr, 0, sizeof(serv_addr));  
   serv_addr.sin_family=AF_INET;  
   serv_addr.sin_addr.s_addr=inet_addr(argv[1]);  
   serv_addr.sin_port=htons(atoi(argv[2]));  

   if( connect(sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1 )  
      error_handling("connect() error!");  

   //ã�� ������ �Է���
   printf(" ã�� ���� �̸� �Է� : ");
   fgets(filename, 10, stdin);
   filename[strlen(filename)-1] = 0; //�Ǹ����� �ι��ڳְ�
   write(sd, filename, 5);   //�����̸��� �������� ����
   
   //�������� ���� ���Ͻ�ũ���� ����
   len = read(sd, &nCheck, sizeof(nCheck)); //���Ͻ�ũ���� �ް�
   if( nCheck == -1) //������ ������ ����
      error_handling("NoFile!");

   //���� ������ 
   printf(" �ٸ��̸����� ������ �̸� �Է� : "); 
   fgets(filename, 10, stdin);
   filename[strlen(filename)-1] = 0;

   //���Ͽ���
   fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

   // �����͸� �����ϰ� ���Ͽ� ���� 
   while( (len=read(sd, buf, BUFSIZE )) != 0 )  
   {  
      write(fd, buf, len);   
   }  

   // �߹޾Ҵٴ� �޼��� ����
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
