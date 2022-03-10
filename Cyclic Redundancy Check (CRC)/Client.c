#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>

int main(int argc, char *argv[]) {
	int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("Error opening socket");
		exit(1);	
	}
	
	struct sockaddr_un addr1;
	addr1.sun_family = AF_UNIX;
	strcpy(addr1.sun_path, "tmp");

	connect(sockfd, (struct sockaddr*) &addr1, sizeof(addr1));

	while(1) {
		char *dataword = malloc(sizeof(char)*100);
		char *divisor = malloc(sizeof(char)*30);
		
		fprintf(stdout, "Enter Dataword: ");
		scanf("%s", dataword);
		write(sockfd, (void *)dataword, strlen(dataword));
		if(strcmp(dataword, "EXIT") == 0)	break;
		
		fprintf(stdout, "Enter Divisor: ");
		scanf("%s", divisor);
		write(sockfd, (void *)divisor, strlen(divisor));
		if(strcmp(divisor, "EXIT") == 0)	break;
		
		int keylen=strlen(divisor);
		int msglen=strlen(dataword);
		int size = msglen + keylen - 1;
		
		char FinalData[100];
		int n = read(sockfd, (void *)FinalData, sizeof(FinalData));
		FinalData[n] = 0;
		fprintf (stdout, "Final Data: %s\n\n", FinalData);
	}
	close (sockfd);
 	return 0;
}
