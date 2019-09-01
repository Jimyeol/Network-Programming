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
	int serv_sd;  
	int clnt_sd;  
	FILE* fd;  
	char buf[BUFSIZE];
	char cbuf[BUFSIZE];
	int recv_len = 0;
	int str_len = 0;
	int recv_cnt = 0;
	int read_cnt = 0;

	int nCheckData = 0;

	struct sockaddr_in serv_addr;  
	struct sockaddr_in clnt_addr;  
	int clnt_addr_size;  
	int len;  

	if(argc!=2){  
		printf("Usage : %s <port>\n", argv[0]);  
		exit(1);  
	}  

	serv_sd=socket(PF_INET, SOCK_STREAM, 0);     
	if(serv_sd == -1)  
		error_handling("socket() error");  

	memset(&serv_addr, 0, sizeof(serv_addr));  
	serv_addr.sin_family=AF_INET;  
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);  
	serv_addr.sin_port=htons(atoi(argv[1]));  

	if( bind(serv_sd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 )  
		error_handling("bind() error");  

	if( listen(serv_sd, 10)==-1 )  
		error_handling("listen() error");  

	clnt_addr_size=sizeof(clnt_addr);      
	clnt_sd=accept(serv_sd, (struct sockaddr*)&clnt_addr,&clnt_addr_size);  
	if(clnt_sd==-1)  
		error_handling("accept() error");

	//�������� ���� ���Ͻ�ũ���� ����
	len = read(clnt_sd, &nCheckData, sizeof(nCheckData)); //���Ͻ�ũ���� �ް�
	if( nCheckData == NULL) //������ ������ ����
		error_handling("NoFile!");


	//�����̸��� Ŭ�󿡰� ����
	len = read(clnt_sd, cbuf, BUFSIZE);
	cbuf[strlen(cbuf)] = 0; //�Ǹ����� �ι��ڳְ�
	printf("cbuf[strlen(cbuf)] : %s\n", cbuf);


	//�����̸����� ����
	fd = fopen(cbuf, "w");
	printf("fd = %d\n", fd);


	while(1)
	{
		read_cnt=read(clnt_sd, buf, BUFSIZE );
		if(read_cnt<BUFSIZE)
		{
			fwrite((void*)buf, 1, read_cnt, fd); 
			break;
		}
		fwrite((void*)buf, 1, read_cnt, fd); 
	}
	/*while( (len=read(clnt_sd, buf, BUFSIZE )) != 0 )  
	{  
		fwrite((void*)buf, 1, len, fd); 
	}
	printf("len = %d \n", len);*/

	fclose(fd);
	close(clnt_sd);  
	close(serv_sd);
	return 0;  
}  

void error_handling(char *message)  
{  
	fputs(message, stderr);  
	fputc('\n', stderr);  
	exit(1);  
}