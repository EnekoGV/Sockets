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

struct envio{
	int id;
	int a;
};

int main(int argc, char** argv){

	//SERVER CODE
	printf("Server opened...");

	struct sockaddr_in server;
	struct sockaddr_in client;
	struct envio envio;
	FILE *f;

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

	while(recvfrom(sd, &envio, sizeof(envio), 0, (struct sockaddr*)&client, &addrlen) > 0){
		if(envio.id == 1){
			f = fopen("1.txt", "wb");
			fwrite(&envio.a, sizeof(int), 1, f);
		}else if(envio.id == 2){
			f = fopen("2.txt", "wb");
			fwrite(&envio.a, sizeof(int), 1, f);
		}else{
			f = fopen("3.txt", "wb");
			fwrite(&envio.a, sizeof(int), 1, f);
		}
		fclose(f);
	}
	return (EXIT_SUCCESS);

}
