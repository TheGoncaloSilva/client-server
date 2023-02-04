// header of standard or provided libraries
#include <iostream>
#include <string>

// headers of local libraries
#include "client.h"

// Namespaces in use
using namespace std;

// Macros declarations
constexpr uint16_t PORT = 8000; 

int main(int argc, char* argv[]) 
{
    Client client{"127.0.0.1", PORT};
    client.client_life();
    return 0;
}