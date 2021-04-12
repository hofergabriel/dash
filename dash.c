/***********************************************************************
 * Author: Gabriel Hofer                                                *
 * Date: February 25, 2021                                              *
 * Course: CSC-458                                                      *
 * Instructor: Dr. Karlsson                                             *
 ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <ctype.h>
#include <sys/wait.h>

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<time.h>

/**********************************************************************/
/*                            EXAM 1 CODE                             */
/**********************************************************************/


/***********************************************************************
 * Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
 * Copyright (C) 2001 by New Riders Publishing                          *
 * See COPYRIGHT for license information.                               *
 ***********************************************************************/
void get_RUSAGE_CHILDREN()
{
  struct rusage usage;
  getrusage (RUSAGE_CHILDREN, &usage);
  printf ("CPU time: %ld.%06ld sec user, %ld.%06ld sec system\n",
      usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,
      usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
}

void cmdnm(char * pid){
  char path[256], scratch[256], nm[256];
  sprintf(path, "/proc/%s/stat", pid);
  FILE * fp = fopen(path, "r");	
  if(fp==NULL) return;
  fscanf(fp, "%s%s", scratch, nm);
  printf("%s\n", nm);
  fclose(fp);
}

void cd(char * dir){
  if(strlen(dir)==0)
    chdir(getenv("HOME"));
  else if(chdir(dir)!=0) 
    perror("something went wrong\n");
}

void showfile(char * filename){
  printf("%s: \n",filename);
  char * buf=NULL;
  size_t leng=32;
  char path[256];
  sprintf(path, "/proc/%s", filename);
  FILE * fp = fopen(path,"r");
  if(fp==NULL) return;
  while(getline(&buf, &leng, fp) != -1)
    printf("%s",buf);
  printf("\n");
  fclose(fp);
}

void systat(){
  char nm[] = "version\0uptime\0meminfo\0cpuinfo\0";
  showfile(nm);
  showfile(&nm[8]);
  showfile(&nm[15]);
  showfile(&nm[23]);
}

/* https://pubs.opengroup.org/onlinepubs/009695399/functions/opendir.html */
void pid(char * command) {
  char pid[33], command2[256];
  sprintf(command2, "(%s)", command);
  DIR * dir;
  struct dirent *dp;
  if((dir = opendir("/proc")) == NULL){
    perror("Can't open directory\n");
    return;
  }
  while((dp = readdir(dir)) != NULL){
    char path[512], scratch[256], nm[256];
    sprintf(path, "/proc/%s/stat", dp->d_name);
    FILE *fp = fopen(path, "r");	
    if(fp==NULL) continue;
    fscanf(fp, "%s%s", scratch, nm);
    if(!strcmp(command2,nm))
      printf("%s\n",dp->d_name);
    fclose(fp);
  }
}

void otherwise(char * buf){
  int status;
  int pid = fork();
  if(pid!=0) printf("Child PID: %d\n",pid);
  if(pid==0){
    char * name[4];
    name[0] = "/bin/bash";
    name[1] = "-c";
    name[2] = buf;
    name[3] = NULL;
    execvp("/bin/sh",name);
  }
  get_RUSAGE_CHILDREN();
  waitpid(pid,&status,0);
}

/**********************************************************************/
/*                            EXAM 2 CODE                             */
/**********************************************************************/
/* for the PID manager */
#define MIN_PID 300
#define MAX_PID 500
char * pid_map;


int handle_lt(char * buf, int idx){ 
  printf("handle_lt\n");

  char a[32], b[32];
  strncpy(a, buf , idx );
  strncpy(b, buf + idx + 1, strlen(buf)-(idx+1)-1);
  a[idx]='\0';
  b[strlen(buf)-(idx+1)-1]='\0';
  //b[strlen(buf)-idx]=a[idx]='\0';
  printf("a: %s \n b: %s\n",a,b);

  /**********************************************************************/
  /*            The following code section is not mine                  */
  /*            or was modified from someone else's code                */
  /* https://www.cse.sdsmt.edu/ckarlsso/csc458/spring21/src/ALP/pipe1.c */
  /**********************************************************************/

  ////////////////////////////////////////////////////////////////////
  // Programmer: Jun Chen           Date: 10/15/92                  //
  // Purpose: Write a C program to demonstrate the use of fork(),   //
  // exec() and dup() for I/O redirection by doing the following.   //
  // When the program is run, take the command line argument after  //
  // the executable name as the name of another executable program  //
  // that is to be run in the background.  The next argument (if    //
  // it exists) is a file to which to redirect stdin, and the       //
  // next argument (again, if it exists) is a file to which to      //
  // redirect stdout.  After it forks the new process, the parent   //
  // should print a message to its standard output file giving      //
  // the process id of the child process.                           //
  ////////////////////////////////////////////////////////////////////

  int fpt1, fpt2, pid;
  pid = fork();
  if (pid == 0)
    {
    // child process executes here
      if ((fpt1 = open(b, O_RDONLY)) == -1) {
        printf("Unable to open %s for reading.\n", b);
        exit(-1);
      }
      close(0);       // close child standard input
      dup(fpt1);      // redirect the child input
      close(fpt1);    // close unnecessary file descriptor
      //execl("/bin/sh", argv[1], 0);
      execl("/bin/sh", "sh ", "-c", a, NULL);
    }
  else
    /* parent process executes here */
    printf("The child process id number is %d \n",pid);

  /**********************************************************************/
  /*                        End code section                            */
  /**********************************************************************/
  return 1;
}


int handle_gt(char * buf, int idx){ 
  printf("handle_gt\n");

  char a[32], b[32];
  strncpy(a, buf , idx );
  strncpy(b, buf + idx + 1, strlen(buf)-(idx+1)-1);
  a[idx]='\0';
  b[strlen(buf)-(idx+1)-1]='\0';
  //b[strlen(buf)-idx]=a[idx]='\0';
  printf("a: %s \n b: %s\n",a,b);

  /**********************************************************************/
  /*            The following code section is not mine                  */
  /*            or was modified from someone else's code                */
  /* https://www.cse.sdsmt.edu/ckarlsso/csc458/spring21/src/ALP/pipe1.c */
  /**********************************************************************/
  
  ////////////////////////////////////////////////////////////////////
  // Programmer: Jun Chen           Date: 10/15/92                  //
  // Purpose: Write a C program to demonstrate the use of fork(),   //
  // exec() and dup() for I/O redirection by doing the following.   //
  // When the program is run, take the command line argument after  //
  // the executable name as the name of another executable program  //
  // that is to be run in the background.  The next argument (if    //
  // it exists) is a file to which to redirect stdin, and the       //
  // next argument (again, if it exists) is a file to which to      //
  // redirect stdout.  After it forks the new process, the parent   //
  // should print a message to its standard output file giving      //
  // the process id of the child process.                           //
  ////////////////////////////////////////////////////////////////////

  int fpt1, fpt2, pid;
  pid = fork();
  if (pid == 0)
    {
        if ((fpt2 = creat(b, 0644)) == -1)
          {
          printf("Unable to open %s for writing.\n", b);
          exit(-1);
          }
        close(1);       // close child standard output 
        dup(fpt2);      // redirect the child output 
        close(fpt2);    // close unnecessary file descriptor
      execl("/bin/sh", "sh ", "-c", a, NULL);

    }
  else
    /* parent process executes here */
    printf("The child process id number is %d \n",pid);

  /**********************************************************************/
  /*                        End code section                            */
  /**********************************************************************/
  return 1;
}



int handle_pipe(char * buf, int idx){
  printf("handle pipe\n");

  char a[32], b[32];
  strncpy(a, buf , idx );
  strncpy(b, buf + idx + 1, strlen(buf)-(idx+1)-1);
  a[idx]='\0';
  b[strlen(buf)-(idx+1)-1]='\0';
  //b[strlen(buf)-idx]=a[idx]='\0';
  printf("a: %s \n b: %s\n",a,b);

  /**********************************************************************/
  /*            The following code section is not mine                  */
  /*            or was modified from someone else's code                */
  /* https://www.cse.sdsmt.edu/ckarlsso/csc458/spring21/src/ALP/pipe1.c */
  /**********************************************************************/
  
  ////////////////////////////////////////////////////////////////////
  //                     Pipe example program                       //
  //                     cat pipe.txt | sort                        //
  //   Just an example, but think about the question of whether     //
  //   child or grandchild should be input of pipe or should both   //
  //   be children of same parent?                                  //
  ////////////////////////////////////////////////////////////////////

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
      execl("/bin/sh", "sh ", "-c", a, NULL);
      printf("execl of /bin/cat failed\n");
      exit(1);
      }

    // back to process for input side of pipe

    close(0);              // close standard input
    dup(fd_pipe[0]);       // redirect the input
    close(fd_pipe[0]);     // close unnecessary file descriptor
    close(fd_pipe[1]);     // close unnecessary file descriptor
    execl("/bin/sh", "sh", "-c", b, 0);
    printf("execl of /bin/sort failed\n");
    exit(1);
    }
  else
    {
    // parent process executes here
    wpid = wait(&status);
    printf("The child process id number is %d [%d]\n", pid1, wpid);
    }

  /**********************************************************************/
  /*                        End code section                            */
  /**********************************************************************/

  return 1;
}

