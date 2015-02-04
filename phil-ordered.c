#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NPHILOSOPHERS 5
#define NMEALS 3
#define NAPTIME 25000
#define VERBOSE 1

void *initializePhilosopher(int id);
void initializeFork(int id);
int getLeftFork(int id);
int getRightFork(int id);
int pickupFork(int id);
void dropFork(int id);
void nap(void);

typedef struct threaddata {
    int taken;
} threaddata;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;// lock to synchronize access to the free flag
pthread_mutex_t wait = PTHREAD_MUTEX_INITIALIZER;// lock to synchronize access to the free flag
pthread_cond_t freeLock = PTHREAD_COND_INITIALIZER;// condition variable so that threads can wait for access to the fork
threaddata f[NPHILOSOPHERS]; // global initialization of forks

int main (int argc, char *argv[])
{
	pthread_t threads[NPHILOSOPHERS];
	int check;
	int i,j,k;

	for(k = 0; k < NPHILOSOPHERS; k++) {
		initializeFork(k);
	}

	for(i = 0; i < NPHILOSOPHERS; i++) {
		check = pthread_create(&threads[i], NULL, initializePhilosopher, i);
		if (check){
			printf("ERROR; return code from pthread_create() is %d\n", check);
			exit(-1);
		}
	}

	for(j = 0; j < NPHILOSOPHERS; j++) {
		pthread_join(threads[j],NULL);
	}

	return 1;
}

void *initializePhilosopher(int id){
	int eaten = 1;
	pthread_cond_init(&freeLock, NULL);

		if ((getLeftFork(id)) < getRightFork(id)) {
			for (eaten = 1; eaten <= NMEALS; eaten++) {
			nap();
			if (pickupFork(getLeftFork(id))) {
				if (VERBOSE) {
					printf(" Philosopher #%d: I am picking up my left fork! \n", id);
				}
pickupRightNow:	nap();
				if (pickupFork(getRightFork(id))) {
					if (VERBOSE) {
						printf(" Philosopher #%d: I am picking up my right fork! \n", id);
					}
				}
				else { // can't pick up right fork, wait until he can
					while (!pickupFork(getRightFork(id))) {
						if (VERBOSE) {
							printf(" Philosopher #%d: I'm now waiting to pick up my right fork! \n", id);
						}
						pthread_mutex_lock(&wait);
						pthread_cond_wait(&freeLock, &wait);
						pthread_mutex_unlock(&wait);
					}
					pickupFork(getRightFork(id));
					if (VERBOSE) {
						printf(" Philosopher #%d: I am picking up my right fork! \n", id);
					}
				}
				nap();
				if (VERBOSE) {
					printf(" Philosopher #%d: I am eating meal #%d! \n", id, eaten);
				}
				if (eaten == NMEALS) {
					printf("\n Philosopher #%d: Mhhm! I have eaten all of my meals! \n", id);
				}
				nap();
				dropFork(getLeftFork(id));
				if (VERBOSE) {
					printf(" Philosopher #%d: I am putting down my left fork! \n", id);
				}
				pthread_mutex_lock(&wait);
				pthread_cond_signal(&freeLock);
				pthread_mutex_unlock(&wait);
				nap();
				dropFork(getRightFork(id));
				if (VERBOSE) {
					printf(" Philosopher #%d: I am putting down my right fork! \n", id);
				}
				pthread_mutex_lock(&wait);
				pthread_cond_signal(&freeLock);
				pthread_mutex_unlock(&wait);
			}


			else { // could not pick up left fork, wait until he can
				while (!pickupFork(getLeftFork(id))) {
					if (VERBOSE) {
						printf(" Philosopher #%d: I'm now waiting to pick up my left fork! \n", id);
					}
					pthread_mutex_lock(&wait);
					pthread_cond_wait(&freeLock, &wait);
					pthread_mutex_unlock(&wait);
				}

					if (VERBOSE) {
						pickupFork(getLeftFork(id));
						printf(" Philosopher #%d: I can now finally pick up my left fork! \n", id);
						printf(" Philosopher #%d: I am picking up my left fork! \n", id);
						goto pickupRightNow;
					}
			}
		}
		}
		else {
			for (eaten = 1; eaten <= NMEALS; eaten++) {
			nap();
			if (pickupFork(getRightFork(id))) {
				if (VERBOSE) {
					printf(" Philosopher #%d: I am picking up my right fork! \n", id);
				}
pickupLeftNow:	nap();
				if (pickupFork(getLeftFork(id))) {
					if (VERBOSE) {
						printf(" Philosopher #%d: I am picking up my left fork! \n", id);
					}
				}
				else { // can't pick up left fork, wait until he can
					while (!pickupFork(getLeftFork(id))) {
						if (VERBOSE) {
							printf(" Philosopher #%d: I'm now waiting to pick up my left fork! \n", id);
						}
						pthread_mutex_lock(&wait);
						pthread_cond_wait(&freeLock, &wait);
						pthread_mutex_unlock(&wait);
					}
					if (VERBOSE) {
						printf(" Philosopher #%d: I can now finally pick up my left fork! \n", id);
					}
					pickupFork(getLeftFork(id));
						if (VERBOSE) {
							printf(" Philosopher #%d: I am picking up my left fork! \n", id);
						}
				}
				nap();
				if (VERBOSE) {
					printf(" Philosopher #%d: I am eating meal #%d! \n", id, eaten);
				}
				if (eaten == NMEALS) {
					printf("\n Philosopher #%d: Mhhm! I have eaten all of my meals! \n", id);
				}
				nap();
				dropFork(getLeftFork(id));
				if (VERBOSE) {
					printf(" Philosopher #%d: I am putting down my left fork! \n", id);
				}
				pthread_mutex_lock(&wait);
				pthread_cond_signal(&freeLock);
				pthread_mutex_unlock(&wait);
				nap();
				dropFork(getRightFork(id));
				if (VERBOSE) {
					printf(" Philosopher #%d: I am putting down my right fork! \n", id);
				}
				pthread_mutex_lock(&wait);
				pthread_cond_signal(&freeLock);
				pthread_mutex_unlock(&wait);
			}
			else { // could not pick up right fork, wait until he can
				while (!pickupFork(getRightFork(id))) {
					if (VERBOSE) {
						printf(" Philosopher #%d: I'm now waiting to pick up my right fork! \n", id);
					}
					pthread_mutex_lock(&wait);
					pthread_cond_wait(&freeLock, &wait);
					pthread_mutex_unlock(&wait);
				}
				if (VERBOSE) {
					pickupFork(getRightFork(id));
					printf(" Philosopher #%d: I can now finally pick up my right fork! \n", id);
					printf(" Philosopher #%d: I am picking up my right fork! \n", id);
					goto pickupLeftNow;
				}
			}
		}
		}

	pthread_mutex_lock(&wait);
	pthread_cond_broadcast(&freeLock);
	pthread_mutex_unlock(&wait);
	pthread_exit(NULL);
}

