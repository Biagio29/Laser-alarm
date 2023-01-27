// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 1032
int main(int argc, char const *argv[]){
	const char* ip = "192.168.1.75";
	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	char risposta = 1;
	char buffer[1024] = {0};
	//controllo se la socket è stata creata correttamente
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("Errore nella creazione del Socket \n");
		return -1; //chiude il programma
	}
	printf("Socket creato \n");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// inet_pton converte gli IPv4 e gli IPv6 da testo a formato binario e controlla se l'ip è giusto
	if(inet_pton(AF_INET, ip , &serv_addr.sin_addr) <= 0){
		printf("\nIndirizzo non valido / Indirizzo non supportato \n");
		int tmp;
		scanf("%d", &tmp);
		return -1; //chiude il programma
	}

	// controllo se la connessione con l'host è riuscita
	if((client_fd = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0){
		printf("\nConnessione Fallita \n");
		int tmp = 0;
		scanf("%d", &tmp);
		return -1; //chiude il programma
	}

	int flag = 0;
	while (!flag)
	{
		printf("Cosa vuoi fare? \n1-attiva l'allarme\n2-disattiva l'allarme:\ninserisci la tua risposta: ");
		scanf("%c", &risposta);
		send(sock, &risposta, strlen(&risposta), 0);
		printf("Invio riuscito\n");
		printf("\nVuoi uscire? \n[1-si / 0-no]:");
		scanf(" %u", &flag);
		if( flag != 1) flag = 0;
		printf("\n");
	}
	close(client_fd); //chiude la connessione
	return 0;
}
