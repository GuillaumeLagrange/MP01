#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <unistd.h>
#include "protocol.hxx"

#define UNUSED(x) (void)(x)

namespace clt {
    class Message {
        public:
            virtual message_header_t * getHeader() = 0;
            static message_header_t * readHeader(int sockfd);
            virtual void initHeader();
            virtual void readBody(int sockfd) = 0;
    };

    template<int N>
    class MessageT : public Message {
        protected:
            message_header_t * header;
            std::string body[N];

        public:
            MessageT();
            MessageT(message_header_t * _header);
            virtual ~MessageT();
            message_header_t * getHeader() { return header; }
            std::string * getBody() { return body; }
            static message_header_t * readHeader(int sockfd);
            void readBody(int sockfd);
            void header_update();
    };
};

using namespace clt;

template<int N>
MessageT<N>::MessageT() : header(new message_header_t),
    body(new std::string[N]) { }

template<int N>
MessageT<N>::MessageT(message_header_t * _header) : header(_header),
    body(new std::string[N]) { }

template<int N>
MessageT<N>::~MessageT()
{
    delete header;
    delete body;
}

template<int N>
message_header_t * MessageT<N>::readHeader(int sockfd)
{
    message_header_t * hdr = new message_header_t();
    int ret = ::read(sockfd, hdr, sizeof(message_header_t));
    if (ret == -1) {
        std::cerr << "Problem reading header" << std::endl;
        exit(EXIT_FAILURE);
    }
    return hdr;
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

/* Updates length and body checksum, then header checksum */
template<int N>
void MessageT<N>::header_update()
{
    int len = 0;
    for (int i=0; i<N; i++) {
        len += body[i].length() + 1;
    }
    header->length = len;

    char * buffer = (char *) malloc(len);
    char * current_buffer = buffer;
    for (int i=0; i<N; i++) {
        buffer = strcpy(current_buffer, body[i].c_str());
        current_buffer += body[i].length() + 1;
    }
    header->body_checksum = fletcher16((const uint8_t *) buffer, len);

    header->head_checksum = 0;
    header->head_checksum = fletcher16((const uint8_t *) header,
            sizeof(message_header_t));
}

#endif
