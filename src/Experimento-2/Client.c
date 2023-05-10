#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 54321
#define MAX_LINE 256

int main (int argc, char* argv[]) {
    FILE *fp;
    struct hostent *hp;
    struct sockaddr_in sin;
    char *host;
    char buf[MAX_LINE];
    int s;
    int len;

    if (argc == 2) {
        host = argv[1];
    } else {
        fprintf(stderr, "Passe o endereço IPv4 do servidor em dotted notation\n");
        exit(1);
    }

    // traduz o nome do host em um endereço IP
    hp = gethostbyname(host);
    if (!hp) {
        fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
        exit(1);
    }

    // Le o endereço IP fornecido em argc em dotted notation e converter para binario.
    bzero((char*)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    if (inet_pton(AF_INET, host, &(sin.sin_addr)) <= 0) {
        fprintf(stderr, "Endereço IP inválido: %s\n", host);
        exit(1);
    }
    sin.sin_port = htons(SERVER_PORT);

    // active open
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("simple-talk: socket");
        exit(1);
    }
    if (connect(s, (struct sockaddr*)&sin, sizeof(sin)) <0) {
        perror("simple-talk: connect");
        close(s);
        exit(1);
    }

    // main loop: recebe e envia linhas de texto
    while (fgets(buf, sizeof(buf), stdin)) {
        buf[MAX_LINE-1] = '\0';
        len = strlen(buf) + 1;
        send(s, buf, len, 0);
    }
}