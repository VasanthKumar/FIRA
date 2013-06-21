#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFFER_SIZE 18    												 // Increase this as required

void error(const char *msg)
{ 
    perror(msg);
    exit(0);
}

int establishConnection(char* bot_address, int bot_portno)
{
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd== -1)
    {
        printf("Could not create socket");
    }
    serv_addr.sin_addr.s_addr = inet_addr(bot_address);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( bot_portno );
 
    //Connect to remote server
    if (connect(sockfd , (struct sockaddr *)&serv_addr , sizeof(serv_addr)) < 0)
    {
        puts("connect error");
        return 1;
    }
        printf("Connected to %s on port %d\n",bot_address,bot_portno); 

    return sockfd;
}

char* tcpRead( int socket_descriptor, char *buffer)
{
    bzero(buffer,BUFFER_SIZE);
    int n = read(socket_descriptor,buffer,BUFFER_SIZE-1);
    if (n == -1)
	{
		error("ERROR reading from socket");
    }
    return buffer;

}

int tcpWrite(int socket_descriptor,char* message)
{
    int n = write(socket_descriptor,message,strlen(message));
    if (n < 0) 
         error("ERROR writing to socket");
  return n;
}

int init_wifi(int bot_port,char* bot_address)
{
	int bot_sockfd = establishConnection(bot_address,bot_port);
	char *bot_buffer = (char *)malloc(BUFFER_SIZE*sizeof(char));
		  
	/* Initialization per bot */     
	do	
	{
		tcpRead(bot_sockfd,bot_buffer);
		printf("got: %s ",bot_buffer); 
	}while(bot_buffer[0]!='M');

	return bot_sockfd;
}

void close_socket(int bot1_sockfd)
{
	close(bot1_sockfd);
}

int main()
{
	init_wifi(9750,"192.168.0.1");
	return 0;
}



