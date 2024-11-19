// sender.cpp
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()

int main() {
    const char *SOCKET_PATH = "./receiver_soc"; // Path for receiver
    const char *SENDER_SOCKET_PATH = "./sender_soc"; // Path for sender
    char message[1024]; // Buffer for messages
    char reply[1024];   // Buffer for replies
    int soc;
    sockaddr_un peer, self; // Corrected declaration

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

    std::srand(std::time(0)); // Seed the random number generator

    int total_messages = 10; // Number of messages to send
    for (int i = 1; i <= total_messages; ++i) {
        // Generate a random number and format the message with a sequence number
        int random_number = std::rand() % 1000; // Random number between 0-999
        std::sprintf(message, "Message %d: %d", i, random_number);

        // Send the message
        int sent = sendto(soc, message, std::strlen(message), 0, (sockaddr *)&peer, sizeof(peer));
        if (sent < 0) {
            std::cerr << "sendto failed\n";
            close(soc);
            unlink(SENDER_SOCKET_PATH);
            return 1;
        }

        std::cout << "Sent: " << message << "\n";

        // Receive a reply from the receiver
        int received = recvfrom(soc, reply, sizeof(reply) - 1, 0, nullptr, nullptr);
        if (received < 0) {
            std::cerr << "recvfrom failed\n";
            close(soc);
            unlink(SENDER_SOCKET_PATH);
            return 1;
        }

        reply[received] = '\0'; // Null-terminate the received reply
        std::cout << "Received reply: " << reply << "\n";
    }

    // Send a special "END" message to signal the receiver
    std::strcpy(message, "END");
    sendto(soc, message, std::strlen(message), 0, (sockaddr *)&peer, sizeof(peer));

    std::cout << "All messages sent.\n";
    close(soc);
    unlink(SENDER_SOCKET_PATH); // Clean up the sender's socket file
    return 0;
}
