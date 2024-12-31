#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

void read_vector(const char *filename, double **vector, int *length) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    fscanf(file, "%d", length);
    *vector = (double *)malloc((*length) * sizeof(double));
    for (int i = 0; i < *length; i++) {
        fscanf(file, "%lf", &((*vector)[i]));
    }
    fclose(file);
}

int decomp1d(int n, int p, int myid, int *s, int *e) {
    int base_size = n / p;
    int remainder = n % p;
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

    double *vector = NULL, *local_vector = NULL;
    int length = 0, start, end, local_length;
    double local_sum = 0.0, global_sum = 0.0;

    if (rank == 0) {
        // Read the vector from file
        read_vector("q3file_16.txt", &vector, &length);
    }

    // Broadcast vector length
    MPI_Bcast(&length, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Decompose the array
    decomp1d(length, size, rank, &start, &end);
    local_length = end - start + 1;

    local_vector = (double *)malloc(local_length * sizeof(double));

    // Scatter the vector to all processes
    MPI_Scatter(vector, local_length, MPI_DOUBLE, local_vector, local_length, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Compute the local sum of squares
    for (int i = 0; i < local_length; i++) {
        local_sum += local_vector[i] * local_vector[i];
    }

    // Gather the results onto rank 2
    if (rank == 2) {
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 2, MPI_COMM_WORLD);
        printf("Rank 2: L2 norm squared = %.2f\n", global_sum);
    }

    // Gather the results onto all processes
    MPI_Allreduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    printf("Rank %d: L2 norm squared = %.2f\n", rank, global_sum);

    free(local_vector);
    if (rank == 0) free(vector);

    MPI_Finalize();
    return 0;
}
