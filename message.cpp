#include <unistd.h>
#include "message.h"

using namespace clt;

Message::Message() : header(new message_header_t), body(std::string()){ }

Message::~Message()
{
    delete header;
}

void Message::readHeader(int sockfd)
{
    int ret = ::read(sockfd, header, sizeof(message_header_t));
    if (ret == -1) {
        std::cerr << "Problem reading header" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Message::readBody(int sockfd)
{
    // TODO : Overload it depending on header->type
    UNUSED(sockfd);
    std::cout << "Message header type : " << header->type << std::endl;
}
