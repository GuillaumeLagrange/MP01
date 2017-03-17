#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include "protocol.hxx"

#define UNUSED(x) (void)(x)


namespace clt {
    class Message {
        public:
            virtual message_header_t * getHeader() = 0;
            //static message_header_t * readHeader(int sockfd);
            virtual void header_update() = 0;
            virtual void readBody(int sockfd) = 0;
    };

    template<int N>
    class MessageT : public Message {
        protected:
            message_header_t * header;
            std::vector<std::string> body;

        public:
            MessageT();
            MessageT(message_header_t * _header);
            virtual ~MessageT();
            message_header_t * getHeader() { return header; }
            std::vector<std::string>  getBody() { return body; }
            void readBody(int sockfd);
            void header_update();
            void printHeader();
    };
};

using namespace clt;

template<int N>
MessageT<N>::MessageT() : header(new message_header_t),
    body(std::vector<std::string>(N)) { }

template<int N>
MessageT<N>::MessageT(message_header_t * _header) : header(_header),
    body(std::vector<std::string>(N)) { }

template<int N>
MessageT<N>::~MessageT()
{
    delete header;
}

template<int N>
void MessageT<N>::readBody(int sockfd)
{
    int buffer_length = header->length - sizeof(message_header_t);
    char * buffer = (char *) malloc(buffer_length);
    int ret = ::read(sockfd, buffer, buffer_length);
    if (ret == -1) {
        std::cerr << "Problem reading body" << std::endl;
        exit(EXIT_FAILURE);
    }

    char * buff = buffer;

    for (int i = 0; i < N; i++) {
        body[i] = std::string(buff);
        buff += body[i].length() + 1;
    }

    free(buffer);
}

template<int N>
void MessageT<N>::printHeader()
{
    std::cout << "Printing header" << std::endl;
    std::cout << "length : " << header->length << std::endl;
    std::cout << "type : " << header->type << std::endl;
    std::cout << "body_checksum : " << header->body_checksum << std::endl;
    std::cout << "head_checksum : " << header->head_checksum << std::endl;
    std::cout << std::endl;
}

/* Updates length and body checksum, then header checksum */
template<int N>
void MessageT<N>::header_update()
{
    int len = 0;
    for (int i=0; i<N; i++) {
        len += body[i].length() + 1;
    }
    header->length = len + sizeof(message_header_t);

    char * buffer = (char *) malloc(len);
    char * current_buffer = buffer;
    for (int i=0; i<N; i++) {
        buffer = strcpy(current_buffer, body[i].c_str());
        current_buffer += body[i].length() + 1;
    }
    header->body_checksum = fletcher16((const uint8_t *) buffer, len);
    std::cout << "Header checksum in header_update" << header->body_checksum <<std::endl<<std::endl;
    free(buffer);

    header->head_checksum = 0;
    header->head_checksum = fletcher16((const uint8_t *) header,
            sizeof(message_header_t));
}

#endif
