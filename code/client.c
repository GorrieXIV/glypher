#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

//writing a message to auxillary file, preparing for encryption
void writeToFile (char* file, char* message) {
	FILE *f_dst = fopen(file, "wb");
	if(f_dst == NULL) {
  	printf("ERROR - Failed to open file for writing\n");
    exit(1);
	}

	// Write Buffer
	if(fwrite(message, 1, 1024, f_dst) != 1024) {
    printf("ERROR - Failed to write 1024 bytes to file\n");
    exit(1);
	}

	// Close File
	fclose(f_dst);
	f_dst = NULL;
}

int main(void) {
  int sockfd = 0,n = 0;
  char * recvBuff;
  struct sockaddr_in serv_addr;
 
  recvBuff = malloc(sizeof(char)*(1024));
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0) {
		printf("\n Error : Could not create socket \n");
    return 1;
  }
 
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(5000);
  serv_addr.sin_addr.s_addr = inet_addr("192.168.2.227");
 
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) {
  	printf("\n Error : Connect Failed \n");
    return 1;
  }
 
  while((n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0) {
  	recvBuff[n] = 0;
  	if(fputs(recvBuff, stdout) == EOF) {
      printf("\n Error : Fputs error");
    }
    printf("\n");
  }
 
  if( n < 0) {
  	printf("\n Read Error \n");
  }

	writeToFile("temp", recvBuff);	 
	free(recvBuff);
  return 0;
}
