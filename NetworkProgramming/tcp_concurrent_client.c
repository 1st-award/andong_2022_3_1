#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_LEN 128

int main(int argc, char* argv[]) {
	int s, n, len_in, len_out;
	struct sockaddr_in server_addr;
	char *haddr;
	char buf[BUF_LEN+1];

	if(argc != 3) {
		printf("usage: %s address port\n", argv[0]);
		exit(0);
	}

	haddr = argv[1];

	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Can't create socket\n");
		exit(0);
	}

	bzero((char*)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(haddr);
	server_addr.sin_port = htons(atoi(argv[2]));
	
	if(connect(s, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
		printf("Can't connect\n");
		exit(0);
	}

	printf("Input any string: ");
	if(fgets(buf, BUF_LEN, stdin)) {
		buf[BUF_LEN] = '\0';
		len_out = strlen(buf);
	}
	else {
		printf("fgets eror\n");
		exit(0);
	}
	
	if(send(s, buf, len_out, 0) < 0) {
		printf("write error\n");
		exit(0);
	}

	printf("Echoed string: ");
	for(len_in = 0, n = 0; len_in < len_out; len_in += n) {
		if((n = recv(s, &buf[len_in], len_out - len_in, 0)) < 0) {
			printf("read error\n");
			exit(0);
		}
	}
	printf("%s", buf);
	close(s);
}
