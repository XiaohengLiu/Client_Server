/*      Web Application HW5      */    
/*   Xiaoheng Liu    152005333   */


#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXPENDING 5
#define RCVBUFSIZE 1024 
#define ERR_EXIT(msg) { perror(msg); exit(1); }



//This Function is used to check which command we use 
//It is also be used in client.c
//can be optimized
int check(char buffer[]){
	char get[] = "GET";
	char ex[] = "EXIT";
	char bounce[] = "BOUNCE";
	if(buffer[0]==get[0]&&buffer[1]==get[1]&&buffer[2]==get[2]) return 1;
	if(buffer[0]==bounce[0]&&buffer[1]==bounce[1]&&buffer[2]==bounce[2]&&buffer[3]==bounce[3]&&buffer[4]==bounce[4]) return 2;
	if(buffer[0]==ex[0]&&buffer[1]==ex[1]&&buffer[2]==ex[2]&&buffer[3]==ex[3]) return 3;
	else return 0;
}


//TO check if the server has the request file
//This is the most stupid way to implement like this
//we have to use c only, or the send file function would be more elegent if I can use some dynamic language
int checkFile(char buffer[]){
	char file1[] = "text1";
	char file2[] = "text2";
	char file3[] = "text3";

	int i = 0;
	if(buffer[4]==file1[0]&&buffer[5]==file1[1]&&buffer[6]==file1[2]&&buffer[7]==file1[3]&&buffer[8]==file1[4]) return 1;
	 if(buffer[4]==file1[0]&&buffer[5]==file1[1]&&buffer[6]==file1[2]&&buffer[7]==file1[3]&&buffer[8]==file2[4]) return 2;
	 if(buffer[4]==file1[0]&&buffer[5]==file1[1]&&buffer[6]==file1[2]&&buffer[7]==file1[3]&&buffer[8]==file3[4]) return 3;
	return 0;
}

char *fileread(char name[]){
	char *pchBuf = NULL;
 	int  nLen = 0;
 	FILE *pF  = fopen(name, "r"); 

 	fseek(pF, 0, SEEK_END); 
	nLen = ftell(pF);  
	rewind(pF);    

 	pchBuf = (char*) malloc(sizeof(char)*nLen+1);
 	if(!pchBuf)
 	{
 	 perror("error¤Ÿ!\n");
 	 exit(0);
 	}

 	nLen = fread(pchBuf, sizeof(char), nLen, pF);

 	pchBuf[nLen] = '\0'; 
 	return pchBuf;
}


int main(int argc, char *argv[]) {
	

	int rv_sock, s_sock, port_num, msg_len;
	char buffer[RCVBUFSIZE];
	struct sockaddr_in serv_addr;
	if (argc != 2) {       /* Test for correct number of arguments */
		char msg[64]; memset((char *) &msg, 0, 64);
		sprintf(msg, "Usage: %s server_port\n", argv[0]);
		ERR_EXIT(msg);
	}


	rv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (rv_sock < 0) ERR_EXIT("ERROR opening socket");
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	port_num = atoi(argv[1]); /* First arg: server port num. */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port_num);


	if (bind(rv_sock,
		(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		ERR_EXIT("ERROR on binding");
	if (listen(rv_sock, MAXPENDING) < 0)
		ERR_EXIT("ERROR on listen");


	while ( 1 ) {
	/* Server runs forever */
		fprintf(stdout, "\nWaiting for client to connect...\n");
		s_sock = accept(rv_sock, NULL, NULL);

		if (s_sock < 0) ERR_EXIT("ERROR on accept new client");
		memset(buffer, 0, RCVBUFSIZE);
		msg_len = recv(s_sock, buffer, RCVBUFSIZE - 1, 0);
		
		if (msg_len < 0)
		ERR_EXIT("ERROR reading from socket");
		int a = 0;
			
		int i =0;
		
		fprintf(stdout, "Client's message:%s\n", buffer);
		a = check(buffer);


		if (a==1){     //GET function
			i = checkFile(buffer);
			if(i == 0){msg_len = send(s_sock, "no such file", 18, 0);}
			else if(i == 1){char name[] = "text1";char *prbuff = fileread(name);send(s_sock, prbuff, 180, 0);}
			else if(i == 2){char name[] = "text2";char *prbuff = fileread(name);send(s_sock, prbuff, 180, 0);}
			else if(i == 3){char name[] = "text3";char *prbuff = fileread(name);send(s_sock, prbuff, 180, 0);}
		}



		else if (a==2){     //BOUNCE function
                        int j = 0;
                        char temp[msg_len-3];
                        while(j<msg_len-7){
                                temp[j]=(char)buffer[j+7];

                                j=j+1;
                        }

                         msg_len = send(s_sock, temp, 180, 0);
                }



		
		else if (a==3){    //EXIT function
                         int k = 0;
                         char temp[msg_len-3];
						 temp[0] = NULL;
                         while(k<msg_len-5){
                                 temp[k]=(char)buffer[k+5];
 
                                 k=k+1;
                         }
						 if (temp[0] != NULL)
						 {
                    		printf("EXIT code is (if any)\n%s\n",temp);
							msg_len = send(s_sock, temp, 18, 0);
						 }
						 else
						 {	
							 printf("Normal");
							 msg_len = send(s_sock, "Normal Exit", 18, 0);

                        exit(0);
                        }
 
						 else{  
			msg_len = send(s_sock, "illegal command", 21, 0);
		}  
		if (msg_len < 0) ERR_EXIT("ERROR writing to socket");
		close(s_sock);
	}
}

