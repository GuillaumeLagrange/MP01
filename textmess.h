#ifndef TEXTMESSAGE_H
#define TEXTMESSAGE_H

#include "message.h"

namespace clt {
    class TextMessage : public MessageT<3> {
        public:
            TextMessage(std::string from, std::string to,
                    std::string mess) : MessageT<3>() {
                body.push_back(from);
                body.push_back(to);
                body.push_back(mess);
                header->type = MSG_TYPE_MESSAGE;
                header_update();
            }

            TextMessage(message_header_t * hdr) : MessageT<3>(hdr) {
                if (hdr->type != MSG_TYPE_MESSAGE) {
                    std::cout << "Wrong message type instanciation" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }

            void handleMessage()
            {
                std::cout << body[0] << "@" << body [1] << " "
                    << body[2] << std::endl;
            }
    };
}

#endif
