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
                if (body[0] == "true")
                    printf("Joined channel %s\n", body[1].c_str());
                else if (body[0] == "false")
                    printf("Could not join channel %s\n", body[1].c_str());
                else
                    printf("Error in ack join message reception\n");
            }
    };
}
