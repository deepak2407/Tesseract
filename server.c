/* TEAM TESSERACT
Neeraj Vihar Pedalanka 105163264
Venkata Deepak Nannapaneni 105188586
*/

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


void servicePlayers(int client_1, int client_2){
	int client_1_score = 0;
	int client_2_score = 0;
	char score[100];

	while(1){
		write(client_1, "You Can Now Play", strlen("You Can Now Play") + 1);
		if (read(client_1, score, 100) < 0){
			fprintf(stderr, "error reading score from player 1\n");
			exit(3);
		}else{
			printf("Score From TOTO: %s\n",score);
			client_1_score = client_1_score + atoi(score);
			printf("Total Score of TOTO: %d\n", client_1_score);
			printf("-------------------------\n");
			if(client_1_score >= 100){
				printf("\n");
				printf("TOTO reached 100 first and won the game\n");
				printf("-------------------------\n");
				write(client_1, "Game over : you won the game", strlen("Game over : you won the game") + 1);
				write(client_2, "Game over : you lost the game", strlen("Game over : you lost the game") + 1);
				return;
			}
		}

		write(client_2, "You Can Now Play", strlen("You Can Now Play") + 1);
		if (read(client_2, score, 100) < 0){
			fprintf(stderr, "error reading score from player 1\n");
			exit(3);
		}else{
			printf("Score from TITI: %s\n",score);
                        client_2_score = client_2_score + atoi(score);
                        printf("Total Score of TITI: %d\n", client_2_score);
			printf("-------------------------\n");
			if(client_2_score >= 100){
				printf("\n");
                                printf("TITI reached 100 first and won the game\n");
				printf("-------------------------\n");
                                write(client_2, "Game over : you won the game", strlen("Game over : you won the game") + 1);
                                write(client_1, "Game over : you lost the game", strlen("Game over : you lost the game") + 1);
				return ;
                        }
		}
	}
}


int main(int argc, char *argv[]){
	int sid, client_1, client_2, portNo;
	struct sockaddr_in servAddress;

	if(argc != 2){
		printf("PORT not given\n");
		exit(0);
	}
	//creating a TCP connection using IPV4
	if((sid = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("Could Not Create Socket\n");
		exit(1);
	}

	servAddress.sin_family = AF_INET;
	servAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	sscanf(argv[1], "%d", &portNo);
	servAddress.sin_port = htons((uint16_t)portNo);
	//binding server address to socket
	if(bind(sid, (struct sockaddr *) &servAddress, sizeof(servAddress)) < 0){
		printf("Unable To Bind With The Socket Created\n");
		exit(2);
	}
	//server starts listening on socket
	if(listen(sid, 5)< 0){
		printf("Call To listen() Function Failed\n");
		exit(3);
	}

	while(1){
		//extracts the connection from the client and returns file descriptor of connected socket
		client_1=accept(sid,(struct sockaddr*)NULL,NULL);
		printf("TOTO entered\nwaiting for Second Player\n");
		printf("-------------------------\n");
		client_2=accept(sid,(struct sockaddr*)NULL,NULL);
		printf("TITI entered\nboth Players are ready to play\n");
		printf("-------------------------\n");
		printf("Starting a Game for TOTO and TITI\n\n");
		printf("-------------------------\n");
		
		if(fork() == 0)
		{
			servicePlayers(client_1,client_2);
			close(client_1);
			close(client_2);
			exit(4);
		}
	}
	return 0;
}

