#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(
        stderr,
        "Usage: ./memory-user <megabytes allocated> <run time (seconds)>\n");
    exit(1);
  }
  int mypid;
  int req_mb, run_time, arrlen;
  int i = 0;
  int *arr;
  double time_spent;
  req_mb = atoi(argv[1]);
  run_time = atoi(argv[2]);
  arrlen = req_mb * 1024 * 1024 / sizeof(int);
  arr = (int *)malloc(req_mb * 1024 * 1024);

  printf("memory-user pid: %d\n", getpid());

  for (int j = 0; j < arrlen; j++) {
    arr[j] = 0;
  }

  clock_t start = clock();
  while (1) {
    time_spent = (double)(clock() - start) / CLOCKS_PER_SEC;
    if (time_spent >= run_time) {
      break;
    }
    arr[i]++;
    i = (i + 1) % arrlen;
  }

  free(arr);
  return 0;
}
