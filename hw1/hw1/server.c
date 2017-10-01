
// -----------------------------------
// CSCI 340 - Operating Systems
// Fall 2017
// server.h header file
// Homework 1
//
// Modified by Cass outlaw
// Sept, 22, 2017
//
// -----------------------------------

#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "server.h"

// ------------------------------------
// Function prototype that creates a socket and 
// then binds it to the specified port_number 
// for incoming client connections
// 
//
// Arguments:	port_number = port number the server 
//				socket will be bound to.
//
// Return:      Socket file descriptor (or -1 on failure)
//

int bind_port( unsigned int port_number ) {

	// -------------------------------------
	// NOTHING TODO HERE :)
	// -------------------------------------
	// Please do not modify

	int socket_fd;
	int set_option = 1;

    struct sockaddr_in server_address;
     
    socket_fd = socket( AF_INET, SOCK_STREAM, 0 );

    setsockopt( socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&set_option, sizeof( set_option ) );

    if (socket_fd < 0) return FAIL;

    bzero( (char *) &server_address, sizeof(server_address) );

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons( port_number );

    if ( bind( socket_fd, (struct sockaddr *) &server_address, sizeof(server_address) ) == 0 ) {

    	return socket_fd;

    } else {

    	return FAIL;

    }

} // end bind_port function

