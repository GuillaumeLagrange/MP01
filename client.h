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
#include "nickmess.h"
#include "acknickmess.h"

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
            template<int N>
            void send_message(MessageT<N>& msg);
            void receive_message();
    };
};

#endif
