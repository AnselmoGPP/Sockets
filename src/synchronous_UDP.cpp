/*
	Two threads are created in main(). One runs the client socket and another the server socket.
			- The client sends a message ("- Hello, server") to the server. Then, waits for an answer from him and prints the received answer. Then, the thread finishes.
			- The server socket waits for a message from the client. When he receives it, he sends another message in response ("- Hello, client"). Then, he waits again for a message.
*/

#include <ctime>
#include <string>
#include <iostream>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::udp;

#define C_BUF_SIZE 20
#define S_BUF_SIZE 20
#define S_IP "127.0.0.1"
#define S_PORT 45123

void client() {
	try	{
		// ----- SEND -----
		boost::asio::io_context io_context;
		udp::resolver resolver(io_context);
		udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), S_IP, std::to_string(S_PORT)).begin();
		udp::socket socket(io_context);
		socket.open(udp::v4());
		char message[] = "- Hello, server" ;
        socket.send_to(boost::asio::buffer(message, 15), receiver_endpoint);

		// ----- RECEIVE -----
		char recv_buf[C_BUF_SIZE];
		udp::endpoint sender_endpoint;
		size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
		std::cout << recv_buf << std::endl;
	}
	catch (std::exception& e) {	std::cerr << e.what() << std::endl; }
}

void server() {
	try {
		// ----- RECEIVE ----
		boost::asio::io_context io_context;
		udp::socket socket(io_context, udp::endpoint(udp::v4(), S_PORT));

		for (;;) {
			char recv_buf[S_BUF_SIZE];
			udp::endpoint remote_endpoint;
			boost::system::error_code error;
            socket.receive_from(boost::asio::buffer(recv_buf, S_BUF_SIZE), remote_endpoint);
			std::cout << recv_buf << std::endl;
		// ----- SEND -----
			std::string message = "- Hello, client";
			boost::system::error_code ignored_error;
			socket.send_to(boost::asio::buffer(message), remote_endpoint, 0, ignored_error);
		}
	}
	catch (std::exception& e) {	std::cerr << e.what() << std::endl;	}
}

int main() {

	std::thread server_thread(server);
	std::thread client_thread(client);

	server_thread.detach();
	client_thread.join();

	return 0;
}
