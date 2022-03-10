#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>

void CRC(char *dataword, char *divisor, int msglen, int keylen, char *FinalData) {
	char *input, *key;
	char *temp = malloc(sizeof(char)*30);
	char *quot = malloc(sizeof(char)*100);
	char *rem = malloc(sizeof(char)*30);
	char *key1 = malloc(sizeof(char)*30);
	
	input = dataword;
	key = divisor;
	strcpy(key1,key);
	
	for (int i=0; i<keylen-1; i++) {
		input[msglen+i]='0';
	}

	for (int i=0; i<keylen; i++) {
		temp[i]=input[i];
	}
	
	for (int i=0; i<msglen; i++) {
		quot[i]=temp[0];
		if(quot[i] == '0') {
			for (int j=0;j<keylen; j++)
				key[j]='0';
		}

		else {
			for (int j=0; j<keylen; j++) {
				key[j]=key1[j];
			}
		}
		
		for (int j=keylen-1;j>0; j--) {
			if(temp[j] == key[j])
				rem[j-1]='0'; 
			else
			 	rem[j-1]='1';
		}
		rem[keylen-1] = input[i+keylen];
		strcpy(temp,rem);
	}
	strcpy(rem,temp);
	
	int j = 0;
	for(int i=0; i<msglen; i++) {
		FinalData[j++] = input[i];
	}
	for(int i=0; i<keylen-1; i++) {
		FinalData[j++] = rem[i];
	}
	FinalData[j] = '\0';
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
		char *dataword = malloc(sizeof(char)*100);
		char *divisor = malloc(sizeof(char)*30);
		
		int n = read(new_socket, (void *)dataword, sizeof(dataword));
		dataword[n] = 0;
		if(strcmp(dataword, "EXIT") == 0)	break;
		
		int m = read(new_socket, (void *)divisor, sizeof(divisor));
		divisor[m] = 0;
		if(strcmp(divisor, "EXIT") == 0)	break;
		
		fprintf (stdout, "Received Dataword: %s\n", dataword);	
		fprintf (stdout, "Received Divisor: %s\n", divisor);
		
		int keylen=strlen(divisor);
		int msglen=strlen(dataword);
		int size = msglen + keylen - 1;
		
		char *FinalData = malloc(sizeof(char)*100);
		CRC(dataword, divisor, msglen, keylen, FinalData);
		fprintf (stdout, "Final Data: %s\n\n", FinalData);
		write (new_socket, (void *)FinalData, strlen(FinalData));
	}
}
