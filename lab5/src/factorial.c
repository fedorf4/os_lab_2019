#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>

int result = 1;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

struct MultiArgs {
  int mod;
  int begin;
  int end;
};

void multiply(const struct MultiArgs *args)
{
  int i;
  int prod = 1;
  for(i = (*args).begin; i < (*args).end; i++) {
    if (i % (*args).mod == 0)
        continue;
	  prod *= (i % (*args).mod);
    prod %= (*args).mod;
  }
  pthread_mutex_lock(&mut);
  result *= prod;
  result %= (*args).mod;
  pthread_mutex_unlock(&mut);
}


int main(int argc, char **argv) {
  
  int k = -1; // factoral argument
  int threads_num = -1;
  int mod = -1; // mod base

  while (1) {
    int current_optind = optind ? optind : 1;
    
    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);
    

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            k = atoi(optarg);
            if (k<0)
            {
              printf("Number must be positive");
              return -1;
            }
            break;
          case 1:
            threads_num = atoi(optarg);
            if(threads_num < 1)
            {
              printf("Pnum of array must be positive!");
              return -1;
            }
            break;
          case 2:
            mod = atoi(optarg);
            if (mod < 1)
            {
              printf("Mod must be positive!");
              return -1;
            }
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (k == -1 || threads_num == -1 || mod == -1) {
    printf("Usage: %s --k \"num\" --pnum \"num\" --mod \"num\" \n",
           argv[0]);
    return 1;
  }

  int j;

  pthread_t threads[threads_num];

  int array_step = k / threads_num;
  int last_step = array_step + k % threads_num;

  struct MultiArgs args[threads_num];

  for (j = 0; j < threads_num; j++)
  {
    int local_step = j < threads_num - 1 ? array_step : last_step;
    args[j].begin = j * array_step + 1;
    args[j].end = j * array_step + local_step + 1;
    args[j].mod = mod;
    if (pthread_create(&threads[j], NULL, (void*)multiply, (void*)&args[j]))
    {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  for (j = 0; j < threads_num; j++) {
    pthread_join(threads[j], NULL);
  }

  printf("result: %d\n", result);

  return -1;
}