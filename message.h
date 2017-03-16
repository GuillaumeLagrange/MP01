#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include "protocol.hxx"

#define UNUSED(x) (void)(x)

namespace clt {
    class Message {
        protected:
            message_header_t * header;
            std::string body;

        public:
            Message();
            ~Message();
            message_header_t * getHeader() { return header; }
            std::string getBody() { return body; }
            void readHeader(int sockfd);
            virtual void readBody(int sockfd);
            virtual void initHeader() { };
            //virtual void * create_buffer();
    };
};

#endif
