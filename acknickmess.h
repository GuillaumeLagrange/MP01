#ifndef ACKNICKMESS_H
#define ACKNICKMESS_H

#include "message.h"

namespace clt {
    class AckNickMessage : public MessageT<2> {
        public:
            AckNickMessage(message_header_t * hdr) : MessageT<2>(hdr) {
                if (hdr->type != MSG_TYPE_ACKNOWLEDGE_CHANGE_NICK) {
                    std::cout << "Wrong type in ack nick header" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            void handleMessage()
            {
                if (body[1] == "true")
                    std::cout << "Nickname changed to " << body[0] << std::endl;
                else if (body[1] == "false")
                    std::cout << "Nickname change failed" << std::endl;
                else
                    printf("Error in ack nick message reception\n");
            }
    };
}

#endif
