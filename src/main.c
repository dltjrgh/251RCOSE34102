#include "fcfs.h"
#include "lif.h"
#include "lisc.h"
#include "plif.h"
#include "plisc.h"
#include "pool.h"
#include "pps.h"
#include "ps.h"
#include "psjf.h"
#include "rr.h"
#include "sjf.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int get_num_proc_from_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening config file");
    exit(EXIT_FAILURE);
  }

  int num_proc;
  if (fscanf(file, "%d", &num_proc) != 1) {
    fprintf(stderr, "Error reading number of processes from %s\n", filename);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  fclose(file);
  return num_proc;
}

void setup_processes_from_file(Pool *pool, int index, const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening config file");
    exit(EXIT_FAILURE);
  }

  int num_proc;
  if (fscanf(file, "%d", &num_proc) != 1) {
    fprintf(stderr, "Error reading number of processes\n");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < num_proc; i++) {
    int arrival_time, priority, burst_t;

    if (fscanf(file, "%d %d", &arrival_time, &priority) != 2) {
      fprintf(stderr, "Error reading arrival time or priority for process %d\n",
              i + index);
      fclose(file);
      exit(EXIT_FAILURE);
    }

    Queue *workloads = create_queue();

    while (fscanf(file, "%d", &burst_t) == 1 && burst_t != 0) {
      add_workload(workloads, burst_t);
    }

    Process *process =
        create_process(i + index, arrival_time, priority, workloads);
    insert_heap(pool, process);
  }

  fclose(file);
}

// create random processes and insert them into the pool
void setup_random_processes(Pool *pool, int num_proc, int max_arrival,
                            int max_burst) {
  srand(time(NULL));

  for (int i = 0; i < num_proc; i++) {
    int arrival_time =
        rand() % max_arrival; // Random arrival time between 0 and max_arrival
    int priority = rand() % 5 + 1; // Random priority between 1 and 5
    int total_burst_time =
        rand() % max_burst + 1; // Random burst time between 1 and max_burst
    Queue *workloads = create_queue();
    while (total_burst_time > 0) {
      int burst_t = rand() % total_burst_time + 1; // Random burst time
      add_workload(workloads, burst_t);
      total_burst_time -= burst_t; // Decrease the remaining burst time
    }
    Process *process = create_process(i, arrival_time, priority, workloads);
    insert_heap(pool, process);
  }
}

int get_num_rdm_proc_from_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening config file");
    exit(EXIT_FAILURE);
  }

  int num_set;
  int acc = 0;
  if (fscanf(file, "%d", &num_set) != 1) {
    fprintf(stderr, "Error reading number of random processes from %s\n",
            filename);
    fclose(file);
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < num_set; i++) {
    int num_proc, arrival_lb, arrival_ub, burst_lb, burst_ub, dummy;

    if (fscanf(file, "%d %d %d %d %d", &num_proc, &arrival_lb, &arrival_ub,
               &burst_lb, &burst_ub) != 5) {
      fprintf(stderr, "Error reading process set %d\n", i + 1);
      fclose(file);
      exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%d", &dummy) == 1 && dummy != 0) {
      ;
    }

    acc += num_proc;
  }

  fclose(file);
  return acc;
}

void set_adtl_random_processes(Pool *pool, int index, int num_proc,
                               int arrival_lb, int arrival_ub, int burst_lb,
                               int burst_ub) {
  srand(time(NULL));

  for (int i = 0; i < num_proc; i++) {
    int arrival_time =
        rand() % (arrival_ub - arrival_lb + 1) +
        arrival_lb; // Random arrival time between arrival_lb and arrival_ub
    int priority = rand() % 5 + 1; // Random priority between 1 and 5
    int total_burst_time =
        rand() % (burst_ub - burst_lb + 1) +
        burst_lb; // Random burst time between burst_lb and burst_ub
    Queue *workloads = create_queue();
    while (total_burst_time > 0) {
      int burst_t = rand() % total_burst_time + 1; // Random burst time
      add_workload(workloads, burst_t);
      total_burst_time -= burst_t; // Decrease the remaining burst time
    }
    Process *process =
        create_process(index + i, arrival_time, priority, workloads);
    insert_heap(pool, process);
  }
}

void setup_rdm_processes_from_file(Pool *pool, int index,
                                   const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening config file");
    exit(EXIT_FAILURE);
  }

  int num_set;
  if (fscanf(file, "%d", &num_set) != 1) {
    fprintf(stderr, "Error reading number of processes\n");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < num_set; i++) {
    int num_proc, arrival_lb, arrival_ub, burst_lb, burst_ub, dummy;

    if (fscanf(file, "%d %d %d %d %d", &num_proc, &arrival_lb, &arrival_ub,
               &burst_lb, &burst_ub) != 5) {
      fprintf(stderr, "Error reading arrival time or priority for process %d\n",
              index);
      fclose(file);
      exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%d", &dummy) == 1 && dummy != 0) {
      ;
    }

    set_adtl_random_processes(pool, index, num_proc, arrival_lb, arrival_ub,
                              burst_lb, burst_ub);
    index += num_proc; // Update index for the next set of processes
  }

  fclose(file);
}

