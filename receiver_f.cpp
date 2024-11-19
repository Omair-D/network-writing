// receiver_f.cpp
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>

void cleanup(int soc, const char *path) {
    close(soc);
    unlink(path);
}

int main() {
    const char *SOCKET_PATH = "./receiver_soc"; // Path for receiver
    char message[1024]; // Buffer for receiving messages
    char reply[1024];   // Buffer for sending replies
    int soc;
    sockaddr_un self, peer;
    socklen_t peer_len = sizeof(peer);

    // Remove any existing socket file for the receiver
    unlink(SOCKET_PATH);

    self.sun_family = AF_UNIX;
    std::strcpy(self.sun_path, SOCKET_PATH);

    soc = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (soc < 0) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // Bind the receiver to its local address
    if (bind(soc, (sockaddr *)&self, sizeof(self)) < 0) {
        std::cerr << "Bind failed\n";
        cleanup(soc, SOCKET_PATH);
        return 1;
    }

    std::cout << "Waiting for messages... (type 'exit' to quit):\n";

    while (true) {
        // Receive a message
        int received = recvfrom(soc, message, sizeof(message) - 1, 0, (sockaddr *)&peer, &peer_len);
        if (received < 0) {
            std::cerr << "recvfrom failed\n";
            cleanup(soc, SOCKET_PATH);
            return 1;
        }

        message[received] = '\0'; // Null-terminate the received message
        std::cout << "Friend: " << message << "\n";

        // Exit if the sender types "exit"
        if (std::strcmp(message, "exit") == 0) {
            break;
        }

        // Get user input for the reply
        std::cout << "You: ";
        std::cin.getline(reply, sizeof(reply));

        // Send the reply
        int sent = sendto(soc, reply, std::strlen(reply), 0, (sockaddr *)&peer, peer_len);
        if (sent < 0) {
            std::cerr << "sendto failed\n";
            cleanup(soc, SOCKET_PATH);
            return 1;
        }
    }

    cleanup(soc, SOCKET_PATH); // Clean up the receiver's socket file
    return 0;
}

