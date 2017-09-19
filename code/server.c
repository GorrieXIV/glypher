#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

//loading an encrypted message into buffer
void readFile (char* file, char* dest) {
	//char *source = NULL;
	FILE *fp = fopen(file, "r");
	if (fp != NULL) {
    /* Go to the end of the file. */
    if (fseek(fp, 0L, SEEK_END) == 0) {
    	/* Get the size of the file. */
      long bufsize = ftell(fp);
      if (bufsize == -1) { /* Error */ }

      /* Allocate our buffer to that size. */
      dest = malloc(sizeof(char) * (bufsize + 1));

      /* Go back to the start of the file. */
      if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

      /* Read the entire file into memory. */
      size_t newLen = fread(dest, sizeof(char), bufsize, fp);
      if ( ferror( fp ) != 0 ) {
      	fputs("Error reading file", stderr);
      } else {
      	dest[newLen++] = '\0'; /* Just to be safe. */
    	}
    }
    fclose(fp);
	}

	//free(source); /* Don't forget to call free() later! */
}

int main(void) {
  int listenfd = 0,connfd = 0;
  

  struct sockaddr_in serv_addr;
 
  char * sendBuff;  
  int numrv;  
 
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  printf("socket retrieve success\n");
  
  memset(&serv_addr, '0', sizeof(serv_addr));
  sendBuff = calloc(1024,sizeof(char));
      
  serv_addr.sin_family = AF_INET;    
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  serv_addr.sin_port = htons(5000);    
 
  bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
  
  if (listen(listenfd, 10) == -1) {
  	printf("Failed to listen\n");
  	return -1;
  } 
    
  char message[1024] = "whats good homeboy";
	//readFile("file", message);
	printf("%s\n", message);	


  while(1) {      
  	connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request
  
  	strcpy(sendBuff, message);
  	write(connfd, sendBuff, strlen(sendBuff));
 
  	close(connfd); 
		free(sendBuff);
  	sleep(1);
  } 
 
  return 0;
}