int main(int argc, char *argv[]) {
  int c;
  int num_rand_proc = 0;
  int num_adtl_rand_proc = 0;
  int num_custom_proc = 0;
  int tot_proc = 0;
  int max_arrival = 0;
  int max_burst = 0;
  int max_quantum = 0;
  int option_index = 0;

  static struct option long_options[] = {
      {"num_proc", required_argument, 0, 'n'},
      {"max_arrival", required_argument, 0, 't'},
      {"max_burst", required_argument, 0, 'b'},
      {"max_quantum", required_argument, 0, 'q'},
      {0, 0, 0, 0}};

  while ((c = getopt_long(argc, argv, "n:t:b:q:", long_options,
                          &option_index)) != -1) {
    switch (c) {
    case 'n':
      num_rand_proc = atoi(optarg);
      break;
    case 't':
      max_arrival = atoi(optarg);
      break;
    case 'b':
      max_burst = atoi(optarg);
      break;
    case 'q':
      max_quantum = atoi(optarg);
      break;
    case '?':
      fprintf(stderr,
              "Usage: %s --num_proc=<number> --max_arrival=<number> "
              "--max_burst=<number> --max_quantum=<number>\n",
              argv[0]);
      exit(EXIT_FAILURE);
    default:
      abort();
    }
  }

  printf("************** Simulation Configuration *******\n");
  printf("Number of random processes: %d\n", num_rand_proc);
  printf("Max arrival time: %d\n", max_arrival);
  printf("Max burst time: %d\n", max_burst);
  printf("Max quantum time: %d\n", max_quantum);
  printf("************************************************\n");

  num_custom_proc = get_num_proc_from_file("custom_processes.txt");
  num_adtl_rand_proc = get_num_rdm_proc_from_file("random_processes.txt");
  tot_proc = num_rand_proc + num_custom_proc + num_adtl_rand_proc;
  Pool *fcfs_pool = create_pool(tot_proc);

  setup_random_processes(fcfs_pool, num_rand_proc, max_arrival, max_burst);
  setup_processes_from_file(fcfs_pool, num_rand_proc, "custom_processes.txt");
  setup_rdm_processes_from_file(fcfs_pool, num_rand_proc + num_custom_proc,
                                "random_processes.txt");

  Pool *sjf_pool = duplicate_pool(fcfs_pool);
  Pool *psjf_pool = duplicate_pool(fcfs_pool);
  Pool *ps_pool = duplicate_pool(fcfs_pool);
  Pool *pps_pool = duplicate_pool(fcfs_pool);
  Pool *rr_pool = duplicate_pool(fcfs_pool);
  Pool *lif_pool = duplicate_pool(fcfs_pool);
  Pool *plif_pool = duplicate_pool(fcfs_pool);
  Pool *lisc_pool = duplicate_pool(fcfs_pool);
  Pool *plisc_pool = duplicate_pool(fcfs_pool);

  printf("******************** Process Information **********************\n");
  print_heap(fcfs_pool);
  printf("***************************************************************\n");

  printf("******************** FCFS Simulation **************************\n");
  FCFSState *fcfs_state = malloc(sizeof(FCFSState));
  fcfs_state = fcfs_init(fcfs_pool);
  execute_fcfs(fcfs_state);
  printf("***************************************************************\n");

  printf("******************** SJF Simulation ***************************\n");
  SJFState *sjf_state = malloc(sizeof(SJFState));
  sjf_state = sjf_init(sjf_pool, tot_proc);
  execute_sjf(sjf_state);
  printf("***************************************************************\n");

  printf("******************** PSJF Simulation **************************\n");
  PSJFState *psjf_state = malloc(sizeof(PSJFState));
  psjf_state = psjf_init(psjf_pool, tot_proc);
  execute_psjf(psjf_state);
  printf("***************************************************************\n");

  printf("******************** PS Simulation ***************************\n");
  PSState *ps_state = malloc(sizeof(PSState));
  ps_state = ps_init(ps_pool, tot_proc);
  execute_ps(ps_state);
  printf("***************************************************************\n");

  printf("******************** PPS Simulation **************************\n");
  PPSState *pps_state = malloc(sizeof(PPSState));
  pps_state = pps_init(pps_pool, tot_proc);
  execute_pps(pps_state);
  printf("***************************************************************\n");

  max_quantum = rand() % max_quantum + 1; // Random quantum time
  printf("******************** RR Simulation with quantum = %d "
         "*********************\n",
         max_quantum);
  RRState *rr_state = malloc(sizeof(RRState));
  rr_state = rr_init(rr_pool, max_quantum);
  execute_rr(rr_state);
  printf("*********************************************************************"
         "***\n");

  printf("******************** LIF Simulation ***************************\n");
  LIFState *lif_state = malloc(sizeof(LIFState));
  lif_state = lif_init(lif_pool, tot_proc);
  execute_lif(lif_state);
  printf("***************************************************************\n");

  printf("******************** PLIF Simulation ***************************\n");
  PLIFState *plif_state = malloc(sizeof(PLIFState));
  plif_state = plif_init(plif_pool, tot_proc);
  execute_plif(plif_state);
  printf("***************************************************************\n");

  printf("******************** LISC Simulation ***************************\n");
  LISCState *lisc_state = malloc(sizeof(LISCState));
  lisc_state = lisc_init(lisc_pool, tot_proc);
  execute_lisc(lisc_state);
  printf("***************************************************************\n");

  printf("******************** PLISC Simulation ***************************\n");
  PLISCState *plisc_state = malloc(sizeof(PLISCState));
  plisc_state = plisc_init(plisc_pool, tot_proc);
  execute_plisc(plisc_state);
  printf("***************************************************************\n");

  return 0;
}
