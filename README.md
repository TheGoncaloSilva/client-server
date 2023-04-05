# client-server

This project represents a Asynchronous protocol-based TCP client and server using C++ and [Boost::Asio](https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio.html). This App is used for seed Distribution, where the Client request the server for seeds and the server responds to the client.

## Cloning the repo

In a directory of your choice, clone the project to your computer

```shell
cd «directory-of-your-choice»
git clone git@github.com:TheGoncaloSilva/client-server.git
```

## Pre-requisites

On Ubuntu you need the following packages installed:
_build-essential_, _cmake_, _doxygen_, and _git_.

```shell
sudo apt install build-essential cmake doxygen git libboost-all-dev libgtest-dev
```

In other Linux distributions you need equivalent packages installed.

**Attention:**  This code was developped using Boost version 1.74 and tests with google test version 1.11.0

## File structure

The project file structure is organized in the following way

* `bin:` Executable files (.out)
* `bin/logs:` Log files of server and client
* `build:` Build associated files
* `docs:` Project documentation
* `include:` Header files
* `tests:` Project test files
* `lib:` Used libraries (built using `ldd`)
* `src:` Source code files

## Compiling

### Preparing the compilation environment

From the command line navigate to the directory of the source code tree and create a build directory:

```shell
mkdir build
```

Next, navigate to that build directory and run [`cmake`](https://cmake.org/cmake/help/latest/manual/cmake.1.html#manual:cmake(1)) to configure the project and generate a native build system

```shell
cd build
cmake ..
```

### Compiling the code

Execute the call for the build system to actually compile/link the project:

```shell
cmake --build .
```

The resulting executables will be located in the projects `bin` folder.

## Generating documentation

To-do

## Testing the code

The tests were constructed using the [Ctest](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Testing%20With%20CMake%20and%20CTest.html) and [Google Test](https://google.github.io/googletest/) tools. To run them, you first need to compile the project, using the steps above. After that, from the project directory, execute the following commands:

```shell
cd bin/tests
./<test_to_run>
```

## Possible improvements

* Challeng-based authentication
* Try SonarCloud tool
* Use tools such as Conan to easily manage third-party packages

# Bibliography

Tutorial on client server implementation [here](https://www.bogotobogo.com/cplusplus/sockets_server_client.php)

Tutorial on Server application confinement for security [here](https://sweet.ua.pt/jpbarraca/course/sio-2223/lab-linux/)

Boost installation guide [here](https://www.boost.org/doc/libs/1_66_0/more/getting_started/unix-variants.html)

Open Source code example [here](https://github.com/iamazeem/TcpClientServerApp)

Logging guide available [here](https://www.sentinelone.com/blog/getting-started-quickly-cplusplus-logging/)

Asynchronous timer [here](https://www.bogotobogo.com/cplusplus/Boost/boost_AsynchIO_asio_tcpip_socket_server_client_timer_A.php) and [here](https://www.boost.org/doc/libs/1_81_0/doc/html/boost_asio/tutorial/tuttimer3.html)

Google Test guide [here](https://matgomes.com/integrate-google-test-into-cmake/)

CTest with CMake documentation [here](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Testing%20With%20CMake%20and%20CTest.html)
