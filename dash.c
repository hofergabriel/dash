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
  printf("%s\n",getenv("HOME"));
  if(strlen(dir)==0){ 
    printf("inside\n");
    chdir(getenv("HOME"));
  }
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

void REPL(){
  char * buf=NULL;
  size_t leng=32;
  char a[32], b[32];
  char str[] = "cmdnm\0pid\0cd\0systat\0exit\0";
  char cwd[PATH_MAX];

  for(;;){
    a[0]=b[0]='\0';
    getcwd(cwd, sizeof(cwd));  
    printf("%s> ", cwd);
    getline(&buf, &leng, stdin);
    sscanf(buf, "%s%s", a, b);

    if(!strcmp(a, str)) cmdnm(b);	
    else if(!strcmp(a, &str[6])) pid(b);
    else if(!strcmp(a, &str[10])) cd(b);
    else if(!strcmp(a, &str[13])) systat();
    else if(!strcmp(a, &str[20])) return;
    else otherwise(buf); 
  }
}

void main(){ 
  REPL(); 
}

