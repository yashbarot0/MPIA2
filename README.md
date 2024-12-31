# README

## MSc in High-Performance Computing
**Module**: Introduction to MPI (55611)  
**Assignment**: Programming Exercises 1  
**Author**: Yashkumar Barot

---

## Table of Contents
1. [Overview](#overview)
2. [Requirements](#requirements)
3. [Files Provided](#files-provided)
4. [Compilation Instructions](#compilation-instructions)
5. [Execution Instructions](#execution-instructions)
6. [Expected Output](#expected-output)
7. [Assumptions and Notes](#assumptions-and-notes)

---

## Overview
This is the solutions to the first MPI programming exercise, consisting of:
1. Reading and modifying a vector from a file (Q1).
2. Decomposing a list of items over processors (Q2).

Each solution has been implemented in C and uses MPI for parallelism.
---

## Requirements
The following are required to compile and run the program:
- MPI installation (`mpicc`, `mpirun`)
- GNU Make
- Input files (`input_vector.txt`)

---

## Files Provided
1. **Source Files**:
   - `q1_send_recv.c`: Q1 implementation using `MPI_Send` and `MPI_Recv`.
   - `q1_collective.c`: Q1 implementation using MPI collective calls.
   - `q2_decomp.c`: Q2 implementation for one-dimensional decomposition.
  
2. **Input Files**:
   - `input_vector.txt`: Input for Q1.

3. **Makefile**:
   - Automates the compilation of all programs.

4. **README**: Instructions for compiling and running the code.

---

## Compilation Instructions
To compile all programs:
```bash
make
```

To compile individual programs:
```bash
make q1_send_recv
make q1_collective
make q2_decomp
```

To clean up binaries:
```bash
make clean
```

---

## Execution Instructions
### Q1: Reading and Modifying a Vector
1. **Using `MPI_Send`/`MPI_Recv`:**
   ```bash
   mpirun -np <num_procs> ./q1_send_recv
   ```
   Example:
   ```bash
   mpirun -np 4 ./q1_send_recv
   ```

2. **Using MPI Collective Calls:**
   ```bash
   mpirun -np <num_procs> ./q1_collective
   ```
   Example:
   ```bash
   mpirun -np 4 ./q1_collective
   ```

### Q2: One-Dimensional Decomposition
Run the decomposition program:
```bash
mpirun -np <num_procs> ./q2_decomp
```
Example:
```bash
mpirun -np 9 ./q2_decomp
```

---

## Expected Output
### Q1
- Modified vector is written to `output_vector.txt` (for `MPI_Send`/`MPI_Recv`) or `output_vector_collective.txt` (for MPI collective calls).

### Q2
Each rank prints its range of elements:
```
Rank <rank_id>: Start = <start_index>, End = <end_index>
```

---

## Assumptions and Notes
1. **Q1**:
   - The vector length must be divisible by the number of processors.
   - Input file format:
     ```
     <length_of_vector>
     <vector_elements>
     ```

2. **Q2**:
   - Decomposition is load-balanced, with a maximum size difference of 1 between chunks.

---