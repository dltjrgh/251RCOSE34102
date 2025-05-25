#include "fcfs.h"
#include "pool.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

// create random processes and insert them into the pool
void setupRandomProcesses(Pool *pool, int num_proc, int max_arrival,
                          int max_burst) {
  for (int i = 0; i < num_proc; i++) {
    int arrival_time =
        rand() % max_arrival; // Random arrival time between 0 and max_arrival
    int priority = rand() % 5 + 1; // Random priority between 1 and 5
    int total_burst_time =
        rand() % max_burst + 1; // Random burst time between 1 and max_burst
    Queue *workloads = create_queue();
    while (total_burst_time > 0) {
      int burst_t = rand() % total_burst_time + 1; // Random burst time
      addWorkload(workloads, burst_t);
      total_burst_time -= burst_t; // Decrease the remaining burst time
    }
    Process *process = createProcess(i, arrival_time, priority, workloads);
    insertProcessToPool(pool, process);
  }
}

int main(int argc, char *argv[]) {
  int c;
  int num_proc = 0;
  int max_arrival = 0;
  int max_burst = 0;
  int option_index = 0;

  static struct option long_options[] = {
      {"num_proc", required_argument, 0, 'n'},
      {"max_arrival", required_argument, 0, 't'},
      {"max_burst", required_argument, 0, 'b'},
      {0, 0, 0, 0}};

  while ((c = getopt_long(argc, argv, "n:t:b:", long_options, &option_index)) !=
         -1) {
    switch (c) {
    case 'n':
      num_proc = atoi(optarg);
      break;
    case 't':
      max_arrival = atoi(optarg);
      break;
    case 'b':
      max_burst = atoi(optarg);
      break;
    case '?':
      fprintf(stderr,
              "Usage: %s --num_proc=<number> --max_arrival=<number> "
              "--max_burst=<number>\n",
              argv[0]);
      exit(EXIT_FAILURE);
    default:
      abort();
    }
  }

  printf("Number of processes: %d\n", num_proc);
  printf("Simulation time: %d\n", max_arrival);
  printf("Max burst time: %d\n", max_burst);

  Pool *pool = (Pool *)malloc(sizeof(Pool));

  pool = createPool(num_proc);

  setupRandomProcesses(pool, num_proc, max_arrival, max_burst);

  printf("%d\n", getMinArrivalTime(pool));
  printPool(pool);

  FCFSState *fcfs_state = malloc(sizeof(FCFSState));
  fcfs_state = fcfs_init(pool);
  execute_fcfs(fcfs_state);

  return 0;
}
