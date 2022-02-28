#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>

//Even Parity Bit

int getParity(int n) {
	int pat = 0;
	while(n) {
		int bit = n%10;
		pat = pat^bit;
		n /= 10;
	}
	return pat;
}

void addParity(char *msg, char c) {
	int len = strlen(msg);
	msg[len] = c;
	msg[len+1] = '\0';
}

int main(int argc, char *argv[]) {
	unlink("tmp");
	int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("Error opening socket");
		exit(1);	
	}

	struct sockaddr_un addr1, addr2;
	addr1.sun_family = AF_UNIX;
	strcpy(addr1.sun_path, "tmp");

	bind(sockfd, (struct sockaddr *) &addr1, sizeof(addr1));
	listen(sockfd, 1);
	
	int l = sizeof(addr2);
	int new_socket = accept(sockfd, (struct sockaddr *) &addr2, &l);
	
	while(1) {
		char msg[100];
		int n = read(new_socket, (void *)msg, sizeof(msg));
		msg[n] = 0;

		if(strcmp(msg, "EXIT") == 0)	break;
		
		int i;
		sscanf(msg, "%d", &i);
		fprintf (stdout, "Received: %s\n", msg);	
		char pat = getParity(i)+'0';
		fprintf (stdout, "Parity: %c\n\n", pat);	
		addParity(msg, pat);
		write (new_socket, (void *)msg, strlen(msg));
	}
}
