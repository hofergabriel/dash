#include <stdlib.h>
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/wait.h> 

#define MAX 100

int main(int argc, char argv[]) 
{
   char  line[MAX]; 
   char *args[MAX]; 
   int   num_args; 
   int   i; 
   int   waitpid; 
   int   childpid; 
   int   status; 
   while (1) 
   { 
      printf("Enter command: "); 
      gets(line);	// I know gets() is unsafe!!
      if(strcmp(line, "quit") == 0) 
         break; 
      num_args = 0; 
      args[num_args] = strtok(line, " "); 
      while (args[num_args] != NULL) 
      { 
         num_args++; 
         args[num_args] = strtok(NULL, " "); 
      } 
      num_args--; 
      printf("About to exec the following: "); 
      for (i = 0; i <= num_args; i++) 
      { 
         printf(args[i]); 
         printf(" "); 
      } 
      printf("\n"); 
      printf("\n"); 
      childpid = fork(); 
      if (childpid == 0) 
      { 
         execvp(args[0], args); 
         perror("Exec failed: "); 
         exit(5); 
      } 
      waitpid = wait(&status); 
      printf("Shell process %d exited with status %d\n", waitpid, (status >> 8)); 
   } 
   return(0);
} 

