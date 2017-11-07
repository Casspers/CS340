#include <stdio.h>
#include "dpsim.h"

/**************************************************

Main Function:
	int main( int argc, char** argv )

------------------------------------------------
In this function perform the following steps:
------------------------------------------------
1. Create the following variables:
	- main_thread (pthread_t)
	- status (join status value)
2. Create a main_thread 
	- If the return value != 0, then display an error message and 
	  immediately exit program with status value 1.
3. Join the main_thread
	- If the return value != 0, then display an error message and
	  immediately exit the program with status value 2.
4. Display join status value.
5. Exit program.

*/

int main( int argc, char** argv ) {


	// ---------------------------------------
	// TODO: you add your implementation here
	
	// declare status variable and main thread
	// used the variable names you wanted
	pthread_t mainThread;
	int pthreadStatus = pthread_create(&mainThread, NULL, th_main, NULL);

	// test if the thread has returned anything but 0
	if(pthreadStatus != 0){
		// Took your instructions litterally with error message
		printf("ERROR MESSAGE");
		exit(1);
	}

	//join attempt
	
	int joinStatus = pthread_join(mainThread, NULL);

	if(joinStatus != 0){
		printf("ERROR MESSAGE");
		exit(2);
	}
	printf("Join Status = %i\n" , joinStatus);
	
	return 0;

} // end main function