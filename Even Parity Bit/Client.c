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
		char msg[100];
		fprintf(stdout, "Enter your Message: ");
		scanf("%s", msg);

		write(sockfd, (void *)msg, strlen(msg));
		
		if(strcmp(msg, "EXIT") == 0)	break;
		
		int n = read(sockfd, (void *)msg, sizeof(msg));
		msg[n] = 0;
		fprintf (stdout, "Returned message: %s\n\n", msg);
	}
	close (sockfd);
 	return 0;
}
