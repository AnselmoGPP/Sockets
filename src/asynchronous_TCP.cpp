/*
 	Server() writes a message in the server's port:
	Create a tcp_server object initialized with io_context.
		1. This object initializes io_context to a certain server's port.
		2. Call tcp_connection::create (you get a shared_ptr to a tcp_connection object)
		3. acceptor.async_accept (initialized with a reference to the socket from the tcp_connection object [tcp_connection->socket()], and a handle_accept)
		4. handle_accept (if there is no error, call tcp_connection::start() [async_write: write the message]).
		5. Back to step 2.
		
	Client_sync() reads from the server's port.
	
	[read_some()], async_write()
*/

#include <ctime>
#include <string>
#include <iostream>
//#include <memory>
#include <boost/bind.hpp>
//#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

#define C_BUF_SIZE 13
#define S_IP "127.0.0.1"
#define S_PORT 45123

int client_sync() {
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

			std::cout << len << " bytes: " << recv_buff << std::endl;
		}
	}
	catch (std::exception& e) { std::cerr << e.what() << std::endl; }

	return 0;
}

class tcp_connection : public std::enable_shared_from_this<tcp_connection> {

	tcp_connection(boost::asio::io_context& io_context) : socket_(io_context) { }

	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/) { }

	tcp::socket socket_;
	std::string message = "Hello, client";

public:
	static std::shared_ptr<tcp_connection> create(boost::asio::io_context& io_context) {
		return std::shared_ptr<tcp_connection>(new tcp_connection(io_context));
	}

	tcp::socket& socket() { 
		return socket_; 
	}

	void start() {
		//char message[] = "Hello, client";
		boost::asio::async_write(	socket_, boost::asio::buffer(message, std::size(message)),
									boost::bind(	&tcp_connection::handle_write, shared_from_this(),
													boost::asio::placeholders::error,
													boost::asio::placeholders::bytes_transferred));
	}
};

class tcp_server {
public:
	tcp_server(boost::asio::io_context& io_context) : acceptor(io_context, tcp::endpoint(tcp::v4(), S_PORT)) {
		start_accept();
	}

private:
	void start_accept() {
		std::shared_ptr<tcp_connection> new_connection = tcp_connection::create(acceptor.get_executor().context());

		acceptor.async_accept(	new_connection->socket(),
								boost::bind(&tcp_server::handle_accept, this, new_connection, boost::asio::placeholders::error));
	}

	void handle_accept(std::shared_ptr<tcp_connection> new_connection, const boost::system::error_code& error) {
		if (!error) new_connection->start();

		start_accept();
	}

	tcp::acceptor acceptor;
};

void server() {
	try {
		boost::asio::io_context io_context;
		tcp_server server(io_context);
		io_context.run();
	}
	catch (std::exception& e) { std::cerr << e.what() << std::endl; }
}

int main() {
	std::thread serverThread(server);
	client_sync();

	serverThread.detach();
	return 0;
}
