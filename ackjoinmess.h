#include "message.h"

namespace clt {
    class AckJoinMessage : public MessageT<2> {
        public:
            AckJoinMessage(message_header_t * hdr) : MessageT<2>(hdr) {
                if (hdr->type != MSG_TYPE_ACKNOWLEDGE_JOIN) {
                    std::cout << "Wrong type in ack join header" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            void handleMessage()
            {
                std::cout << "Coucou" << std::endl;
                if (body[0] == "true")
                    printf("Joined channel %s\n", body[1].c_str());
                else
                    printf("Could not join channel %s\n", body[1].c_str());
            }
    };
}
