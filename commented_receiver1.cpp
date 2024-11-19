// receiver.cpp
// Receiver program for a datagram socket using UNIX domain in C++
#include <iostream> // for std::cout and std::cerr
#include <cstring> // for std::strcpy
#include <sys/types.h> // basic socket types
#include <sys/socket.h> // main socket functions
#include <unistd.h> // for close() and unlink()
#include <sys/un.h> // UNIX domain socket structures
#include <sys/time.h> // For select() and timeval

// Cleanup function to close the socket and unlink (delete) the socket file
void cleanup(int soc, const char *path) {
   close(soc); // Close the socket file descriptor
   unlink(path); // Remove the socket file from the file system
}
int main() {
   // Define the path for the receiver socket file
   const char *SOCKET_PATH = "./receiver_soc";
   int soc; // Socket file descriptor
   char buf[1024]; // Buffer to hold the received message
   sockaddr_un self, peer; // Structs for socket addresses (self = receiver, peer =:: sender)
   socklen_t peer_len = sizeof(peer); // Length of the peer struct for receiving the message
   // Set up the address struct for the receiver (self)
   self.sun_family = AF_UNIX; // Set address family to UNIX domain
   std::strcpy(self.sun_path, SOCKET_PATH); // Copy socket path to self struct
   // Create a datagram (SOCK_DGRAM) socket in the UNIX domain (AF_UNIX)
   soc = socket(AF_UNIX, SOCK_DGRAM, 0);
   if (soc < 0) { // Check for socket creation error
      std::cerr << "Socket creation failed\n";
      return 1; // Exit with error code
   }
   // Bind the socket to the specified path
   if (bind(soc, (sockaddr*)&self, sizeof(self)) < 0) {
      std::cerr << "Bind failed\n"; // Print error if binding fails
      cleanup(soc, SOCKET_PATH); // Clean up resources
      return 1;
   }

    std::cout << "Waiting for messages...\n";

    // Loop to receive multiple messages
    while (true) {
        int n = recvfrom(soc, buf, sizeof(buf) - 1, 0, (sockaddr *)&peer, &peer_len);
        if (n < 0) {
            std::cerr << "recvfrom failed\n";
            cleanup(soc, SOCKET_PATH);
            return 1;
        }

        buf[n] = '\0'; // Null-terminate the received message
        std::cout << "Received: " << buf << "\n";

        // Exit loop if the sender indicates completion
        if (strcmp(buf, "END") == 0) {
            std::cout << "All messages received. Exiting.\n";
            break;
        }

        sleep(10); // Delay for 10 seconds before sending the reply


        // Generate a random number and reply to the sender
        int random_number = std::rand() % 1000; // Random number between 0-999
        char reply[1024];
        std::sprintf(reply, "Reply: %d", random_number);

        int sent = sendto(soc, reply, std::strlen(reply), 0, (sockaddr *)&peer, peer_len);
        if (sent < 0) {
            std::cerr << "sendto failed\n";
            cleanup(soc, SOCKET_PATH);
            return 1;
        }

        std::cout << "Sent reply: " << reply << "\n";
    }

    cleanup(soc, SOCKET_PATH);
    return 0;
}

