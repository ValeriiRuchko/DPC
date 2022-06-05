#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int count_primes(int n);
void count_primes_loop(int from, int to, int step);

int main(int argc, char *argv[]) {
    printf("         N        Pi          Time\n\n");
    count_primes_loop(100000, 10000000, 2);
    printf("\nDone\n");
    return 0;
}

void count_primes_loop(int from, int to, int step) {
    for (int n = from; n <= to; n *= step) {
        clock_t wtime = clock();
        int primes = count_primes(n);
        printf("  %8d  %8d  %14f\n", n, primes,
               (double)(clock() - wtime) / CLOCKS_PER_SEC);
    }
}

int count_primes(int n) {
    int total = 0;

    for (int i = 2; i <= n; i++) {
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
