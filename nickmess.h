#ifndef NICKMESS_H
#define NICKMESS_H

#include "message.h"

namespace clt {
    class NickMessage : public MessageT<1> {
        public:
            NickMessage(std::string name) {
                header = new message_header_t;
                body = std::vector<std::string>(1, name);
                header->type = MSG_TYPE_CHANGE_NICK;
                header_update();
            }
    };
}

#endif
