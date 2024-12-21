# Texting App with UNIX Domain Sockets

## Description

This project is a simple **texting application** built using **C++** and **UNIX domain sockets**. The app allows two users, operating on different terminals under the same account, to exchange messages in real-time. It demonstrates basic socket programming concepts and ier-process communication (IPC).

---

## Features

- **Bidirectional Communication:** Users can send and receive messages in real-time.
- **Exit Mechanism:** Typing `exit` terminates the conversation for both parties.
- **Simple and User-Friendly:** A command-line interface for seamless chatting.
- **Socket-Based Communication:** Utilizes `AF_UNIX` sockets for efficient local communication.

---

## How It Works

This application consists of two programs:

1. **Sender (********`sender_f.cpp`********):**

   - Sends messages typed by the user to the receiver.
   - Displays replies received from the receiver.

2. **Receiver (********`receiver_f.cpp`********):**

   - Displays messages received from the sender.
   - Allows the user to reply to the sender.

Both programs run in separate terminals and communicate using UNIX domain sockets.

---

## Prerequisites

1. **Operating System:** Linux or macOS (with UNIX domain socket support).
2. **Compiler:** GCC or any C++ compiler.

---

## Usage

### Running the Application

1. **Open Two Terminals:**

   - In the first terminal, start the **receiver**:
     ```bash
     ./receiver
     ```
   - In the second terminal, start the **sender**:
     ```bash
     ./sender
     ```

2. **Start Chatting:**

   - Type your message in the sender terminal and press Enter.
   - The receiver will see the message and can reply.
   - Both users can continue chatting until one types `exit`.

---

## Example Conversation

### Terminal 1 (Receiver):

```
Waiting for messages... (type 'exit' to quit):
Friend: Hello!
You: Hi there!
Friend: How are you?
You: I'm good, thanks! What about you?
Friend: exit
```

### Terminal 2 (Sender):

```
Start chatting! Type your message below (type 'exit' to quit):
You: Hello!
Friend: Hi there!
You: How are you?
Friend: I'm good, thanks! What about you?
You: exit
```

---

## Code Overview

### Sender (`sender_f.cpp`)

- Binds to a local socket file (`sender_soc`).
- Sends messages to the receiver's socket (`receiver_soc`).
- Waits for and displays replies from the receiver.

### Receiver (`receiver_f.cpp`)

- Binds to a local socket file (`receiver_soc`).
- Displays messages received from the sender.
- Allows the user to send replies back to the sender's socket (`sender_soc`).

---



