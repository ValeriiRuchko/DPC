#include <math.h>
#include </opt/homebrew/include/mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int count_primes(int n, int id, int p);
void count_primes_loop(int from, int to, int step, int p, int id);

int main(int argc, char *argv[]) {
    int id;
    int p;

    if (MPI_Init(&argc, &argv) != 0) {  //initialization, params: to get command line args
        printf("\nFatal error! MPI_Init returned nonzero IERR.\n"); //check of initialization of parallel programm
        exit(1);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &p);  //to find out how much _threads_ are running; 1 - communicator; 2 - amount of threads
    MPI_Comm_rank(MPI_COMM_WORLD, &id); //to find out on which _process_ instance of programm is running; 2 - number of process (rank)

    if (id == 0) {
        /* printf("The number of processes is %d\n\n", p); */
        printf("         N        Pi          Time\n\n");
    }

    count_primes_loop(100000, 10000000, 2, p, id);

    MPI_Finalize(); //end of parallel programm

    if (id == 0) {
        printf("\nDone\n");
    }

    return 0;
}

void count_primes_loop(int from, int to, int step, int p, int id) {
    int primes = 0;
    double wtime;

    for (int n = from; n <= to; n *= step) {
        if (id == 0) {              //if it's process with rank zero - start counting time
            wtime = MPI_Wtime();    //returns some astronomic time in seconds
        }

        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);   //one to many processes; memory buffer;
        int primes_part = count_primes(n, id, p);
        MPI_Reduce(&primes_part, &primes, 1, MPI_INT, MPI_SUM, 0,   //defines collective operation
                   MPI_COMM_WORLD);

        if (id == 0) {
            wtime = MPI_Wtime() - wtime;    //runtime of our programm
            printf("  %8d  %8d  %14f\n", n, primes, wtime);
        }
    }
}

int count_primes(int n, int id, int p) {
    int total = 0;

    for (int i = 2 + id; i <= n; i = i + p) {
        int prime = 1;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                prime = 0;
                break;
            }
        }
        total += prime;
    }

    return total;
}
