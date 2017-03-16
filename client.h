#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include "message.h"

namespace clt {
    class Client {
        private:
            //std::string name;
            sockaddr_in server;
            int client_socket;

        public:
            Client(const char* hostname, uint16_t port);
            ~Client();
            void connect();
            void change_nickname(std::string name);
            void send_message(Message& msg);
            Message receive_message();
    };
};

#endif
