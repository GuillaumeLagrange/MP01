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
    NickMessage nick_mess (name);
    send_message(nick_mess);
    Message * msg = receive_message();
    if (msg->getHeader()->type != MSG_TYPE_ACKNOWLEDGE_CHANGE_NICK) {
        std::cerr << "Did not receive ack for changing name";
        exit(EXIT_FAILURE);
    }
}

template<int N>
void Client::send_message(MessageT<N>& msg)
{
    message_header_t * header_ptr = msg.getHeader();
    std::string * body = msg.getBody();
    ::send(client_socket, (void *) header_ptr, sizeof(*header_ptr), 0);
    for (int i = 0; i < N; i++) {
        const char * buffer = body[i].c_str();
        int buffer_length = body[i].length() + 1;
        ::send(client_socket, (void *) buffer,
                sizeof(char) * buffer_length, 0);
    }
}

Message * Client::receive_message()
{
    //Message * message;
    //message_header_t * hdr = Message::readHeader(client_socket);
    //switch (hdr->type) {
        //case(4):
            //// TODO : parse the body of the message !
            //message = (NickMessage *) message;
            //message = new NickMessage(hdr);
            //message->readBody(client_socket);
    //}
    ////message.readHeader(client_socket);
    ////message.readBody(client_socket);
    //handleMessage(message);
}
