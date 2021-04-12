////////////////////////////////////////////////////////////////////
//                     Pipe example program                       //
//                     cat pipe.txt | sort                        //
//   Just an example, but think about the question of whether     //
//   child or grandchild should be input of pipe or should both   //
//   be children of same parent?                                  //
////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
  {
  int fd_pipe[2];
  int pid1;
  int pid2;
  int status;
  int wpid;

  pid1 = fork();
  if (pid1 == 0)
    {
    // child process executes here for input side of pipe

    pipe(fd_pipe);           // create pipe

    pid2 = fork();
    if (pid2 == 0)
      {
      // grandchild process executes here for output side of pipe
      close(1);              // close standard output
      dup(fd_pipe[1]);       // redirect the output
      close(fd_pipe[0]);     // close unnecessary file descriptor
      close(fd_pipe[1]);     // close unnecessary file descriptor
      execl("/bin/sh", "sh ", "-c", "cat pipe.txt", NULL);
      printf("execl of /bin/cat failed\n");
      exit(1);
      }

    // back to process for input side of pipe

    close(0);              // close standard input
    dup(fd_pipe[0]);       // redirect the input
    close(fd_pipe[0]);     // close unnecessary file descriptor
    close(fd_pipe[1]);     // close unnecessary file descriptor
    execl("/bin/sh", "sh", "-c", "sort", 0);
    printf("execl of /bin/sort failed\n");
    exit(1);
    }
  else
    {
    // parent process executes here
    wpid = wait(&status);
    printf("The child process id number is %d [%d]\n", pid1, wpid);
    }
  }
