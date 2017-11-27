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


#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

int main()
{
 struct sockaddr_in sock_var;
 int clientFileDiscriptor=socket(AF_INET,SOCK_STREAM,0);
 char str_clnt[20],str_ser[20];
 
 sock_var.sin_addr.s_addr=inet_addr("127.0.0.1");
 sock_var.sin_port=3000;
 sock_var.sin_family=AF_INET;
 
 if(connect(clientFileDiscriptor,(struct sockaddr*)&sock_var,sizeof(sock_var))>=0)
 {
  printf("Connected to server %dn",clientFileDiscriptor);
  printf("nEnter Srting to send");
  scanf("%s",str_clnt);
  write(clientFileDiscriptor,str_clnt,20);
  read(clientFileDiscriptor,str_ser,20);
  printf("String from Server: %s",str_ser);
  close(clientFileDiscriptor);
 }
 else{
  printf("socket creation failed");
 }
 return 0;
}