/**********************************************************************/
/*                        PID Manager                                 */
/**********************************************************************/
int handle_redirection(char * buf){
  for(int i=0;i<strlen(buf);i++){
    if( buf[i]-'|' == 0 ) return handle_pipe(buf, i);
    if( buf[i]-'<' == 0 ) return handle_lt(buf, i);
    if( buf[i]-'>' == 0 ) return handle_gt(buf, i);
  }
  return 0; 
}

int released_all(){
  for(int i=MIN_PID; i<=MAX_PID; i++)
    if(pid_map[i]!=0) return 0;
  return 1;
}

int get_random_pid(int lower, int upper){
  return lower + (rand() % (upper - lower)); 
}

int allocate_pid(){
  int pid = get_random_pid(MIN_PID, MAX_PID);
  pid_map[ pid ]=1;
  printf("allocated %d\n", pid);
  return pid;
}

int release_pid(int pid){
  if(pid_map[ pid ]){
    pid_map[ pid ]=0;
    printf("released %d\n", pid);
  }
}

int allocate_map(){
  pid_map = (char *) malloc(512);
}

void new_thread(){
  int pid = allocate_pid();
  sleep(get_random_pid(1,5));
  release_pid(pid);
}

void testpid(){
  allocate_map();
  for(int i=0;i<10;i++){
    int p = fork();
    if(p==0){ 
      srand(i);
      new_thread();
      break;
    }
  }
}

