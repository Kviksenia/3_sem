#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char *x;
	char *y;
	int status;
	char **cmd;
	int i=1;

  	while(1) {
		cmd = (char**)malloc(sizeof(char*)*100);
		x = (char*)malloc(sizeof(char)*100);

		printf("ENTER CMD OR 'q' >>>\n");

		scanf("%[^\n]%*c", x);

		if (*(x) == 'q') { return 0;}

		x = strdup(x);


		const pid_t pid = fork();


		if (pid < 0) {
			printf("fork() error\n");
		}


  		if (pid) {
  			waitpid(pid, &status, 0);
          		printf("status: %d\n", status);
          		printf("Return code: %d\n", WEXITSTATUS(status));
		}

		if (!pid) {
			y=strtok(x, " ,");
            *(cmd)=y;
			while (y != NULL) {
				printf("y: %s", y);
				y=strtok(NULL, ", ");
				*(cmd+i)=y;
				i++;
			}


			execvp(cmd[0], cmd);
			printf("exec* failed\n");
		}
  	}

  return 0;
}

