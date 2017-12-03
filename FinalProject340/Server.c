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
#include <stdlib.h>
#include <sqlite3.h> 
 
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: \n", (const char*)data);
   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}

int main(){

    sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";

   
    
    char auth_code[100] = "307";
    char str[100];
    int listen_fd, comm_fd;
    char message[100];
 
    struct sockaddr_in servaddr;
 
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 
    bzero( &servaddr, sizeof(servaddr));
 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(22000);
 
    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
 
    listen(listen_fd, 10);
 
    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
 
    while(1){
 
        bzero( str, 100);
 
        // reads client input
        read(comm_fd,str,100);

        // will give connection if client has correct code
        printf("%s\n", str);
        printf("%s\n", auth_code);
        printf("%i\n", strcmp(str, auth_code));


        // Correct code
        if (strcmp(str, auth_code) == 10){
 
          //printf("Access Granted");
          strncpy(message, "Access Granted\n", sizeof(message) +1);
          write(comm_fd, message, strlen(message)+1);
          printf("Correct auth_code provided by client\n");
          
          //Database request
          printf("gathering database request for: ");
          read(comm_fd, str, 100);
          printf("%s\n", str);
          
          //DATABASE
          /* Open database */
          rc = sqlite3_open("database.db", &db);

          if( rc ) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            //return(0);
          } else {
            fprintf(stdout, "Opened database successfully\n");
          }
          // Tests inputs from table
          sql = "SELECT * FROM PASSWORDS";

          /* Execute SQL statement */
          rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

          if( rc != SQLITE_OK ) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
          } else {
            fprintf(stdout, "Operation done successfully\n");
          }

          strncpy(message, "fetching\n", sizeof(message) +1);
          write(comm_fd, message, strlen(message)+1);





        }


        // incorrect code kicks client off the network
        else{

          printf("Terminating Client socket\n");

          strncpy(message, "Access Denied\n", sizeof(message) +1);
          write(comm_fd, message, strlen(message)+1);
          break;

        }
 
    }
} 