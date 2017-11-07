#include "dpsim.h"


static const unsigned int NUM_PHILOSPHERS = 5;
static const unsigned int NUM_CHOPSTICKS = 5;

static int chopsticks[5];
static pthread_mutex_t mutex[5];
static pthread_t philosphers[5];


void* th_main( void* th_main_args ) {

	// ---------------------------------------
	// TODO: you add your implementation here

	// initilize array and pthreads
	for(int i = 0; i < NUM_CHOPSTICKS ; i++){
		// set every position in chopsticks to -1
		chopsticks[i] = -1;

		//this was the only way i could pass the correct values in the pthread_create method
		int* arg = malloc(sizeof(*arg));
		*arg = i;
		//printf("arg = %i\n", *arg);

		// create pthread and test
		//delay(2000);
	    if(pthread_create(&philosphers[i], NULL, th_phil, arg) != 0){
	    	*arg = 1;
	    	pthread_exit(arg);
	    }

	}

int chopsticksCpy[5];
	
	// infinite loop that wil check for deadlock
	while(TRUE){

		// initilize chopstick copy
		memcpy(chopsticksCpy, chopsticks,  sizeof(chopsticks));

		


		// hard coded deadlock condition
		if(chopsticks[0] == 0 && chopsticks[1] == 1 && chopsticks[2] ==2 && chopsticks[3] == 3 && chopsticks[4] == 4){
			printf("Deadlock condition (0,1,2,3,4) ... terminating\n");
			// break this program off
			break;
			// winning
		}


		// TRACEABILITY TESTS
		/*
		//trace original chopsticks array
		printf("chopsticks[0] == %i\nchopsticks[1] == %i\nchopsticks[2] == %i\nchopsticks[3] == %i\nchopsticks[4] == %i\n", 
			chopsticks[0], chopsticks[1], chopsticks[2], chopsticks[3], chopsticks[4]);
		
		printf("\n");
		
		//trace copstick coppy array
		printf("chopstickCpy[0] == %i\nchopstickCpy[1] == %i\nchopstickCpy[2] == %i\nchopstickCpy[3] == %i\nchopstickCpy[4] == %i\n", 
			chopsticksCpy[0], chopsticksCpy[1], chopsticksCpy[2], chopsticksCpy[3], chopsticksCpy[4]);
		*/
		

		
		// code that prints status here
		printf("Philosopher(s) ");
		if(chopsticksCpy[0] == 0 && chopsticksCpy[1] == 0){
		printf("0, ");
		//delay(10000);
		}
		if(chopsticksCpy[1] == 1 && chopsticksCpy[2] == 1){
		printf("1, ");
		//delay(10000);
		}
		if(chopsticksCpy[2] == 2 && chopsticksCpy[3] == 2){
		printf("2, ");
		//delay(10000);
		}
		if(chopsticksCpy[3] == 3 && chopsticksCpy[4] == 3){
		printf("3, ");
		//delay(10000);
		}
		if(chopsticksCpy[4] == 4 && chopsticksCpy[0] == 4){
		printf("4 ");
		//delay(10000);
		}
		printf("are eating. \n");
		
		
		delay(1500);

		
	}



	


} // end th_main function


void* th_phil( void* th_phil_args ) {

	// ---------------------------------------
	// TODO: you add your implementation here

	//printf("Phil ID = %i\n", th_phil_args);
	int* philosopher = (int*)(th_phil_args);
	while(1){
		// delay for philosphers think about eating
		delay(2000);
		// philosphers eat
		eat(*philosopher);
	}


} // end th_phil function


void delay( long nanosec ) {

	struct timespec t_spec;

	t_spec.tv_sec = 0;
	t_spec.tv_nsec = nanosec;

	nanosleep( &t_spec, NULL );

} // end think function


void eat( int phil_id ) {

	// ---------------------------------------
	// TODO: you add your implementation here
	//delay(2000);

	// right chopstick shit
	
	pthread_mutex_lock(&mutex[phil_id]);
	chopsticks[phil_id] = phil_id;
	//printf("phil %i locked right\n", phil_id);
	delay(1000);

	//left chopstick shit
	int left_fucker = 0;
	//special case handeling
	if(phil_id !=4){
		left_fucker = ++phil_id;
	}
	//pick up left chopstick
	//printf("left Fucker %i\n", left_fucker);
	
	pthread_mutex_lock(&mutex[left_fucker]);
	chopsticks[left_fucker] = phil_id;
	//printf("phil %i locked %i\n", phil_id, left_fucker);

	//eating delay
	delay(2000);

	//put down left 
	chopsticks[left_fucker] = -1;
	pthread_mutex_unlock(&mutex[left_fucker]);	
	//printf("phil %i unlocked %i\n", phil_id, left_fucker);

	//put down right
	chopsticks[phil_id] = -1;
	pthread_mutex_unlock(&mutex[phil_id]);
	//printf("phil %i unlocked right\n", phil_id);
	

} // end eat function
