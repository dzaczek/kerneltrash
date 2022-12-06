#include <linux/module.h>
#include <net/sock.h>

// Initialization function for the kernel module
static int __init hello_init(void)
{
    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // Bind the socket to port 8080
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));

    // Start listening for incoming connections
    listen(sock, 5);

    // Accept incoming connections
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sock = accept(sock, (struct sockaddr*)&client_addr, &client_addr_len);

    // Main loop
    while (true)
    {
        // Send the "Hello World" message to the client
        send(client_sock, "Hello World", 11, 0);
    }

    return 0;
}

// Cleanup function for the kernel module
static void __exit hello_cleanup(void)
{
    // Perform any necessary cleanup here
}

// Define the init and cleanup functions
module_init(hello_init);
module_exit(hello_cleanup);
