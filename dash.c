#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>


void cmdnm(char * pid){
  char path[256], scratch[256], nm[256];
  sprintf(path, "/proc/%s/stat", pid);
  FILE * fp = fopen(path, "r");	
  if(fp==NULL) return;
  fscanf(fp, "%s%s", scratch, nm);
  printf("%s\n", nm);
  fclose(fp);
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
    if(fp==NULL) return;
    if(fp){ 
      fscanf(fp, "%s%s", scratch, nm);
      printf("pid: %s cmdnm: %s\n",scratch,nm);
      if(!strcmp(command2,nm))
        printf("\t\t%s",nm);
    }
    fclose(fp);
  }
}

void cd(char * dir){
  if(chdir(dir)!=0)
    perror("error changing directories\n");
}

void showfile(char * filename){
  char * buf=NULL;
  size_t leng=32;
  char path[256];
  sprintf(path, "/proc/%s", filename);
  FILE * fp = fopen(path,"r");
  if(fp==NULL) return;
  while(getline(&buf, &leng, fp) != -1)
    printf("%s",buf);
  fclose(fp);
}


void systat(){
  char nm[] = "meminfo\0cpuinfo\0uptime\0version\0";
  showfile(nm);
  showfile(&nm[8]);
  showfile(&nm[16]);
  showfile(&nm[23]);
}


// proc/meminfo
// proc/version


void REPL(){
  char * buf=NULL;
  size_t leng=32;
  char a[32], b[32];
  char str1[] = "cmdnm";
  char str2[] = "pid";
  char str3[] = "cd";
  char str4[] = "exit"; 
  char str5[] = "systat";
  char cwd[PATH_MAX];

  for(;;){
    getcwd(cwd, sizeof(cwd));  
    printf("%s> ", cwd);
    getline(&buf, &leng, stdin);
    sscanf(buf, "%s%s", a, b);

    if(!strcmp(a, str1)) cmdnm(b);	
    if(!strcmp(a, str2)) pid(b);
    if(!strcmp(a, str3)) cd(b);
    if(!strcmp(a, str4)) return;
    if(!strcmp(a, str5)) systat();
    else {
      system(buf); 

    }
  }
}


/***********************************************************/

/***********************************************************/
void main(){
  REPL();
}



