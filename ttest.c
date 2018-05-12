#include "types.h"
#include "stat.h"
#include "user.h"

#define S (10000)

int buf[S];

int
main(int argc, char *argv[])
{
  uint s, re, ru;
  if (fork() == 0) {
    for (int i = 1; i < atoi(argv[2]); i++) {
      for (int j = 0; j < S; j++) {
        buf[j] += i + j;
      }
    }

    int s = 0;
    for (int j = 0; j < S; j++) {
      s += buf[j];
    }
    printf(1, "Done: %d\n", s);
    sleep(atoi(argv[1]));
  } else {
    int pid = wait2(&re, &ru, &s);
    printf(1, "%d -> %d, %d, %d\n", pid, re, ru, s);
  }
  exit();
}
