//server.cpp
#include <zmq.hpp>
#include <string.h>
#include <unistd.h>

char cmd[500], ret[50000];

int main () {
    // Prepare our context and socket
    void *context =  zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(responder, "tcp://*:5555");
    printf("%d\n", rc);
    while (true) {
        memset(cmd, 0, sizeof(cmd));
        zmq_recv(responder, cmd, 1000, 0);
        printf("received %s\n", cmd);
        sleep(1);
        zmq_send(responder, cmd, strlen(cmd), 0);
    }
    return 0;
}
