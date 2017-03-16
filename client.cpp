#include "client.h"
#include "protocol.hxx"

using namespace clt;

Client::Client(const char* hostname, uint16_t port)
{
    /* Initializing server adress struct */
    server = sockaddr_in();
    struct hostent *hostinfo;

    server.sin_family = AF_INET;
    server.sin_port   = htons(port);
    hostinfo = gethostbyname(hostname);

    if (hostinfo == NULL) {
        std::cerr << "Unkown host name : " << hostname << std::endl;
        exit(EXIT_FAILURE);
    }

    server.sin_addr = *(struct in_addr *) hostinfo->h_addr;

    /* Creating socket */
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
}

Client::~Client()
{
    int ret = ::close(client_socket);
    if (ret != 0) {
        std::cerr << "Error closing socket" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Client::connect()
{
    ::connect(client_socket, (struct sockaddr *) &server, sizeof(server));
}

void Client::change_nickname(std::string name)
{
    message_header_t header;
    header.length = sizeof(name);
    header.type = MSG_TYPE_CHANGE_NICK;
    header.body_checksum = fletcher16((uint8_t const *) &name, sizeof(name));
    header.head_checksum = fletcher16((uint8_t const *) &header,
            sizeof(message_header_t));

    ::send(client_socket, (void *) &header, sizeof(header), 0);
    ::send(client_socket, (void *) &name, sizeof(name), 0);
}

void Client::send_message(Message& msg)
{
    message_header_t * header_ptr = msg.getHeader();
    std::string body = msg.getBody();
    ::send(client_socket, (void *) header_ptr, sizeof(*header_ptr), 0);
    ::send(client_socket, (void *) body.c_str(),
            sizeof(char) * (body.length()+1), 0);
}

Message Client::receive_message()
{
    Message message;
    message.readHeader(client_socket);
    message.readBody(client_socket);
    return message;
}
