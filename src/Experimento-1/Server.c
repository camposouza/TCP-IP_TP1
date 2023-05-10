#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 54321
#define MAX_PENDING 5
#define MAX_LINE 256

int main() {
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    int buf_len, addr_len;
    int s, new_s;
    int len = sizeof(sin);

    // construir estrutura de dados de endereco
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);

    // setup passive open
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("simplex-talk: socket");
        exit(1);
    }
    if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
        perror("simplex-talk: bind");
        exit(1);
    }
    listen(s, MAX_PENDING);

    // aguarda conexao, recebe o texto. Imprime e reenvia o texto ao cliente
    while(1) {
        if ((new_s = accept(s, (struct sockaddr *)&sin, &addr_len)) < 0) {
            perror("simplex-talk: accept");
            exit(1);
        }
        while (buf_len = recv(new_s, buf, sizeof(buf), 0)) {
            fputs(buf, stdout);
        }
        close(new_s);
        send(new_s, buf, strlen(buf), 0);
    }
}