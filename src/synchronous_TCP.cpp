/*
	The server writes a message in one of its ports.
	The client access to that server's port and reads the message.
	
	read_some(), write()
*/

#include <ctime>
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

#define C_BUF_SIZE 20
#define S_IP "127.0.0.1"
#define S_PORT 45123

int client() {
	try {
		boost::asio::io_context io_context;
		tcp::resolver resolver(io_context);
		tcp::resolver::results_type endpoints = resolver.resolve(S_IP, std::to_string(S_PORT));
		tcp::socket socket(io_context);
		boost::asio::connect(socket, endpoints);

		for (;;) {
			// ----- RECEIVE -----
			char recv_buff[C_BUF_SIZE];
			boost::system::error_code error;
			size_t len = socket.read_some(boost::asio::buffer(recv_buff, C_BUF_SIZE), error);

			if (error == boost::asio::error::eof) break;					// Connection closed cleanly by peer.
			else if (error) throw boost::system::system_error(error);		// Some other error.

			std::cout << recv_buff << std::endl;
		}
	}
	catch (std::exception& e) { std::cerr << e.what() << std::endl; }

	return 0;
}

int server() {
	try {
		boost::asio::io_context io_context;
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), S_PORT));

		for (;;) {
			tcp::socket socket(io_context);
			acceptor.accept(socket);

			// ----- SEND -----
			char message[] = "- Hello, client";
			boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message, 15), ignored_error);
		}
	}
	catch (std::exception& e) {	std::cerr << e.what() << std::endl; }

	return 0;
}

int main() {

	std::thread server_thread(server);
	std::thread client_thread(client);

	server_thread.detach();
	client_thread.join();

	return 0;
}
