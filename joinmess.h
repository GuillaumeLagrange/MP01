#ifndef JOINMESS_H
#define JOINMESS_H

#include "message.h"

namespace clt {
    class JoinMessage : public MessageT<1> {
        public:
            JoinMessage(std::string channel) {
                header = new message_header_t;
                body = std::vector<std::string>(1, channel);
                header->type = MSG_TYPE_JOIN;
                header_update();
            }
    };
}

#endif
