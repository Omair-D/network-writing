// sender.cpp
// Sender program for a datagram socket using UNIX domain in C++ 

#include <iostream>     // for std::cout and std::cerr
#include <cstring>      // for std::strlen and std::strcpy
#include <sys/types.h>  // basic socket types
#include <sys/socket.h> // main socket functions
#include <sys/un.h>     // UNIX domain socket structures
#include <unistd.h>     // for close()
#include <cstdlib> // rand()
#include <ctime> // seeding rand()


int main() {
    // Define the path for the receiver socket file
    const char *SOCKET_PATH = "./receiver_soc";
    char message[1024]; // Matching buffer size


    int soc;                             // Socket file descriptor
    sockaddr_un peer;                    // Struct for the address of the receiving socket

    // Set up the address struct for the receiver (peer)
    peer.sun_family = AF_UNIX;               // Set address family to UNIX domain
    std::strcpy(peer.sun_path, SOCKET_PATH); // Copy socket path to peer struct

    // Create a datagram (SOCK_DGRAM) socket in the UNIX domain (AF_UNIX)
    soc = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (soc < 0) { // Check for socket creation error
        std::cerr << "Socket creation failed\n";
        return 1;  // Exit with error code
    }

    std::srand(std::time(0)); // Seed the random number generator


    int total_messages = 10; // Number of messages to send
    for (int i = 1; i <= total_messages; ++i) {
        // Generate a random number and format the message with a sequence number
        int random_number = std::rand() % 1000; // Random number between 0-999
        std::sprintf(message, "Message %d: %d", i, random_number);

        // Send the message
        int n = sendto(soc, message, std::strlen(message), 0, (sockaddr *)&peer, sizeof(peer));
        if (n < 0) {
            std::cerr << "sendto failed\n";
            close(soc);
            return 1;
        }

        std::cout << "Sent: " << message << "\n";
    }

    // Send a special "END" message to signal the receiver
    std::strcpy(message, "END");
    sendto(soc, message, std::strlen(message), 0, (sockaddr *)&peer, sizeof(peer));

    std::cout << "All messages sent.\n";
    close(soc);
    return 0;
}
