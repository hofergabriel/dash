#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>


void cmdnm(char * pid){
  char path[256], scratch[256], nm[256];
  sprintf(path, "/proc/%s/stat", pid);
  FILE *fp = fopen(path, "r");	
  fscanf(fp, "%s%s", scratch, nm);
  printf("%s\n", nm);
}

void pid(char * command) {
  printf("%s\n\n",command);
  char pid[33], command2[256];
  sprintf(command2, "(%s)", command);
  printf("%s\n\n",command2);

  for(int i=0;i<13120;i++){
    sprintf(pid,"%d",i);

    char path[256], scratch[256], nm[256];
    sprintf(path, "/proc/%s/stat", pid);

    printf("path: %s\n",path);

    FILE *fp = fopen(path, "r");	
    if(fp){ 
      fscanf(fp, "%s%s", scratch, nm);
      printf("pid: %s cmdnm: %s\n",scratch,nm);
      if(!strcmp(command2,nm))
        printf("\t\t%s",nm);
    }
  }
}



void REPL(){
  char * buf=NULL;
  char a[32], b[32];
  char str1[] = "cmdnm";
  char str2[] = "pid";
  size_t leng=32;
  for(;;){
    printf("dash> ");
    getline(&buf, &leng, stdin);
    sscanf(buf, "%s%s", a, b);
    if(!strcmp(a, str1)){
      cmdnm(b);	
    } 
    if(!strcmp(a, str2)){
      printf("pid...\n");
      pid(b);
    }
    break;	
  }
  printf("\nexiting...\n");
}



void main(){
  REPL();
}



