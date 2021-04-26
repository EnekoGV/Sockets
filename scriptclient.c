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
#include <time.h>

#define DST_PORT 8809
#define SRC_PORT 8810

struct envio{
	int id;
	int a;
};


int main(int argc, char** argv){	

	//CLIENT CODE	
	struct sockaddr_in client;
	struct sockaddr_in dest;
	struct hostent *hp; 
	struct envio envio;

	int fdc = socket(AF_INET, SOCK_DGRAM, 0);
	if(fdc == - 1){
		perror("Error: Socket can not be opened. \n"); 
		exit(1); 
	}
	
	hp = gethostbyname("parking1");
	if(hp == NULL){
		perror("Error: Can not get host information. \n"); 
	}
	int bc_perm = 1;
	//if(setsockopt(fdc, SOL_SOCKET,SO_BROADCAST,(void *) &bc_perm, sizeof(bc_perm)) < 0){
	//	perror("Error.");
	//}

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
	envio.id=1;
	while(1){
		srand(time(0));
		envio.a = rand() % 2;
		sendto(fdc, &envio, sizeof(envio), 0, (struct sockaddr*)&dest, sizeof(dest));
		sleep(5);
	}
	return (EXIT_SUCCESS);			
}		
