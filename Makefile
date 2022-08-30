all:
	gcc -Wall client.c -I ./include -o client
	gcc -Wall server.c -I ./include -o server

clean:
	rm client server
	
