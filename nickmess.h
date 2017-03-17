#ifndef NICKMESS_H
#define NICKMESS_H

#include "message.h"

using namespace clt;

class NickMessage : public MessageT<1> {
    public:
        NickMessage(std::string name) : MessageT<1>() {
            body[0] = name;
            header_update();
        }
};

#endif