/**********************************************************************/
/*                      Memory Manager                                */
/**********************************************************************/
void memman(int addr){
  printf("The address %d contains :\n", addr);
  printf("page number = %d\n",addr / (1<<12));
  printf("offset = %d\n", addr % (1<<12));
}

void REPL(){
  char * buf=NULL;
  size_t leng=32;
  char a[32], b[32];
  char str[] = "cmdnm\0pid\0cd\0systat\0exit\0testpid\0memman\0";
  char cwd[PATH_MAX];

  for(;;){
    a[0]=b[0]='\0';
    getcwd(cwd, sizeof(cwd));  
    printf("%s> ", cwd);
    getline(&buf, &leng, stdin);
    sscanf(buf, "%s%s", a, b);

    char allspaces=1;
    for(int i=0;i<strlen(buf);i++)
      allspaces = (allspaces && isspace(buf[i]));
    if(allspaces) continue;
    if(!strcmp(a, str)) cmdnm(b);	
    else if(!strcmp(a, &str[6])) pid(b);
    else if(!strcmp(a, &str[10])) cd(b);
    else if(!strcmp(a, &str[13])) systat();
    else if(!strcmp(a, &str[20])) return;
    else if(handle_redirection(buf));
    else if(!strcmp(a, &str[25])) testpid();
    else if(!strcmp(a, &str[33])) memman(atoi(b));
    else otherwise(buf); 
  }
}

void main(){ 
  REPL(); 
}

