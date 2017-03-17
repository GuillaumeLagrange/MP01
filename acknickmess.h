#ifndef ACKNICKMESS_H
#define ACKNICKMESS_H

#include "message.h"

namespace clt {
    class AckNickMessage : public MessageT<2> {
        public:
            AckNickMessage(message_header_t * hdr) : MessageT<2>(hdr) {
                if (hdr->type != 4) {
                    std::cout << "Wrong type in ack nick header" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            void handleMessage()
            {
                if (body[1] == "true")
                    std::cout << "Nickname changed to " << body[0] << std::endl;
                else
                    std::cout << "Nickname change failed" << std::endl;
            }
    };
}

#endif
