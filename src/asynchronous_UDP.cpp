/*
	Server waits for a message. 
	When a message arrives (async_receive_from), it stores it in a buffer and runs handle_receive(). 
	handle_receive() sends a message (async_send_to), runs handle_send() and server waits for a message again.
*/

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

#define C_BUF_SIZE 20
#define S_BUF_SIZE 20
#define S_IP "127.0.0.1"
#define S_PORT 45123

void send_sync() {
	try {
		// ----- SEND -----
		boost::asio::io_context io_context;
		udp::resolver resolver(io_context);
		udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), S_IP, std::to_string(S_PORT)).begin();
		udp::socket socket(io_context);
		socket.open(udp::v4());
		char message[] = { "- Hi there, server" };
        socket.send_to(boost::asio::buffer(message, 18), receiver_endpoint);

		// ----- RECEIVE -----
		char recv_buf[C_BUF_SIZE];
		udp::endpoint sender_endpoint;
		size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
		std::cout << recv_buf << std::endl;
	}
	catch (std::exception& e) { std::cerr << e.what() << std::endl; }
}

class udp_server {
public:
	udp_server(boost::asio::io_context& io_context) : socket(io_context, udp::endpoint(udp::v4(), S_PORT))	{
		start_receive();
	}

private:
	void start_receive() {
		socket.async_receive_from(	boost::asio::buffer(recv_buffer, S_BUF_SIZE), remote_endpoint_,
									boost::bind(	&udp_server::handle_receive, this,
													boost::asio::placeholders::error,
													boost::asio::placeholders::bytes_transferred));
	}

	void handle_receive(const boost::system::error_code& error,	std::size_t /*bytes_transferred*/) {
		if (!error) {
			std::cout << recv_buffer << std::endl;
			boost::shared_ptr<std::string> message(new std::string("- Hi there, client"));		// boost::asio::buffer(*message)
			//std::shared_ptr<char[]> ptr(new char[20]);
			//boost::shared_ptr<char[]> message1(new char[]{"- Hi there, client"});
			//std::shared_ptr<char> ptr(new char[20], std::default_delete<char[]>());
			//auto shared_string = std::make_shared<std::string>("- Hi there, client");
			//boost::shared_ptr<std::string> ptr = boost::make_shared<std::string>(string_to_send);
			//auto message = std::make_shared<char>(message2);

			socket.async_send_to(	boost::asio::buffer(*message, 20), remote_endpoint_,
									boost::bind(	&udp_server::handle_send, this, message,
													boost::asio::placeholders::error,
													boost::asio::placeholders::bytes_transferred));
			start_receive();
		}
	}

	void handle_send(boost::shared_ptr<std::string> /*message*/, const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/) { }

	udp::socket socket;
	udp::endpoint remote_endpoint_;
	char recv_buffer[S_BUF_SIZE];
};

int main()
{

	//std::thread server_thread(server);
	std::thread client_thread(send_sync);

	try	{
		boost::asio::io_context io_context;
		udp_server server(io_context);
		io_context.run();
	}
	catch (std::exception& e) { std::cerr << e.what() << std::endl; };

	return 0;
}
