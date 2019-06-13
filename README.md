# Sockets
Compilation of sockets TCP and UDP

- synchronous_UDP.cpp

Two threads are created in main(). One runs the client socket and another the server socket.
  
>The client sends a message ("- Hello, server") to the server. Then, waits for an answer from him and prints the received answer. Then, the thread finishes.
>The server socket waits for a message from the client. When he receives it, he sends another message in response ("- Hello, client"). Then, he waits again for a message.
  
- asynchronous_UDP.cpp

Server waits for a message. 
When a message arrives (async_receive_from), it stores it in a buffer and runs handle_receive(). 
handle_receive() sends a message (async_send_to), runs handle_send() and server waits for a message again.

- synchronous_TCP.cpp

The server writes a message in one of its ports.
The client access to that server's port and reads the message.

- synchronous_TCP_2.cpp

Server waits for a connection -> Client connects and sends a message -> Server receives the message, responds with another message, and continue waiting.
The receive() is inside a while-loop for allowing the reception of long messages, as long as they end with '\0'. 

- asynchronous_TCP.cpp

Server() writes a message in the server's port:
>	  Create a tcp_server object initialized with io_context.
>		    1. This object initializes io_context to a certain server's port.
>		    2. Call tcp_connection::create (you get a shared_ptr to a tcp_connection object)
>		    3. acceptor.async_accept (initialized with a reference to the socket from the tcp_connection object [tcp_connection->socket()], and a handle_accept)
>	        4. handle_accept (if there is no error, call tcp_connection::start() [async_write: write the message]).
>		    5. Back to step 2.

Client_sync() reads from the server's port.

- asynchronous message (folder) (UDP)

A client class sends a message (a server socket is required to be binded to the destination port). A server class receives the message (server takes the message from the port when he wants).

Client sends a message (to a server's port). Before sending the message, it seems that the server must be conected to that socket. That's why we create both the client and the server objects before sending the message (when server is created after the sending, he receives no message). Whenever we want, we can make the server go to see if he received a message in the specified port. 

>		receive_from(): If a message arrived, he takes it. If not, he waits until a messages arrives.
>		async_receive_from(): If a message arrived, he takes it. If not, he doesn't wait for it.
 
  ----- Notes ---------------------------------------------------
  
Before sending (synchronously) a message, there must exist a socket listening. If it's not, an exception may occur ("It has been forced the interruption of an existing conexion with the remote host").

The buffer of the system can be modified with	getsockopt() and setsockopt()
  
If the received message contains undefined values at the end when printed (std::cout), it may be because the recovering buffer has not been initialized, so it is not properly null-terminated.

In TCP, you have to pass buffer as shared_ptr so your object won't be destroyed after leaving the function.

If you are receiving messages with .receive() regularly, if you don't take them, Boost will append those messages in your buffer.

Links:

- https://sciencesoftcode.wordpress.com/2018/12/11/boost-asio-network-and-low-level-i-o-programming/
- https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio.html
