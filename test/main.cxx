#include <iostream>
#include <bits/ostream.tcc>
#include <dfl/Types.h>

using namespace dfl;

#include <atomic>
#include <cassert>
#include <string>
#include <thread>
#include <vector>

#include "dfl/net/NamedPipeClientStream.h"
#include "dfl/net/NamedPipeServerStream.h"

void test_named_pipe() {
    std::cout << "--- Running Named Pipe Test ---" << std::endl;

    // Use a unique name for the pipe to avoid conflicts.
    // In a real test suite, this might be a UUID or based on process ID.
    const std::string pipe_name = "my_dfl_test_pipe";
    std::atomic<bool> server_ok = false;
    std::atomic<bool> client_ok = false;

    // Messages to exchange
    const std::string client_msg_str = "Hello from Client!";
    const std::string server_msg_str = "Hello from Server!";
    std::vector<u8> client_msg(client_msg_str.begin(), client_msg_str.end());
    std::vector<u8> server_msg(server_msg_str.begin(), server_msg_str.end());

    // --- Server Thread ---
    std::thread server_thread([&]() {
        try {
            std::cout << "[Server] Creating pipe: " << pipe_name << " and waiting for client..." << std::endl;
            dfl::NamedPipeServerStream server(pipe_name);
            std::cout << "[Server] Client connected." << std::endl;

            // 1. Read message from client
            std::vector<u8> buffer(client_msg.size());
            ssize_t bytes_read = server.read(buffer);
            std::cout << "[Server] Read " << bytes_read << " bytes." << std::endl;

            assert(static_cast<size_t>(bytes_read) == client_msg.size());
            assert(buffer == client_msg);
            std::cout << "[Server] Received correct message from client." << std::endl;

            // 2. Write response to client
            ssize_t bytes_written = server.write(server_msg);
            std::cout << "[Server] Wrote " << bytes_written << " bytes." << std::endl;
            assert(static_cast<size_t>(bytes_written) == server_msg.size());

            server_ok = true;
        } catch (const std::exception& e) {
            std::cerr << "[Server] Exception: " << e.what() << std::endl;
            server_ok = false;
        }
    });

    // --- Client Thread ---
    std::thread client_thread([&]() {
        try {
            // The client constructor will retry a few times, so we don't need
            // an explicit sleep here, but it's good practice to ensure the
            // server thread starts first.
            std::cout << "[Client] Attempting to connect to " << pipe_name << std::endl;
            dfl::NamedPipeClientStream client(pipe_name);
            std::cout << "[Client] Connected to server." << std::endl;

            // 1. Write message to server
            ssize_t bytes_written = client.write(client_msg);
            std::cout << "[Client] Wrote " << bytes_written << " bytes." << std::endl;
            assert(static_cast<size_t>(bytes_written) == client_msg.size());

            // 2. Read response from server
            std::vector<u8> buffer(server_msg.size());
            ssize_t bytes_read = client.read(buffer);
            std::cout << "[Client] Read " << bytes_read << " bytes." << std::endl;

            assert(static_cast<size_t>(bytes_read) == server_msg.size());
            assert(buffer == server_msg);
            std::cout << "[Client] Received correct message from server." << std::endl;

            client_ok = true;
        } catch (const std::exception& e) {
            std::cerr << "[Client] Exception: " << e.what() << std::endl;
            client_ok = false;
        }
    });

    // Wait for both threads to complete
    server_thread.join();
    client_thread.join();

    // Final assertion
    if (server_ok && client_ok) {
        std::cout << "--- Test Passed ---" << std::endl;
    } else {
        std::cout << "--- Test Failed ---" << std::endl;
        // In a real test framework, this would be a hard failure.
        assert(false && "Test failed. Check logs.");
    }
}

using TestEnum = Enum<i32, f32>;

void test_enum() {
    TestEnum te = f32(1.5f);
    match(
      te,
      [](const f32 f) {
        std::cout << "float " << f << std::endl;
      },
      [](const i32 &i) {
        std::cout << "int " << i << std::endl;
      }
    );
}

// Main function to run the test
int main() {
    test_enum();
    test_named_pipe();
    return 0;
}
