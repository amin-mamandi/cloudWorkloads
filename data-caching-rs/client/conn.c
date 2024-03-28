
#include "conn.h"

extern int verbose;

struct conn* createConnection(const char* ip_address, int port, int protocol, int naggles) {

  struct conn* connection = malloc(sizeof(struct conn));
  memset(connection, 0, sizeof(struct conn));

  connection->sock = openUnixSocket("/tmp/memcached.sock");
  connection->protocol = UNIX_SOCKET_MODE;
  static int uid_gen;
  connection->uid = uid_gen;
  uid_gen++;
  return connection;
}//End createConnection()

int openUnixSocket(const char* path) {
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("ERROR: Couldn't create a Unix socket\n");
        exit(-1);
    }

    struct sockaddr_un server_address;
    memset(&server_address, 0, sizeof(struct sockaddr_un));
    server_address.sun_family = AF_UNIX;
    strncpy(server_address.sun_path, path, sizeof(server_address.sun_path) - 1);

    int error = connect(sock, (struct sockaddr*)&server_address, sizeof(server_address));
    if (error < 0) {
        printf("Connection error\n");
        exit(-1);
    }

    return sock;
}

