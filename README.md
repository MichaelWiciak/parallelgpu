### Repository Overview: Parallel Programming in C

This repository demonstrates three different approaches to parallel programming in C, each utilizing a distinct parallel computing model: OpenMP, MPI, and OpenCL. These models are applied to different computational problems, showcasing how parallelism can be leveraged to improve performance and scalability. Below is a detailed breakdown of the three implementations:

---

#### 1. **OpenMP: Parallel Set Operations**

This implementation leverages OpenMP to perform parallel operations on a dynamic set of integers. The operations include adding elements to the set, removing elements, and sorting the set using an odd-even sort algorithm. Key features include:

- **Critical Sections**: Ensures thread safety when adding elements to the set to avoid race conditions.
- **Parallel Loops**: The removal operation and sorting algorithm utilize `#pragma omp parallel for` to distribute work across threads.
- **Dynamic Set Management**: Handles operations like checking for duplicates and shifting elements after removal in a parallelized manner.

*Key Functions*:
- `addToSet(int value)`: Adds an integer to the set in a thread-safe manner.
- `removeFromSet(int value)`: Removes an integer from the set while maintaining the order of elements.
- `sortSet()`: Sorts the set using a parallelized version of the bubble sort algorithm.

---

#### 2. **MPI: Parallel Text Histogram**

This implementation uses MPI to perform parallel processing of text files, aiming to build a histogram of letter frequencies. It demonstrates both collective communication and point-to-point communication techniques in MPI:

- **File Distribution**: The text file is read and distributed among processes, either through broadcasting or an inverse binary tree pattern depending on the number of processes.
- **Local Computation**: Each process calculates a local histogram of the letters from its portion of the text.
- **Global Reduction**: Local histograms are then reduced to a global histogram using `MPI_Reduce`.

*Key Concepts*:
- **Dynamic Memory Allocation**: Handles different parts of the text dynamically across processes.
- **Scalability**: The implementation adapts to different numbers of processes and efficiently manages communication.
- **Error Checking**: The result is checked against a serial computation to ensure correctness.

---

#### 3. **OpenCL: Parallel Gradient Computation**

This implementation uses OpenCL to parallelize the computation of weighted sums based on gradients and input values. It showcases how to use GPU parallelism effectively:

- **Kernel Execution**: The main computation is offloaded to the GPU, where each thread computes a part of the weighted sum.
- **Device Management**: The OpenCL context, command queue, and memory buffers are set up and managed to execute the kernel.
- **Performance Tuning**: The work-group size is determined based on the device capabilities, ensuring optimal performance.

*Key Functions*:
- `simpleOpenContext_GPU()`: Initializes the OpenCL context on the GPU.
- `compileKernelFromFile()`: Compiles the OpenCL kernel from a source file.
- **Kernel Execution**: The kernel performs the computation of `weights[i][j] += gradients[i] * inputs[j]`.

---

### Usage

Each folder in this repository contains a specific parallel programming example. To compile and run the examples, follow these general steps:

1. **Navigate to the respective directory** (`openmp/`, `mpi/`, `opencl/`).
2. **Compile the program** using `make` or a provided `gcc`/`mpicc`/`gcc -lOpenCL` command.
3. **Run the program** using `./program_name` with the appropriate command-line arguments.

Ensure that your environment has the necessary dependencies installed, such as OpenMP, MPI, and OpenCL development libraries.
