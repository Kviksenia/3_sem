#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int txd[2]; 
  int rxd[2]; 
} dpipe_t;

int main()
{
  char buf1[4096];
  buf1[0]='\0';
  char buf2[4096];
  int size;
  dpipe_t fd;
  
  if (pipe(fd.txd) < 0) 
  {
	printf("Pipe creation is failed!\n");
	return -1;
  }
  
  if(pipe(fd.rxd)<0)
  {
	  printf("Pipe creation is failed!\n");
	  return -2;
  }
  const pid_t pid = fork();
  if (pid < 0) {
    	printf("fork failed!\n");
    	return -1;
  }

  if (pid) {
    	close(fd.txd[0]);
    	close(fd.rxd[1]);
	while((size = read(0, buf1, sizeof(buf1)-1)) > 0) 
        {
                buf1[size] = 0;
                write(fd.txd[1], buf1, size);
                printf("Parent:Send to child: %s\n", buf1);
		read(fd.rxd[0], buf2, sizeof(buf2)-1);
		buf2[size]='\0';
		printf("Parent:Received from child: %s\n", buf2);
        }	
  }
  else {
	close(fd.txd[1]);
    	close(fd.rxd[0]);
    	while((size = read(fd.txd[0], buf1, sizeof(buf1)-1)) > 0) 
    	{
      		buf1[size] = '\0'; 
      		printf("Child:Received from parent: %s\n", buf1);
		
		read(0, buf2, sizeof(buf2) -1);
		buf2[size]=0;
		write(fd.rxd[1], buf2, size);
		printf("Child:Send to parent: %s\n", buf2);
    	}
  }
  return 0;
}

