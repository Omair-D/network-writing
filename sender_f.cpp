// sender_f.cpp
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main() {
    const char *SOCKET_PATH = "./receiver_soc";     // Path for receiver
    const char *SENDER_SOCKET_PATH = "./sender_soc"; // Path for sender
    char message[1024]; // Buffer for sending messages
    char reply[1024];   // Buffer for receiving replies
    int soc;
    sockaddr_un peer, self;

    // Set up the sender's own address
    self.sun_family = AF_UNIX;
    std::strcpy(self.sun_path, SENDER_SOCKET_PATH);

    // Remove any existing socket file for the sender
    unlink(SENDER_SOCKET_PATH);

    // Create the socket
    soc = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (soc < 0) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // Bind the sender to its local address
    if (bind(soc, (sockaddr *)&self, sizeof(self)) < 0) {
        std::cerr << "Bind failed for sender\n";
        close(soc);
        return 1;
    }

    // Set up the receiver's address
    peer.sun_family = AF_UNIX;
    std::strcpy(peer.sun_path, SOCKET_PATH);

    std::cout << "Start chatting! Type your message below (type 'exit' to quit):\n";

    while (true) {
        // Get user input
        std::cout << "You: ";
        std::cin.getline(message, sizeof(message));

        // Exit if user types "exit"
        if (std::strcmp(message, "exit") == 0) {
            break;
        }

        // Send the message
        int sent = sendto(soc, message, std::strlen(message), 0, (sockaddr *)&peer, sizeof(peer));
        if (sent < 0) {
            std::cerr << "sendto failed\n";
            break;
        }

        // Receive a reply
        int received = recvfrom(soc, reply, sizeof(reply) - 1, 0, nullptr, nullptr);
        if (received < 0) {
            std::cerr << "recvfrom failed\n";
            break;
        }

        reply[received] = '\0'; // Null-terminate the received reply
        std::cout << "Friend: " << reply << "\n";
    }

    close(soc);
    unlink(SENDER_SOCKET_PATH); // Clean up the sender's socket file
    return 0;
}

