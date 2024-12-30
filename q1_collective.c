#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

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

void write_vector(const char *filename, double *vector, int length) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    fprintf(file, "%d\n", length);
    for (int i = 0; i < length; i++) {
        fprintf(file, "%lf ", vector[i]);
    }
    fprintf(file, "\n");
    fclose(file);
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double *vector = NULL;
    double *local_vector = NULL;
    int length = 0, local_length;

    if (rank == 0) {
        // Read the vector from file
        read_vector("input_vector.txt", &vector, &length);

        if (length % size != 0) {
            fprintf(stderr, "Vector length must be divisible by number of processes.\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
    }

    // Broadcast the length to all processes
    MPI_Bcast(&length, 1, MPI_INT, 0, MPI_COMM_WORLD);

    local_length = length / size;
    local_vector = (double *)malloc(local_length * sizeof(double));

    // Scatter the vector to all processes
    MPI_Scatter(vector, local_length, MPI_DOUBLE, local_vector, local_length, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Add 1.0 to each local element
    for (int i = 0; i < local_length; i++) {
        local_vector[i] += 1.0;
    }

    // Gather the updated local vectors back to rank 0
    MPI_Gather(local_vector, local_length, MPI_DOUBLE, vector, local_length, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Write the modified vector to output file
        write_vector("output_vector_collective.txt", vector, length);
        free(vector);
    }

    free(local_vector);
    MPI_Finalize();
    return 0;
}
