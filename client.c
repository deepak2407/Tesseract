/* TEAM TESSERACT
Neeraj Vihar Pedalanka 105163264
Venkata Deepak Nannapaneni 105188586
*/

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
	char mssg[100];
	int servo, portNumbo;
	struct sockaddr_in servAddr;
	int die;

	if(argc != 3){
		printf("IP and PORT are required. Try again.\n",argv[0]);
		exit(0);
	}

	if ((servo=socket(AF_INET,SOCK_STREAM,0)) < 0){
		printf("Could not create the socket. Creation Failed. Try again.\n");
		exit(1);
	}

	servAddr.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumbo);
	servAddr.sin_port = htons((uint16_t)portNumbo);

	if(inet_pton(AF_INET, argv[1], &servAddr.sin_addr) < 0){
		printf("Calling the inet_pton() Failed. Try again.\n");
		exit(2);
	}

	if(connect(servo, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0){
		printf("Call to connect() has failed. Try again.\n");
		exit(3);
	}
//client conected to server and rolls its die

	srand(time(0));
	while(1){
		if (read(servo, mssg, 100) < 0){
			fprintf(stderr, "read() error.\n");
			exit(3);
		}
		if(!strcmp(mssg,"You Can Now Play")){
			sleep(1);
			die =rand()%10 + 1;
			printf("I got %d points this round\n", die);
			sprintf(mssg, "%d", die);
			write(servo, mssg, strlen(mssg) + 1);
		}else if(!strcmp(mssg,"Game over : you won the game")){
			printf("\n");
			printf("I Won the game\n");
			printf("-------------------------\n");
			break;
		}else if(!strcmp(mssg,"Game over : you lost the game")){
			printf("\n");
			printf("I Lost the game\n");
			printf("-------------------------\n");
			break;
		}
	}
	close(servo);
	return 0;
}
