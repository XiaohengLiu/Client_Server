   /*   Xiaoheng Liu    152005333   */

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define RCVBUFSIZE 1024 /* Size of receive buffer */
#define ERR_EXIT(msg) { perror(msg); exit(1); }



int check(char buffer[]){    // check what kind of operation is.
         char bounce[] = "BOUNCE"; 
		 char get[] = "GET";
         char ex[] = "EXIT";
         if(buffer[0]==get[0]&&buffer[1]==get[1]&&buffer[2]==get[2]) return 1;
         if(buffer[0]==bounce[0]&&buffer[1]==bounce[1]&&buffer[2]==bounce[2]&&buffer[3]==bounce[3]&&buffer[4]==bounce[4]) return 2;
         if(buffer[0]==ex[0]&&buffer[1]==ex[1]&&buffer[2]==ex[2]&&buffer[3]==ex[3]) return 3;
         else return 0;
 }


int main(int argc, char *argv[]) {
	int c_sock, port_num, msg_len;
	struct sockaddr_in serv_addr;
	struct hostent *serverIP;
	char buffer[RCVBUFSIZE];

	if (argc != 3) {
		/* Test for correct number of arguments */
		char msg[64]; memset((char *) &msg, 0, 64); /* erase */
		sprintf(msg, "Usage: %s serv_name serv_port\n", argv[0]);
		ERR_EXIT(msg);
	}
	

	while(1){
	serverIP = gethostbyname(argv[1]); /* 1st arg: server name */
	if (serverIP == NULL)
		ERR_EXIT("ERROR, server host name unknown");
	port_num = atoi(argv[2]); /* Second arg: server port num. */
	c_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);


	if (c_sock < 0) ERR_EXIT("ERROR opening socket");
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((char *) &(serverIP->h_addr),(char *) &serv_addr.sin_addr.s_addr, serverIP->h_length);
	serv_addr.sin_port = htons(port_num);
	

	if (connect(c_sock,
		(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		ERR_EXIT("ERROR connecting");

	
	fprintf(stdout, "User, enter your message: ");
	memset(buffer, 0, RCVBUFSIZE); /* erase */
	fgets(buffer, RCVBUFSIZE, stdin); /* read input */
	msg_len = send(c_sock, buffer, strlen(buffer), 0);
	int a = 0;
	a = check(buffer);

	if (msg_len < 0) ERR_EXIT("ERROR writing to socket");
	memset(buffer, 0, RCVBUFSIZE);
	msg_len = recv(c_sock, buffer, RCVBUFSIZE - 1, 0);
	


 
	if(a == 1){         //GET function
		printf("The content is: \n");
		printf("%s\n",buffer);
		continue;
		}
	

	else if(a==2){       //BOUNCE function
		fprintf(stdout, "BOUNCE is: %s\n", buffer);
		}
	
	
	else if(a==3){      //EXIT function
		fprintf(stdout, "EXIT CODE(if any): %s\n", buffer);
		exit(0);
		}

	else{				//illegal operations
		fprintf(stdout, "The server said: %s\n", buffer);
	}
	if (msg_len < 0) ERR_EXIT("ERROR reading from socket");
	close(c_sock);}
	exit(0);
}
