// ----------------------------------------------
// These are the only libraries that can be
// used. Under no circumstances can additional
// libraries be included
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
	// Ignore spaces, check wheater anything has been entered, if entered do something

	int i = 0;
	char space = ' ';
	while (line[i] == space){ //remove leading space
		i++;
	}
	if(strcmp(line, "")== 0 ){
		p_cmd->path = NULL;
		p_cmd->argc = 0;
		p_cmd->argv = NULL;
	}
	else{
		char* token = strtok(line, " ");
		char cmd[sizeof(token)];
		strcpy(cmd, token);
		int count = 0;
		//p_cmd->argv = malloc(64 * sizeof(char*));

		while (token != NULL){
			p_cmd->argv[count] = token;
			count++;
			token = strtok(NULL, " ");
		}
		//p_cmd->argv[ count + 1 ] = NULL;

		int j; //index
		int builtin = -1; //boolean
		while(builtin == -1 && j<2){
			if(strcmp(cmd, valid_builtin_commands[j]) == 0){
				builtin = 0;
			}
			j++;
		}

		if(builtin == 0){
			//p_cmd->path = cmd;
			strcpy(p_cmd->path, cmd);
			p_cmd->argc = count;
		}
		else if(find_fullpath(cmd, p_cmd) == 0){
			p_cmd->path = cmd;
			p_cmd->argc = -1;
			p_cmd->argv = NULL;
		}
		else{
			find_fullpath(cmd, p_cmd);
			p_cmd->argc = -1;
		}
	}
} // end parse function


int execute( command_t* p_cmd ) {


	// ----------------------------------------
	// TODO: you fully implement this function

	// -------------------------------------------
	// Added a default return statement
	// however this needs to be changed
	// to reflect the correct return
	// value described in the header file
	//int status;
	//char fullpath[512];
	if(p_cmd->argc > 0)
	{
		if(fork() == 0)
		{
			char** argv[p_cmd->argc+1];
			for(int i = 0; i < p_cmd->argc; ++i) {
				argv[i] = p_cmd->argv[i];
			}
			argv[p_cmd->argc] = NULL;
			//sfind_fullpath(fullpath, p_cmd);
			execv(p_cmd->path, p_cmd->argv);
			exit(0);
		}
		wait(NULL);
	}
	else if (p_cmd -> argc == -1)
	{
		perror("User command is not found");
	}
	return wait(NULL);

} // end execute function


int find_fullpath( char* command_name, command_t* p_cmd ) {


	//char* path = getenv( "PATH" );

	// ----------------------------------------
	// TODO: you fully implement this function


	// -------------------------------------------
	// Added a default return statement
	// however this needs to be changed
	// to reflect the correct return
	// value described in the header file
	char* path_env_variable;
	path_env_variable = getenv( "PATH" );
	//printf("PATH = %s\n", path_env_variable );
	//char* pathcpy = malloc(strlen(path_env_variable));
	char pathcpy[strlen(path_env_variable)];
	strcpy(pathcpy, path_env_variable);
	char* path = strtok(pathcpy, ":");

	char* file_or_dir[512];
	int boolean = 0;
	int exists = -1;

	while(boolean == 0 && path != NULL)
	{
		struct stat buffer;
		int exists;
		strcpy(file_or_dir, path);
		strcat(file_or_dir, "/");
		strcat(file_or_dir, command_name);
		//printf("File or dir = %s\n", file_or_dir );
		exists = stat(file_or_dir, &buffer);
		if (exists == 0)
		{
			//p_cmd -> path =
			strcpy(p_cmd -> path, file_or_dir);
			boolean = 1;
			return TRUE;
		}
		path = strtok(NULL, ":");

	}
	// if (boolean != 1)
	// {
	// 	p_cmd -> path = strcpy(p_cmd -> path, file_or_dir);
	// 	return FALSE;
	// }
	// else
	// {
 	return FALSE;
	// }
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

