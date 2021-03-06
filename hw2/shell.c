// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included


// Modified by Cass Outlaw.


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include "shell.h"

// --------------------------------------------
// Currently only two builtin commands for this
// assignment exist, however in future, more could 
// be added to the builtin command array.
// --------------------------------------------
const char* valid_builtin_commands[] = {"cd", "exit", NULL};


void parse( char* line, command_t* p_cmd ) {

	// ----------------------------------------
	// TODO: you fully implement this function

	// Get rid of those dirty spaces
	// this token thing is pretty nifty
	// token becomes the first thing entered up to the first space
	/*

	char* token = strtok(line, " ");

	// if token is null, then no command was entered
	if(token = NULL){
		p_cmd->path = NULL;
		p_cmd->argv = NULL;
		p_cmd->argc = 0;
	}

	// otherwise you have to do things

	else{

		// make cmd the right size
		char cmd[sizeof(token)];

		// we gona take that there token and make it the cmd
		//strcpy(dest,src)
		strcpy(cmd, token);

		// more token stuff
		while(token != NULL){
			p_cmd->[count] = token;
			count ++;

			// learned this neat trick too
			// will reset the value of token starting form the 
			// NULL terminator up to the next space.
			token = strtok(NULL, " ");
		}


		int i = 0;

	}




*/

} // end parse function


int execute( command_t* p_cmd ) {

	
	// ----------------------------------------
	// TODO: you fully implement this function

	// -------------------------------------------
	// Added a default return statement
	// however this needs to be changed 
	// to reflect the correct return 
	// value described in the header file

	if((*p_cmd).argc > 0){


		if(fork() == 0){

			execv( p_cmd->path, p_cmd->argv );
			exit(0);
		}
		wait(0);
	}
	else{
		perror("Command not found");

	}

	wait(NULL);

	return 0;


} // end execute function


int find_fullpath( char* command_name, command_t* p_cmd ) {



	
	// ----------------------------------------
	// TODO: you fully implement this function


	// -------------------------------------------
	// Added a default return statement
	// however this needs to be changed 
	// to reflect the correct return 
	// value described in the header file

	// provided code
	char* path_env_variable;
	path_env_variable = getenv( "PATH" );
	//printf("PATH = %s\n", path_env_variable );

	// duplicate the path so we dont overwrite it
	char pathCopy[strlen(path_env_variable)];
	strcpy(pathCopy, path_env_variable);

	// using tokens and ":" to delimit the path
	char* path = strtok(pathCopy, ";");

	// provided code
	struct stat buffer;
	int exists = -1;
	// string that represents the fully qualified
	// path of a file or directory on the file system
	char* file_or_dir[512];

	// Put your code in a loop
	while(path != NULL && exists == -1){

		// building the path 
		strcpy(file_or_dir, path);
		strcat(file_or_dir, "/");
		strcat(file_or_dir, command_name);

		exists = stat( file_or_dir, &buffer );
		if ( exists == 0 && ( S_IFDIR & buffer.st_mode ) ) {
		// Directory exists

		// I added this to your code
			// setting path 
			strcpy(p_cmd-> path, file_or_dir);
			exists = 0;
			return TRUE;


		} else if ( exists == 0 && ( S_IFREG & buffer.st_mode ) ) {
		// File exists
			strcpy(p_cmd-> path, file_or_dir);
			exists = 0;
			return TRUE;

		}
		path = strtok(NULL, ":");
	}	

	



	return FALSE;

} // end find_fullpath function


int is_builtin( command_t* p_cmd ) {

	int cnt = 0;

	while ( valid_builtin_commands[cnt] != NULL ) {

		if ( equals( p_cmd->path, valid_builtin_commands[cnt] ) ) {

			return TRUE;

		}

		cnt++;

	}

	return FALSE;

} // end is_builtin function


int do_builtin( command_t* p_cmd ) {

	// only builtin command is cd

	if ( DEBUG ) printf("[builtin] (%s,%d)\n", p_cmd->path, p_cmd->argc);

	struct stat buff;
	int status = ERROR;

	if ( p_cmd->argc == 1 ) {

		// -----------------------
		// cd with no arg
		// -----------------------
		// change working directory to that
		// specified in HOME environmental 
		// variable

		status = chdir( getenv("HOME") );

	} else if ( ( stat( p_cmd->argv[1], &buff ) == 0 && ( S_IFDIR & buff.st_mode ) ) ) {


		// -----------------------
		// cd with one arg 
		// -----------------------
		// only perform this operation if the requested
		// folder exists

		status = chdir( p_cmd->argv[1] );

	} 

	return status;

} // end do_builtin function



void cleanup( command_t* p_cmd ) {

	int i=0;
	
	while ( p_cmd->argv[i] != NULL ) {
		free( p_cmd->argv[i] );
		i++;
	}

	free( p_cmd->argv );
	free( p_cmd->path );	

} // end cleanup function


int equals( char* str1, const char* str2 ) {

	// First check length

	int len[] = {0,0};

	char* b_str1 = str1;
	const char* b_str2 = str2;

	while( (*str1) != '\0' ) { 
		len[0]++;
		str1++;
	}

	while( (*str2) != '\0' ) {
		len[1]++;
		str2++;
	}

	if ( len[0] != len[1] ) {

		return FALSE;

	} else {

		while ( (*b_str1) != '\0' ) {

			// don't care about case (you did not have to perform
			// this operation in your solution

			if ( tolower( (*b_str1)) != tolower((*b_str2)) ) {

				return FALSE;

			}

			b_str1++;
			b_str2++;

		}

	} 

	return TRUE;


} // end compare function definition

