#include <stdio.h>
#include <mpi.h>

int decomp1d(int n, int p, int myid, int *s, int *e) {
    int base_size = n / p;       // Base size of each chunk
    int remainder = n % p;      // Extra elements to distribute
    if (myid < remainder) {
        *s = myid * (base_size + 1);
        *e = *s + base_size;
    } else {
        *s = myid * base_size + remainder;
        *e = *s + base_size - 1;
    }
    return 0; // Success
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 25;  // Change to test with other values, e.g., 100000
    if (rank == 0) {
        printf("Decomposing %d items across %d processors\n", n, size);
    }

    int s, e;
    decomp1d(n, size, rank, &s, &e);

    printf("Rank %d: Start = %d, End = %d\n", rank, s, e);

    MPI_Finalize();
    return 0;
}
