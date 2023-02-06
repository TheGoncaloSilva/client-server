// header of standard or provided libraries
#include <iostream>
#include <string>
#include <ctime>

// headers of local libraries
#include "client.h"

// Namespaces in use
using namespace std;

// Macros declarations
constexpr uint16_t PORT = 8000; 
constexpr const char* LOGFOLDER = "./logs/client/";


//int main(int argc, char* argv[]) 
int main() 
{   
    // Get the name of the new log file
    time_t timeStamp = std::time(nullptr);
    string logFileName = "client_" + to_string(timeStamp) + ".log";

    if(!logger::init_logging(LOGFOLDER, logFileName))
        return 1;

    Client client{"127.0.0.1", PORT};
    client.client_life();
    return 0;
}