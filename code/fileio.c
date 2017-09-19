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

void writeToFile (char* file, char* message, int buffsize);
void fetchEmail (char *to, char *recp);

void sendMessage (char *recp, char *message) {
	writeToFile("msg", message, sizeof(message));
	char *to;
	to = malloc (sizeof(char) * 25);
  fetchEmail(to, recp);

	char cmd1[100];
	char cmd2[100];

	sprintf(cmd2, "gpg -e -r \"%s\" msg", recp);
	system(cmd2);

  sprintf(cmd1,"sendmail %s < %s", to, "msg.tar.gpg"); // prepare command.
  system(cmd1); 
}

void readFile (char* file, char* dest, int buffsize) {

	FILE *fp = fopen(file, "r");
	if (fp != NULL) {

		int i=0;
		int c;
		while ((c = fgetc(fp)) != EOF) {
			dest[i] = c;
			i++;
		}
		
    if ( ferror( fp ) != 0 ) {
    	fputs("Error reading file", stderr);
    }
    fclose(fp);
	} 
}

void writeToFile (char* file, char* message, int buffsize) {
	FILE *f_dst = fopen(file, "w");
	if(f_dst == NULL) {
  	printf("ERROR - Failed to open file for writing\n");
    exit(1);
	}

	if(fputs(message, f_dst) < 0) {
    printf("ERROR - Failed to write 1024 bytes to file\n");
    exit(1);
	}

	fclose(f_dst);
	f_dst = NULL;
}

long fileSize (char* file) {
	FILE *fp = fopen(file, "r");

	fseek(fp, 0L, SEEK_END);
	long bufsize = ftell(fp);
	if (bufsize == -1) { printf("buff size error\n");/* Error */ }

	fclose(fp);
	return bufsize;
}

void initKeyRing () {
	system("ls data/conts/ > data/contacts");
}

void fetchEmail (char *to, char *recp) {	
	char *fileName;
	char *fileContent;
	fileName = malloc(sizeof(char) * (sizeof(recp) + 4));
	
	sprintf(fileName,"%s.key", recp);

	fileContent = malloc(sizeof(char) * (fileSize(fileName) + 1));
	readFile(fileName, fileContent, fileSize(fileName) + 1);

	int q = 0;
	char ch = fileContent[q];
	while (ch != '\n') {
		to[q] = fileContent[q];
		q ++;
		ch = fileContent[q];
	}
}
