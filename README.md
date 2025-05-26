# 251RCOSE34102

To build, simply run:
```
make
```

Then run the program with:
```
./main --num_proc=<number> --max_arrival=<number> --max_burst=<number> --max_quantum=<number>
```

- num_proc: Number of processes to generate
- max_arrival: Maximum arrival time for processes
- max_burst: Maximum burst time for processes
- max_quantum: Maximum quantum time for processes

For custom processes, modify the custom_processes.txt file, following the instructions in the file.
For random custom processes, modify the random_processes.txt file, following the instructions in the file.

# Algorithms
- FCFS
- SJF
- PSJF
- PS (Nonpreemptive Priority Scheduling)
- PPS (Preemptive Priority Scheduling)
- RR
- LIF (Largest I/O First)
- PLIF
- LISC (Largest I/O & Smallest CPU First)
- PLISC
- HRN (Highest Response ration Next)
- PSA (Priority Scheduling with linear Aging)

# Evaluation
- Waiting time
- Turnaround time
- CPU Utilization
- Throughput
- Context Swith rate
