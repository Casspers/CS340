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
	    	printf("Type Command:\nget - to get passwords\nupdate - to modify passwords\nexit - to exit\nhelp - for list of commands\n");
	    	while(1){

		    	printf("Command: ");
		    	fgets(sendline, 100,stdin);
		    	write(sockfd, sendline, strlen(sendline)+1);
		    	printf("%i",strcmp(sendline,"update"));


		    	if(strcmp(sendline, "get") == 10){
		    		printf("Enter database username you want a password for: ");
		    		fgets(sendline, 100,stdin);
		    		//write(sockfd, sendline, strlen(sendline)+1);

		    		//read(sockfd, recvline, 100);
		    		//printf("%s", recvline);

		    	}
		    	if(strcmp(sendline, "update") == 10){
		    		printf("Enter database username you want to update: ");
		    		fgets(sendline, 100,stdin);
		    		//write(sockfd, sendline, strlen(sendline)+1);

		    		//read(sockfd, recvline, 100);
		    		//printf("%s", recvline);

		    	}
		    	if(strcmp(sendline, "exit") == 0){
		    		break;
		    	}
		    	if(strcmp(sendline, "help") == 0){
		    		printf("Acceptable Commands:\nget - to get passwords\nupdate - to modify passwords\nexit - to exit\nhelp - for list of commands\n");
		    	}
		    	else{
		    		printf("Not vaid Command\n");
		    	}
	    	}
 
	    	

	    }
    }
 
}