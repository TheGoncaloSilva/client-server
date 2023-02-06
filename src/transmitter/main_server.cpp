// header of standard or provided libraries
#include <iostream>
#include <string>

// headers of local libraries
#include "server.h"

// Namespaces in use
using namespace std;

// Macros in use
constexpr int32_t PORT = 8000;
constexpr const char* LOGFOLDER = "./logs/server/";

//int main(int argc, char* argtv[])
int main()
{   
    // Get the name of the new log file
    time_t timeStamp = std::time(nullptr);
    string logFileName = "server_" + to_string(timeStamp) + ".log";

    if(!logger::init_logging(LOGFOLDER, logFileName))
        return 1;

    Server server{"127.0.0.1", PORT};
    server.create_server();
    return 0;
}
