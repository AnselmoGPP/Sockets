/*
	Server waits for a connection.
	Client connects and sends a message.
	Server receives the message, responds with another message, and continue waiting.

	The receive() is inside a while-loop for allowing the reception of long messages, as long as they
	end with '\0'. 
	Remember: If you call receive() too soon, before the message has arrived completely, 
	you will get only a part of the message. To get the whole message, you can wait (sleep_for) for 
	the message to arrive completely and then call receive() (not very elegant), or you can put
	the receive() inside a loop and call it many times until you get the whole message.

	send(), receive()
*/

#include <iostream>
#include <thread>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::address;

#define BUFFER_SIZE 20
#define S_IP "127.0.0.1"
#define S_PORT 36123

class client;
class server;
void server_thread();
void client_thread();

int main() {

	std::thread serv(server_thread);
	std::thread clie(client_thread);

	clie.join();
	serv.detach();
}


class client {
	boost::asio::io_context io_context;
	tcp::resolver::results_type endpoints;
	tcp::socket socket;
	boost::system::error_code error;
	size_t len;
	char buff[BUFFER_SIZE];

public:
	client() : socket(io_context) {
		tcp::resolver resolver(io_context);
		endpoints = resolver.resolve(S_IP, std::to_string(S_PORT));
		boost::asio::connect(socket, endpoints);								// Client socket connects to the server (where a server socket must be waiting for that conection).
	}
	~client() {
		socket.close();
	}

	void send() {
		try {
			socket.send(boost::asio::buffer("Hello, server"));

			size_t len = 0, accum_len = 0;

			do {
				len = socket.receive(boost::asio::buffer(&buff[accum_len], (BUFFER_SIZE - accum_len)), 0, error);	// https://stackoverflow.com/questions/10250666/socket-programming-issue-with-recv-receiving-partial-messages
				accum_len += len;
			} while (buff[accum_len - 1] != '\0');

			//size_t len = socket.receive(boost::asio::buffer(buff, BUFFER_SIZE));

			std::cout << buff << std::endl;
		}
		catch (std::exception& e) { std::cerr << e.what() << std::endl; }
	}
};

class server {
	boost::asio::io_context io_context;
	tcp::resolver::results_type endpoints;
	tcp::socket socket;
	boost::system::error_code ec;
	size_t len;
	char buff[BUFFER_SIZE];

public:
	server() : socket(io_context) {
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), S_PORT));
		acceptor.accept(socket);												// Server socket waits until another socket connects to him.
	}
	~server() {
		socket.close();
	}

	void listen() {

		try {

			for (;;) {
				size_t len = socket.receive(boost::asio::buffer(buff, BUFFER_SIZE));
				std::cout << buff << std::endl;

				socket.send(boost::asio::buffer("Hello, client"));
			}

		}
		catch (std::exception& e) { std::cerr << e.what() << std::endl; }

	}

};

void client_thread() {
	client clie;
	clie.send();
}

void server_thread() {
	server serv;
	serv.listen();
}
