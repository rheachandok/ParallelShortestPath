# Parallel Shortest Path Algorithms

This repository contains implementations and performance analysis of six classical shortest path algorithms in both **sequential** and **parallel** forms using **OpenMP**. The goal is to analyze their scalability and performance across various graph sizes and threading configurations on modern multicore systems.

## 📚 Project Overview

This project was developed as part of the course _Multicore Processors: Architecture & Programming - Spring 2025_ at NYU. We investigate and implement parallel versions of:

- Bellman-Ford
- Dijkstra
- Floyd-Warshall
- Johnson's Algorithm
- Bidirectional Dijkstra
- Delta-Stepping

Each algorithm was implemented with both sequential and parallel logic, tested on graphs ranging from 10 to 8000 nodes.

## 👥 Authors
- Arya Goyal (ag9961)
- Haardik Dharma (hd2585)
- Manvi Pandya (mp6813)
- Rhea Chandok (rc5397)

## 🗂️ Repository Structure

```
ParallelShortestPath/
├── parallel/               # Parallel versions of each algorithm
├── sequential/             # Sequential versions of each algorithm
├── utils/                  # Graph generation and IO utilities
├── main.cpp                # Main driver program
├── algorithm_fns.h         # Header with function declarations
├── run_proj.sh             # Automated benchmarking script
└── README.md               # You're here!
```

## 🛠️ Build Instructions

1. ssh to cims
2. ssh to a crunchy server
3. cd into folder ParallelShortestPath

Ensure you have `g++` with OpenMP support.

```bash
g++ -fopenmp -o main \
    main.cpp \
    parallel/*.cpp \
    sequential/*.cpp \
    utils/*.cpp
```

## 🚀 Run Instructions

```bash
./main <algorithm_flag> <num_nodes> <num_threads>
```

### Supported Flags:
| Flag | Algorithm                |
|------|--------------------------|
|  B   | Bellman-Ford             |
|  D   | Dijkstra                 |
|  F   | Floyd-Warshall           |
|  J   | Johnson's Algorithm      |
|  BD  | Bidirectional Dijkstra   |
|  S   | Delta-Stepping           |

### Example:
```bash
./main B 1000 4
```

## 📊 Benchmark Script
Run all experiments (algorithms x node sizes x threads) and generate speedup and efficiency tables:

```bash
chmod +x run_proj.sh
./run_proj.sh
```

## 📈 Experimental Setup

- **Machine:** NYU CIMS Crunchy Servers (64-thread AMD Opteron)
- **Nodes Tested:** 10, 100, 1000, 2000, 4000, 8000
- **Threads:** 1, 2, 4, 8
- **Graph Density:** Controlled via sparsity factor (default = 5)

**Highlights:**
- Bellman-Ford showed >800x speedup for large graphs with 4 threads
- Floyd-Warshall and Johnson's scaled well
- Dijkstra and Bidirectional Dijkstra had poor scalability due to sequential priority queue
- Delta-Stepping balanced parallelism and performance on large sparse graphs

## 🔮 Future Scope
- Hybrid parallel + distributed models (OpenMP + MPI)
- Lock-free concurrent data structures
- Task-based runtime schedulers
- Real-world benchmark datasets (e.g., road networks, social graphs)

## 📊 Experimental Results

All experiments were conducted on NYU CIMS Crunchy Servers using 1, 2, 4, and 8 threads with node sizes ranging from 10 to 8000. Below are summarized results for key algorithms.

### Bellman-Ford (Speedup)

| Nodes \ Threads | 1     | 2     | 4     | 8     |
|------------------|-------|-------|-------|-------|
| 10               | 0.36  | 0.02  | 0.01  | 0.01  |
| 100              | 6.58  | 1.43  | 1.05  | 0.77  |
| 1000             | 128.1 | 93.9  | 79.3  | 53.2  |
| 2000             | 554.5 | 411.6 | 312.6 | 252.2 |
| 4000             | 662.2 | 865.7 | 865.9 | 662.8 |

### Dijkstra (Speedup)

| Nodes \ Threads | 1     | 2     | 4     | 8     |
|------------------|-------|-------|-------|-------|
| 10               | 0.36  | 0.03  | 0.02  | 0.01  |
| 100              | 0.20  | 0.05  | 0.03  | 0.02  |
| 1000             | 0.05  | 0.03  | 0.03  | 0.03  |
| 2000             | 0.05  | 0.01  | 0.02  | 0.01  |
| 4000             | 0.06  | 0.02  | 0.01  | 0.01  |

### Floyd-Warshall (Speedup)

| Nodes \ Threads | 1     | 2     | 4     | 8     |
|------------------|-------|-------|-------|-------|
| 1000             | 1.68  | 3.38  | 6.80  | 12.65 |
| 2000             | 1.55  | 3.08  | 6.37  | 12.73 |
| 4000             | 1.62  | 3.23  | 5.64  | 10.55 |

### Johnson's Algorithm (Speedup)

| Nodes \ Threads | 1     | 2     | 4     | 8     |
|------------------|-------|-------|-------|-------|
| 1000             | 2.36  | 4.35  | 7.82  | 7.69  |
| 2000             | 2.42  | 4.66  | 8.55  | 8.73  |
| 4000             | 2.33  | 4.80  | 8.80  | 9.06  |

### Delta-Stepping (Speedup)

| Nodes \ Threads | 1     | 2     | 4     | 8     |
|------------------|-------|-------|-------|-------|
| 1000             | 0.54  | 0.23  | 0.19  | 0.17  |
| 2000             | 0.72  | 0.33  | 0.25  | 0.22  |
| 4000             | 1.38  | 0.77  | 0.65  | 0.52  |

## 📌 Key Insights

- **Bellman-Ford** and **Floyd-Warshall** scale the best with thread count.
- **Dijkstra** and **Bidirectional Dijkstra** have minimal parallel gains due to priority queue dependence.
- **Delta-Stepping** is moderately parallel, ideal for sparse graphs.
- Thread counts beyond 4 generally yield diminishing returns due to overhead.