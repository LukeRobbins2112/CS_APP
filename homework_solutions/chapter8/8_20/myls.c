/* Use execve to write a program called myls whose behavior is identical to the */
/* /bin/ls program. Your program should accept the same command line arguments, */
/* interpret the identical environment variables, and produce the identical */
/* output. */
/* The ls program gets the width of the screen from the COLUMNS environment */
/* variable. If COLUMNS is unset, then ls assumes that the screen is 80 */
/* columns wide. Thus, you can check your handling of the environment variables */
/* by setting the COLUMNS environment to something smaller than 80: */

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char ** argv, char **envp){

  char * columns = getenv("COLUMNS");
  printf("Columns: %s\n", columns);

  setenv("COLUMNS", "50", 1);

  columns = getenv("COLUMNS");
  printf("Columns: %s\n", columns);
  
  return 0;
}
