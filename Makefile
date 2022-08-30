all:
	gcc -Wall client.c -I ./include/demi -o client
	gcc -Wall server.c -I ./include/demi -o server

clean:
	rm client server
	