void initializeFork(int id) {
	f[id].taken = 0;
}

// given a philosopher id (0 .. 4), return the index of his left fork
// non-circular ordered fork pickup to prevent deadlocks
int getLeftFork(int id) {
	if (id == 0) {
		return 3;
	}
	else if (id == 1) {
		return 2;
	}
	else if (id == 2) {
		return 1;
	}
	else if (id == 3) {
		return 4;
	}
	else {
		return 0;
	}
}

// given a philosopher id (0 .. 4), return the index of his right fork
// non-circular ordered fork pickup to prevent deadlocks
int getRightFork(int id) {
	if (id == 0) {
		return 2;
	}
	else if (id == 1) {
		return 1;
	}
	else if (id == 2) {
		return 4;
	}
	else if (id == 3) {
		return 0;
	}
	else {
		return 3;
	}
}

// given a philosopher id (0 .. 4), picks up his fork, returns 1 if successful pickup, 0 if not
int pickupFork(int id) {
	pthread_mutex_lock(&lock);
    if (!f[id].taken) {
    	f[id].taken = 1;
    	pthread_mutex_unlock(&lock);
    	return 1;
    }
    else {
    	pthread_mutex_unlock(&lock);
    	return 0;
    }
}

// given a philosopher id (0 .. 4), drops his fork, returns 1 if successful drop, 0 if not
void dropFork(int id) {
	pthread_mutex_lock(&wait);
	f[id].taken = 0;
	pthread_cond_signal(&freeLock);
	pthread_mutex_unlock(&wait);
}

// naps for a random time
void nap(void) {
	usleep(random() % NAPTIME);
}