// ------------------------------------
// Function prototype that accepts a client
// socket connection
// 
//
// Arguments:	server file descriptor
//
// Return:      Termination status of client
//				( 0 = No Errors, -1 = Error )
//
int accept_client( int server_socket_fd ) {

	int exit_status = OK;

	int client_socket_fd = -1;

	socklen_t client_length; 

	struct sockaddr_in client_address;

	char request[512];

	client_length = sizeof( client_address );

    client_socket_fd = accept( server_socket_fd, (struct sockaddr *) &client_address, &client_length );
		
	// -------------------------------------
	// TODO:
	// -------------------------------------
	// Modify code to fork a child process
	// -------------------------------------
	

	if ( client_socket_fd >= 0 ) {
		
		bzero( request, 512 );
		
		read( client_socket_fd, request, 511 );


		if ( DEBUG ) printf("Here is the http message:\n%s\n\n", request );
		
		// -------------------------------------
		// TODO:
		// -------------------------------------
		// Generate the correct http response when a GET or POST method is sent
		// from the client to the server.
		// 
		// In general, you will parse the request character array to:
		// 1) Determine if a GET or POST method was used
		// 2) Then retrieve the key/value pairs (see below)
		// -------------------------------------
		
		/*
		 ------------------------------------------------------
		 GET method key/values are located in the URL of the request message
		 ? - indicates the beginning of the key/value pairs in the URL
		 & - is used to separate multiple key/value pairs 
		 = - is used to separate the key and value
		 
		 Example:
		 
		 http://localhost/?first=brent&last=munsell
		 
		 two &'s indicated two key/value pairs (first=brent and last=munsell)
		 key = first, value = brent
		 key = last, value = munsell
		 ------------------------------------------------------
		 */

		//old test case
		// initializes the body of the html page, without the closing tags.
		char entity_body[512] = "<html><body><h2>CSCI 340 (Operating Systems) Project 1</h2><table border=1 width=\"50%\"><tr><th>Key</th><th>Value</th></tr>";


		// determines if the request is GET, by looking at first character
		if(request[0] == 'G') {

			// debug testing
			//printf("its a GET \n");

	// to comment out above below for testing
	//	/*	
			// initilizes character pointers for string traversal
			// initializes the body of the html page, without the closing tags.
			//char entity_body[512] = "<html><body><h2>CSCI 340 (Operating Systems) Project 1</h2><table border=1 width=\"50%\"><tr><th>Key</th><th>Value</th></tr>";
		
			char* p1;
			char* p2;

			p1 = request;
			p2 = request;
			
			
			// lenth of the request, for traversal
			size_t str_len = strlen(request);

			// travers the request untill the starting character of the key value pairs is found
			// this loop terminates when the start of the key pairs is found 
			// this loop also initilizes the values of p1 and p2 we need to return the html table
			unsigned long int i = 0;
			for(i=0; i < str_len; i++){

				//debug testing
				//printf(" start loop \n");

				// traverse to the starting variable
				if( (*p1)=='?') {

					// initilize frist value to null
					(*p1) = '\0';

					//set p2 to the next value after p1
					p2 = ++p1;

					//debug testing
					//printf(" FOUND START POINT \n");

					//terminate the loop
					i = str_len;

				}

				// loop itteration
				p1++;
			}

			// this loop does all the traversal between key value pairs
			// this loop also adds html tags to the request
			for(i=0; i < str_len; i++){
				// traverse to first delimeter
				if((*p1) == '='){

					// debug testing
					//printf(" FOUND = \n");

					//set value at p1 to null
					(*p1) = '\0';

					// add new row and cell tags to the table
					// traverse p2 untill p1
					strcat(entity_body, "<tr>");
					strcat(entity_body, "<td>");
					// add p2 to the table
					strcat(entity_body, p2);
					// close cell tags
					strcat(entity_body, "</td>");

					//re-initilize p2 to the next key/value pair
					p2 = ++p1;

					//debug testing
					//printf("%s\n" , entity_body);

				}

				// traverse to the & delimiter
				if((*p1) == '&'){

					// debug testing
					//printf(" FOUND & ");

					//set value of p1 to null to end p2
					(*p1) = '\0';
					
					//add cell tags
					strcat(entity_body, "<td>");
					// add p2 to the table
					strcat(entity_body, p2);
					strcat(entity_body, "</td>");
					
					// close the row tags created by the key vaule pair that was just added to the table
					strcat(entity_body, "</tr>");

					//re-initilize the value of p2 to prepare for more traversals
					p2= ++p1;
					
					//debug testing
					//printf("%s\n" , entity_body);

				}

				//debug test
				//printf("got to the end of the loop");

				//LOOP ITTERATION
				p1++;
				

				//post itteration check to see if the key value pairs have ended
				if((*p1) == ' '){

					//set the empty space to null and add the final p2 tag to the table
					(*p1) = '\0';
					strcat(entity_body, "<td>");
					strcat(entity_body, p2);
					strcat(entity_body, "</td>");
					strcat(entity_body, "</tr>");

					//terminate the loop
					i = str_len;
				}

				//debug testing
				//printf(" end loop \n");
			}
			

			//debug testing
			//printf("%s" , entity_body);
	// to comment out above block for testing
	// 	*/

		

		}


				
	
		/*
		 ------------------------------------------------------
		 POST method key/value pairs are located in the entity body of the request message
		 ? - indicates the beginning of the key/value pairs
		 & - is used to delimit multiple key/value pairs 
		 = - is used to delimit key and value
		 
		 Example:
		 
		 first=brent&last=munsell
		 
		 two &'s indicated two key/value pairs (first=brent and last=munsell)
		 key = first, value = brent
		 key = last, value = munsell
		 ------------------------------------------------------
		 */
		if(request[0] == 'P'){

			//debug testing
		 	//printf("its a Post\n");

		 	//char entity_body[512] = "<html><body><h2>CSCI 340 (Operating Systems) Project 1</h2><table border=1 width=\"50%\"><tr><th>Key</th><th>Value</th></tr>";


		 	char* p1;
			char* p2;

			p1 = request;
			p2 = request;
			
			
			// lenth of the request, for traversal
			size_t str_len = strlen(request);

			// travers the request untill the starting character of the key value pairs is found
			// this loop terminates when the start of the key pairs is found 
			// this loop also initilizes the values of p1 and p2 we need to return the html table
			unsigned long int i = 0;
			unsigned long int x = 0;
			for(i=0; i < str_len; i++){

				//debug testing
				//printf(" start loop \n");


				// traverse to the starting variable
				if( (*p1)== '\n') {
					//debug test
					//printf("FOUND LINE BREAK \n");

					x++;
					if(x == 11){

					// initilize frist value to null
					(*p1) = '\0';

					//set p2 to the next value after p1
					p2 = ++p1;

					//debug testing
					//printf(" FOUND START POINT \n");

					//terminate the loop
					i = str_len;

					
					}



				}

				// loop itteration
				p1++;
			}

			// this loop does all the traversal between key value pairs
			// this loop also adds html tags to the request
			for(i=0; i < str_len; i++){

				p1++;
				// traverse to first delimeter
				if((*p1) == '='){

					// debug testing
					//printf(" FOUND = \n");

					//set value at p1 to null
					(*p1) = '\0';

					// add new row and cell tags to the table
					// traverse p2 untill p1
					strcat(entity_body, "<tr>");
					strcat(entity_body, "<td>");
					// add p2 to the table
					strcat(entity_body, p2);
					// close cell tags
					strcat(entity_body, "</td>");

					//re-initilize p2 to the next key/value pair
					p2 = ++p1;

					//debug testing
					printf("%s\n" , entity_body);

				}

				// traverse to the & delimiter
				if((*p1) == '&'){

					// debug testing
					//printf(" FOUND & ");

					//set value of p1 to null to end p2
					(*p1) = '\0';
					
					//add cell tags
					strcat(entity_body, "<td>");
					// add p2 to the table
					strcat(entity_body, p2);
					strcat(entity_body, "</td>");
					
					// close the row tags created by the key vaule pair that was just added to the table
					strcat(entity_body, "</tr>");

					//re-initilize the value of p2 to prepare for more traversals
					p2= ++p1;
					
					//debug testing
					printf("%s\n" , entity_body);

				}

				//debug test
				//printf("got to the end of the loop");

				//LOOP ITTERATION
				
				

				//post itteration check to see if the key value pairs have ended
				if((*p1) == '\0'){

					//set the empty space to null and add the final p2 tag to the table
					(*p1) = '\0';
					strcat(entity_body, "<td>");
					strcat(entity_body, p2);
					strcat(entity_body, "</td>");
					strcat(entity_body, "</tr>");

					//terminate the loop
					i = str_len;
				}

				//debug testing
				//printf(" end loop \n");
			}

			//debug testing
			//printf("%s" , entity_body);
	// to comment out above block for testing
	// 	*/

		}
		
		char endHtml[100] = "</table></body></html>";
		char* end = endHtml;
		strcat( entity_body, end);


		
		
		
		
		char response[512];
		sprintf( response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", (int)strlen( entity_body ), entity_body );
		
		if ( DEBUG ) printf( "%s\n", response );
		
    	write( client_socket_fd, response, strlen( response ) );
		
		close( client_socket_fd );
		
		
	} else {
		
		exit_status = FAIL;
		
	}
	
	if ( DEBUG ) printf("Exit status = %d\n", exit_status );
	
	return exit_status;
	
} // end accept_client function
