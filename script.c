#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <sys/wait.h>

#define PORT 8809

int main(int argc, char** argv){	

	//SERVER CODE
	if (fork() == 0){

		printf("Server opened...");
		
		struct sockaddr_in server;
		struct sockaddr_in client;

		int sd = socket(AF_INET, SOCK_DGRAM, 0);
		if(sd == -1){
			perror("Error: Socket can not be opened. \n"); 
			exit(1); 
		}

		memset (&server, 0, sizeof(server));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(PORT); 	

		bind(sd,(struct sockaddr*)&server,sizeof(server));
		int addrlen = sizeof (struct sockaddr_in);
		
		int num;
		while(recvfrom(sd, &num, sizeof(int), 0, (struct sockaddr*)&client, &addrlen) > 0){
			printf("This is the message recieved: %i \n", num);		
		}
	}

	//CLIENT CODE	
	struct sockaddr_in client_2;
	struct sockaddr_in server_2;
	struct hostent *hp; 

	int fdc = socket(AF_INET, SOCK_DGRAM, 0);
	if(fdc == - 1){
		perror("Error: Socket can not be opened. \n"); 
		exit(1); 
	}
	
	hp = gethostbyname("localhost");
	if(hp == NULL){
		perror("Error: Can not get host information. \n"); 
	}

	//struct sockaddr_in client;
	memset (&client_2, 0, sizeof(client_2)); 
	client_2.sin_family = AF_INET;
	client_2.sin_addr.s_addr = htonl(INADDR_ANY);
	client_2.sin_port = htons(PORT);

	//struct sockaddr_in server; 
	memset (&server_2, 0, sizeof(server_2)); 
	server_2.sin_family = AF_INET;
	server_2.sin_addr.s_addr = ((struct in_addr*)(hp->h_addr))->s_addr;//htonl(SO_BROADCAST);//((struct in_addr*)(hp->h_addr))->s_addr;
	server_2.sin_port = htons(PORT);
	
	bind(fdc,(struct sockaddr*)&server_2, sizeof(server_2));
	
	int a = 1;
	while(1){
		sendto(fdc, &a, sizeof(int), 0, (struct sockaddr*)&server_2, sizeof(server_2));
		sleep(5);
	}
	return (EXIT_SUCCESS);	
	
	
}		
