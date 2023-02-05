// header of standard or provided libraries
#include <iostream>
#include <string>

// headers of local libraries
#include "server.h"

// Namespaces in use
using namespace std;

// Macros in use
constexpr int32_t PORT = 8000;

int main(int argc, char* argv[])
{
    Server server{"127.0.0.1", PORT};
    server.create_server();
    return 0;
}
