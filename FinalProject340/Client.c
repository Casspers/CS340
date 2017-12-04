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
        printf("Enter Authenticaton Key: ");
        fgets(sendline,100,stdin); /*stdin = 0 , for standard input */
        write(sockfd,sendline,strlen(sendline)+1);

        read(sockfd,recvline,100);
	    printf("%s",recvline);

	    //printf("%i\n", strcmp(recvline, "Access Granted\n"));

	    if( strcmp(recvline, "Access Granted\n") == 0 ){
	    	// Accept logic
	    	printf("Type Command:\nget - to get passwords\nedit - edit database with SQL commands\nexit - to exit\nhelp - for list of commands\n\n");
	    	while(1){

		    	printf("Command: ");
		    	fgets(sendline, 100,stdin);
		    	write(sockfd, sendline, strlen(sendline)+1);
		    	//printf("%i",strcmp(sendline,"insert"));


		    	if(strcmp(sendline, "get") == 10){
		    		printf("By Default users 1-4 are created\n");
		    		printf("\nPlease Specify one of Users by name:\nexample - User1\n");
		    		printf("\nEnter get Command: ");
		    		fgets(sendline, 100,stdin);
		    		write(sockfd, sendline, strlen(sendline)+1);
		    		
		    		if(strcmp(sendline, "exit") != 10 ){
			    	
			    		// if user does not exit
			    		read(sockfd, recvline, 100);

			    		if(strcmp(recvline, "Invalid Input") == 10){
			    			printf("%s",recvline);

			    		}else{
			    		//read(sockfd, recvline, 100);
		    			printf("For User: %sPassword is: %s\n", sendline, recvline);
		    			printf("\n-Exit from get Commands-\n\n");
		    			}
		    		}else{
		    			printf("\n-Exit from get Commands-\n\n");
		    			goto end;
		    		}
    	
		    	}else if(strcmp(sendline, "edit") == 10){

		    		printf("\n---COMMANDS MUST BE IN SQL FORMAT TO BE VALID!---\nEample: to change password for user 1\n");
		    		printf("UPDATE PASSWORDS set PASS = 'newPassord' where USER = 'User1'\n");
		    		printf("or\nINSERT INTO PASSWORDS (USER, PASS) VALUES('NewUser','NewPassword')\n");
		    		printf("or\nTo retrive new users data:\nSELECT PASSWORDS.PASS from PASSWORDS where PASSWORDS.USER = 'username'\n");
		    		printf("Command: ");
		    		fgets(sendline,100, stdin);
		    		if(strcmp(sendline,"exit") != 0){
		    			write(sockfd,sendline,strlen(sendline)+1);
		    	
			    		// if user does not exit
			    		read(sockfd, recvline, 100);
			    		//printf("The new Password is: %s\n", sendline);
			    		printf("\n%s", recvline);
			    		printf("\n-Exit from edit Commands-\n\n");
		    		}else{
		    			printf("\n-Exit from edit Commands-\n\n");

		    			goto end;
		    		}

		    	
		    	}else if(strcmp(sendline, "exit") == 10){
		    		goto end;
		    	
		    	}else if(strcmp(sendline, "help") == 10){
		    		printf("Acceptable Commands:\nget - to get passwords\nupdate - to modify passwords\nexit - to exit\nhelp - for list of commands\n");
		    	
		    	}else{
		    		printf("Not vaid Command, Please try not to be stupid\n");
		    	}
	    	}
 
	    	end: break;


	    }
    }
 
}