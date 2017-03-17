#include <sys/types.h>
#include "client.h"
#include "nickmess.h"

using namespace clt;

int main()
{
    Client client = Client("localhost", 12345); // My machine
    client.connect();
    std::cout << "Connected" << std::endl << std::endl;
    client.change_nickname("guillaume");

    return 0;
}
