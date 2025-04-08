#!/bin/bash

# Compile the program
g++ -std=c++17 -fopenmp -O2 main.cpp graph_utils.cpp bellmanford_parallel.cpp bellmanford_sequential.cpp floydwarshall_parallel.cpp floydwarshall_sequential.cpp johnsons_parallel.cpp johnsons_sequential.cpp -o run_main

# Algorithms: B = Bellman-Ford, F = Floyd-Warshall, J = Johnson's
declare -a algorithms=("B" "F" "J")
declare -a node_counts=(10 20 40)
declare -a thread_counts=(1 2 4 8)
declare -A avg_times

for algo in "${algorithms[@]}"
do
    echo -e "\n========== $algo Algorithm Benchmark =========="
    echo -e "\nTime Table (in seconds)"
    echo -e "Nodes\t\t1 thread\t2 threads\t4 threads\t8 threads"

    for nodes in "${node_counts[@]}"
    do
        echo -n "$nodes"
        for t in "${thread_counts[@]}"
        do
            total_time=0
            valid_runs=0

            for i in {1..5}
            do
                output=$(./run_main $algo $nodes $t)
                if echo "$output" | grep -q "negative weight cycle"; then
                    echo "⚠️ Invalid graph for $algo with $nodes nodes and $t threads (Run $i)"
                    continue
                fi

                time_taken=$(echo "$output" | grep "Time Taken:" | tail -n 1 | awk -F: '{print $2}')
                total_time=$(echo "$total_time + $time_taken" | bc -l)
                valid_runs=$((valid_runs + 1))
            done

            if [ "$valid_runs" -gt 0 ]; then
                avg_time=$(echo "$total_time / $valid_runs" | bc -l)
            else
                avg_time="NaN"
            fi

            avg_times["$algo,$nodes,$t"]=$avg_time
            printf "\t%s" "$avg_time"
        done
        echo
    done

    echo -e "\nSpeedup Table"
    echo -e "Nodes\t\t1 thread\t2 threads\t4 threads\t8 threads"
    for nodes in "${node_counts[@]}"
    do
        echo -n "$nodes"
        base_time=${avg_times["$algo,$nodes,1"]}

        for t in "${thread_counts[@]}"
        do
            this_time=${avg_times["$algo,$nodes,$t"]}
            if [[ "$base_time" != "NaN" && "$this_time" != "NaN" ]]; then
                speedup=$(echo "$base_time / $this_time" | bc -l)
                printf "\t%.6f" "$speedup"
            else
                printf "\t%s" "NaN"
            fi
        done
        echo
    done

    echo -e "\nEfficiency Table"
    echo -e "Nodes\t\t1 thread\t2 threads\t4 threads\t8 threads"
    for nodes in "${node_counts[@]}"
    do
        echo -n "$nodes"
        base_time=${avg_times["$algo,$nodes,1"]}

        for t in "${thread_counts[@]}"
        do
            this_time=${avg_times["$algo,$nodes,$t"]}
            if [[ "$base_time" != "NaN" && "$this_time" != "NaN" ]]; then
                speedup=$(echo "$base_time / $this_time" | bc -l)
                efficiency=$(echo "$speedup / $t" | bc -l)
                printf "\t%.6f" "$efficiency"
            else
                printf "\t%s" "NaN"
            fi
        done
        echo
    done
done
