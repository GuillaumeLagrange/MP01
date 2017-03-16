#ifndef NICKMESS_H
#define NICKMESS_H

#include "message.h"

using namespace clt;

class NickMessage : public Message {
    public:
        NickMessage(const std::string nick) : Message() {
            body = nick;
            initHeader();
        }
        void initHeader() {
            header->length = sizeof(message_header_t) + body.length() + 1;
            header->type = MSG_TYPE_CHANGE_NICK;
            header->body_checksum =
                    fletcher16(reinterpret_cast<const uint8_t *>(body.c_str()),
                                body.length() + 1);
            header->head_checksum = 0;
            header->head_checksum = fletcher16(reinterpret_cast<uint8_t *>(header),
                                sizeof(*header));
        }
};

#endif
