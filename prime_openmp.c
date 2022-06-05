#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int count_primes(int n);
void count_primes_loop(int n_lo, int n_hi, int n_factor);

int main(int argc, char *argv[]) {
    /* printf("Number of processors available = %d\n", omp_get_num_procs()); */
    /* printf("Number of threads = %d\n", omp_get_max_threads()); */
    printf("         N        Pi          Time\n\n");

    count_primes_loop(100000, 10000000, 2);

    printf("\nDone\n");

    return 0;
}

void count_primes_loop(int from, int to, int step) {
    for (int n = from; n <= to; n *= step) {
        double wtime = omp_get_wtime();
        int primes = count_primes(n);
        wtime = omp_get_wtime() - wtime;
        printf("  %8d  %8d  %14f\n", n, primes, wtime);
    }
}

int count_primes(int n) {
    int i;
    int j;
    int prime;
    int total = 0;

#pragma omp parallel shared(n) private(i, j, prime) num_threads(8)
#pragma omp for reduction(+ : total)
    for (i = 2; i <= n; i++) {
        prime = 1;

        for (j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                prime = 0;
                break;
            }
        }
        total += prime;
    }

    return total;
}
