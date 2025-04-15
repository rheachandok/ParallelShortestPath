#!/usr/bin/env bash
###############################################################################
# 0) Exit on any un‑handled error
###############################################################################
set -euo pipefail

###############################################################################
# 1) Compile
###############################################################################
g++ -std=c++17 -fopenmp -O2 \
    main.cpp utils/graph_utils.cpp parallel/*.cpp sequential/*.cpp \
    -o run_main

if [[ $? -ne 0 ]]; then
    echo "❌ Compilation failed."
    exit 1
fi

###############################################################################
# 2) Parameter lists
###############################################################################
algorithms=(B F J D BD S)          # Bellman‑Ford, Floyd–Warshall, Johnson
node_counts=(10 100 1000 2000 4000)
thread_counts=(1 2 4 8)

# Two associative arrays:
#   seq_times["algo,nodes"]              → average sequential time
#   par_times["algo,nodes,threads"]      → average parallel   time
declare -A seq_times
declare -A par_times

###############################################################################
# 3) Helper: “is this a valid floating‑point number?”
###############################################################################
is_float() { [[ $1 =~ ^[0-9]+(\.[0-9]+)?([eE][-+]?[0-9]+)?$ ]]; }

###############################################################################
# 4) Benchmark loops
###############################################################################
for algo in "${algorithms[@]}"; do
  echo -e "\n================  $algo  ================="

  for nodes in "${node_counts[@]}"; do
    for t in "${thread_counts[@]}"; do

      total_seq=0
      total_par=0
      valid_runs=0

      for run in {1..3}; do
        # ---------------------------------------------------------------
        # Run the program
        # ---------------------------------------------------------------
        output=$(./run_main "$algo" "$nodes" "$t")

        # Skip graphs with negative cycles for F or J
        if [[ "$algo" != "B" && "$output" =~ [Nn]egative\ weight\ cycle ]]; then
          echo "⚠️  Skipping run (neg. cycle)  algo=$algo nodes=$nodes t=$t run=$run"
          continue
        fi

        # ---------------------------------------------------------------
        # Extract sequential (first) and parallel (last) times
        # ---------------------------------------------------------------
        seq_time=$(echo "$output" | grep "Time Taken:" | head -n 1 | awk -F: '{print $2}' | xargs)
        par_time=$(echo "$output" | grep "Time Taken:" | tail -n 1 | awk -F: '{print $2}' | xargs)

        # # DEBUG
        >&2 echo "DEBUG  algo=$algo nodes=$nodes t=$t run=$run  seq=$seq_time  par=$par_time"

        if is_float "$seq_time" && is_float "$par_time"; then
          total_seq=$(echo "$total_seq + $seq_time" | bc -l)
          total_par=$(echo "$total_par + $par_time" | bc -l)
          valid_runs=$((valid_runs + 1))
        fi
      done  # five runs

      if [[ $valid_runs -gt 0 ]]; then
        avg_seq=$(echo "$total_seq / $valid_runs" | bc -l | sed 's/^\./0./')
avg_par=$(echo "$total_par / $valid_runs" | bc -l | sed 's/^\./0./')

      else
        avg_seq="NaN"
        avg_par="NaN"
      fi

      seq_times["$algo,$nodes"]=$avg_seq           # one per (algo,nodes)
      par_times["$algo,$nodes,$t"]=$avg_par        # one per (algo,nodes,threads)

    done  # thread loop
  done    # node loop

  #############################################################################
  # 5) Print speed‑up table  (seq / par)
  #############################################################################
  echo -e "\nSpeed‑up  (Sequential ÷ Parallel)"
  echo -e "Nodes\t\t1‑thr\t\t2‑thr\t\t4‑thr\t\t8‑thr"

  for nodes in "${node_counts[@]}"; do
    printf "%s\t" "$nodes"
    base_seq=${seq_times["$algo,$nodes"]}

    for t in "${thread_counts[@]}"; do
      par=${par_times["$algo,$nodes,$t"]}

      if is_float "$base_seq" && is_float "$par" && [[ "$par" != "0" ]]; then
        spd=$(echo "$base_seq / $par" | bc -l)
        printf "\t%.6f" "$spd"
      else
        printf "\tNaN"
      fi
    done
    echo
  done


  #############################################################################
  # 6) Print efficiency table  (speed‑up ÷ threads)
  #############################################################################
  echo -e "\nEfficiency  (Speed‑up ÷ #Threads)"
  echo -e "Nodes\t\t1‑thr\t\t2‑thr\t\t4‑thr\t\t8‑thr"

  for nodes in "${node_counts[@]}"; do
    printf "%s\t" "$nodes"
    base_seq=${seq_times["$algo,$nodes"]}

    for t in "${thread_counts[@]}"; do
      par=${par_times["$algo,$nodes,$t"]}

      if is_float "$base_seq" && is_float "$par" && [[ "$par" != "0" ]]; then
        spd=$(echo "$base_seq / $par" | bc -l)
        eff=$(echo "$spd / $t" | bc -l)
        printf "\t%.6f" "$eff"
      else
        printf "\tNaN"
      fi
    done
    echo
  done
done  # algorithm loop
