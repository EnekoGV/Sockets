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

#define DST_PORT 8809
#define SRC_PORT 8810

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
		server.sin_port = htons(DST_PORT); 	

		bind(sd,(struct sockaddr*)&server,sizeof(server));
		int addrlen = sizeof (struct sockaddr_in);
		
		int num;
		while(recvfrom(sd, &num, sizeof(int), 0, (struct sockaddr*)&client, &addrlen) > 0){
			printf("This is the message recieved: %i \n", num);		
		}
	}

	//CLIENT CODE	
	struct sockaddr_in client;
	struct sockaddr_in dest;
	struct hostent *hp; 

	int fdc = socket(AF_INET, SOCK_DGRAM, 0);
	if(fdc == - 1){
		perror("Error: Socket can not be opened. \n"); 
		exit(1); 
	}
	
	hp = gethostbyname("broadcast");
	if(hp == NULL){
		perror("Error: Can not get host information. \n"); 
	}
	int bc_perm = 1;
	if(setsockopt(fdc, SOL_SOCKET,SO_BROADCAST,(void *) &bc_perm, sizeof(bc_perm)) < 0){
		perror("Error.");
	}

	//struct sockaddr_in client;
	memset (&client, 0, sizeof(client)); 
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = htonl(INADDR_ANY);
	client.sin_port = htons(SRC_PORT);

	//struct sockaddr_in server; 
	memset (&dest, 0, sizeof(dest)); 
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = ((struct in_addr*)(hp->h_addr))->s_addr;
	dest.sin_port = htons(DST_PORT);
	
	bind(fdc,(struct sockaddr*)&client, sizeof(client));
	
	int a = 1;
	while(1){
		sendto(fdc, &a, sizeof(int), 0, (struct sockaddr*)&dest, sizeof(dest));
		sleep(5);
	}
	return (EXIT_SUCCESS);	
	
	
}		
