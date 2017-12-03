/*
**************************************
*
*   Client.c
*
*   CS340 Final Project
*
*       Description: Password storage
*       for multipel users implementing 
*       a MultiTreaded server, Network 
*       layers, and mutex locks
*
*   Team Name: Neam Tame
*   Cass Outlaw
*   Zach Coffman
*   
*
**************************************
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include<string.h>


 
int main(int argc,char **argv)
{
    int sockfd,n;
    char sendline[100];
    char recvline[100];
    struct sockaddr_in servaddr;
 
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof servaddr);
 
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(22000);
 
    inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));
 
    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
 
    while(1){

        bzero( sendline, 100);
        bzero( recvline, 100);

        // Verify Client
        printf("Enter Authenticaton code: ");
        fgets(sendline,100,stdin); /*stdin = 0 , for standard input */
        write(sockfd,sendline,strlen(sendline)+1);

        read(sockfd,recvline,100);
	    printf("%s",recvline);

	    //printf("%i\n", strcmp(recvline, "Access Granted\n"));

	    if( strcmp(recvline, "Access Granted\n") == 0 ){
	    	// Accept logic

	    	printf("Enter database username you want a password for: ");
	    	fgets(sendline, 100,stdin);
	    	write(sockfd, sendline, strlen(sendline)+1);

	    	read(sockfd, recvline, 100);
	    	printf("%s", recvline);

	    }
    }
 
}