#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <demi/libos.h>
#include <demi/sga.h>
#include <demi/wait.h>

#define BUFSZ 512

void logexit(const char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

int addrparse(const char *addrstr, const char *portstr,
              struct sockaddr_storage *storage) {
    if (addrstr == NULL || portstr == NULL) {
        return -1;
    }

    uint16_t port = (uint16_t)atoi(portstr); // unsigned short
    if (port == 0) {
        return -1;
    }
    port = htons(port); // host to network short

    struct in_addr inaddr4; // 32-bit IP address
    if (inet_pton(AF_INET, addrstr, &inaddr4)) {
        struct sockaddr_in *addr4 = (struct sockaddr_in *)storage;
        addr4->sin_family = AF_INET;
        addr4->sin_port = port;
        addr4->sin_addr = inaddr4;
        return 0;
    }

    struct in6_addr inaddr6; // 128-bit IPv6 address
    if (inet_pton(AF_INET6, addrstr, &inaddr6)) {
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *)storage;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_port = port;
        // addr6->sin6_addr = inaddr6
        memcpy(&(addr6->sin6_addr), &inaddr6, sizeof(inaddr6));
        return 0;
    }

    return -1;
}

void usage(int argc, char **argv) {
	printf("usage: %s <server IP> <server port>\n", argv[0]);
	printf("example: %s 127.0.0.1 51511\n", argv[0]);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	if (argc < 3) {
		usage(argc, argv);
	}

	struct sockaddr_storage storage;
	if (0 != addrparse(argv[1], argv[2], &storage)) {
		usage(argc, argv);
	}

	int s;
	// s = socket(storage.ss_family, SOCK_STREAM, 0);
	s = demi_socket(storage.ss_family, SOCK_STREAM, 0);
	if (s == -1) {
		logexit("socket");
	}
	struct sockaddr *addr = (struct sockaddr *)(&storage);
	if (0 != demi_connect(s, addr, sizeof(storage))) {
		logexit("connect");
	}

	char buf[BUFSZ];
	
	while (1) {

		memset(buf, 0, BUFSZ);
		fgets(buf, BUFSZ-1, stdin);
		
		//envia os dados para o servidor
		size_t count = send(s, buf, strlen(buf)+1, 0);

		//sai do programa se a quantidade de dados enviados for diferente do que deveria
		if (count != strlen(buf)+1) {
			logexit("send");
		}

		memset(buf, 0, BUFSZ);
		unsigned total = 0;
		
		count = recv(s, buf + total, BUFSZ - total, 0);
		
		//count = 0 apenas se a conex??o tiver sido fechada, entao o programa deve fechar
		if (count == 0) {
			break;
			close(s);
		}	
		puts(buf);
	}

	exit(EXIT_SUCCESS);
}
