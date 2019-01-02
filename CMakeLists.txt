cmake_minimum_required (VERSION 3.13)
project (email-client)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.69.0 REQUIRED
    COMPONENTS
        system
        regex
        iostreams
        thread
)

include_directories( ./include ./src ./src/imap-client ${Boost_INCLUDE_DIRS} )
add_executable(email-client
    ./src/main.cpp
    ./src/MailAccount.cpp
    ./src/imap-client/Protocol.cpp
    ./src/imap-client/Connection.cpp
)

target_link_libraries(email-client Boost::system Boost::regex Boost::iostreams Boost::thread crypto ssl)
