#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define STOP_TIME 2

int main(int argc, char* argv[])

{
  int pipefds[2];

  if(pipe(pipefds) == -1)
  {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  int nombor;
  pid_t pid = fork();
  void sigint_handler(int sig);

  if (signal(SIGINT, sigint_handler) == SIG_ERR)
  {
     perror("signal");
     return 1;
  }


  if(pid == 0)
  {
    close(pipefds[0]);
    printf("Enter a number: ");
    scanf("%d", &nombor);
    write(pipefds[1], &nombor, sizeof(nombor));
    close(pipefds[1]);
    sleep(STOP_TIME);
    exit(EXIT_SUCCESS);
  }

  else if(pid > 0)
  {
    printf("Waiting for child process to finish.\n");
    wait(NULL);
    printf("Child process finished!\n");
    close(pipefds[1]);
    read(pipefds[0], &nombor, sizeof(nombor));
    close(pipefds[0]);
    if(nombor % 2 == 0)
    {
	printf("%d is not a prime number\n", nombor);
    }
    else
    {
	printf("%d is a prime number\n", nombor);
    }
  }

  else
  {
    printf("Unable to create child process.\n");
  }

return EXIT_SUCCESS;
}

void sigint_handler(int sig)
{
  printf("This is a special signal handler for SIGINT\n");
}
