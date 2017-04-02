#include "client.h"
#include "protocol.hxx"

using namespace clt;

message_header_t * read_header(int sockfd)
{
    message_header_t * hdr = new message_header_t();
    int ret = ::read(sockfd, hdr, sizeof(message_header_t));
    if (ret == -1) {
        std::cerr << "Problem reading header" << std::endl;
        exit(EXIT_FAILURE);
    }
    return hdr;
}


Client::Client(const char* hostname, uint16_t port)
{
    /* Initializing server adress struct */
    server = sockaddr_in();
    struct hostent *hostinfo;

    server.sin_family = AF_INET;
    server.sin_port   = htons(port);
    hostinfo = gethostbyname(hostname);

    if (hostinfo == NULL) {
        throw ConnectionException();
    }

    server.sin_addr = *(struct in_addr *) hostinfo->h_addr;

    /* Creating socket */
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
}

Client::~Client()
{
    int ret = ::close(client_socket);
    if (ret != 0) {
        throw ConnectionException();
    }
}

void Client::connect()
{
    ::connect(client_socket, (struct sockaddr *) &server, sizeof(server));
}

void Client::change_nickname(std::string name)
{
    NickMessage nick_mess (name);
    send_message(nick_mess);
    receive_message();
}

void Client::change_channel(std::string channel)
{
    JoinMessage join_mess(channel);
    send_message(join_mess);
    receive_message();
}

template<int N>
void Client::send_message(MessageT<N>& msg)
{
    message_header_t * header_ptr = msg.getHeader();
    std::vector<std::string>  body = msg.getBody();
    ::send(client_socket, (void *) header_ptr, sizeof(*header_ptr), 0);
    int len = 0;
    for (int i=0; i<N; i++) {
        len += body[i].length() + 1;
    }
    char * buffer = (char *) malloc(len);
    char * current_buffer = buffer;
    for (int i=0; i<N; i++) {
        buffer = strcpy(current_buffer, body[i].c_str());
        current_buffer += body[i].length() + 1;
    }
    int ret = ::send(client_socket, (void*) buffer, len, 0);
    if (ret == -1)
        throw MessageException();
    free(buffer);
}

void Client::receive_message()
{
    message_header_t * hdr = read_header(client_socket);
    //printf("Received type %d message\n",hdr->type);
    switch (hdr->type) {
        case(MSG_TYPE_ACKNOWLEDGE_CHANGE_NICK): {
            AckNickMessage * message = new AckNickMessage(hdr);
            message->readBody(client_socket);
            message->handleMessage();
            delete message;
            break;
        }
        case(MSG_TYPE_MESSAGE): {
            TextMessage * message = new TextMessage(hdr);
            message->readBody(client_socket);
            message->handleMessage();
            delete message;
            break;
        }
        case(MSG_TYPE_ACKNOWLEDGE_JOIN): {
            AckJoinMessage * message = new AckJoinMessage(hdr);
            message->readBody(client_socket);
            message->handleMessage();
            delete message;
            break;
        }
    }
};
