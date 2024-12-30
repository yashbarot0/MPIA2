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

    double *vector = NULL, *local_vector = NULL;
    int length, local_length;

    if (rank == 0) {
        // Read the vector from the file
        read_vector("input_vector.txt", &vector, &length);

        if (length % size != 0) {
            fprintf(stderr, "Vector length must be divisible by number of processes.\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        local_length = length / size;

        // Distribute parts of the vector
        for (int i = 1; i < size; i++) {
            MPI_Send(&local_length, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(vector + i * local_length, local_length, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&local_length, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        local_vector = (double *)malloc(local_length * sizeof(double));
        MPI_Recv(local_vector, local_length, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Add 1.0 to each local element
    if (rank == 0) {
        for (int i = 0; i < local_length; i++) {
            vector[i] += 1.0;
        }
    } else {
        for (int i = 0; i < local_length; i++) {
            local_vector[i] += 1.0;
        }
    }

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Recv(vector + i * local_length, local_length, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Write the modified vector to the output file
        write_vector("output_vector.txt", vector, length);
        free(vector);
    } else {
        MPI_Send(local_vector, local_length, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        free(local_vector);
    }

    MPI_Finalize();
    return 0;
}
