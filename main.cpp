#include <sys/types.h>
#include "client.h"
#include "nickmess.h"

using namespace clt;

int main()
{
    std::cout << "Hello" << std::endl;

    Client client = Client("localhost", 12345); // My machine
    client.connect();
    client.change_nickname("guigui");

    return 0;
}
