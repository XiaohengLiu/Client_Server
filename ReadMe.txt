Introduction:
	This is a code about the communication between clients and server. I used C code to simulate behaviors of TCP/IP and Sockets.

Running environment:
I ran my code on Ubuntu 13.10, actually this code can run on any linux OS.

Here is the steps:
1.Open two terminals.

2.Run the "server.c" on the first terminal: 
a.Find the directory of server
b."gcc -o server server.c" (compile)
c."./server 5100"  (5100 is the port number, you can pick up any number, but it will be better to choose number more than 2000)

3.Run the "client.c" on the second terminal:
a. "gcc -o client client.c"
b. "./client xiao 5100"

4.Operating on the client like:
GET text1        (server will give the contents in text1)
BOUNCE OhMyGod   (server will display "OhMyGod") 
EXIT		 (server will display "Normal Exit")	
EXIT HeHe        (server will display "HeHe")
