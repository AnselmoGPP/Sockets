#pragma once

// Server and client boost socket: https://stackoverflow.com/questions/44273599/udp-communication-using-c-boost-asio
// Boost socket class: https://www.boost.org/doc/libs/1_41_0/doc/html/boost_asio/reference/ip__udp/socket.html
// Asynchronous: https://stackoverflow.com/questions/44273599/udp-communication-using-c-boost-asio
// Bit padding

/*
----- RECEIVE -----
	boost::asio::io_service io_service;
	udp::socket socket;
*/

#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
//#include <thread>

using boost::asio::ip::udp;
using boost::asio::ip::address;

#define IPADDRESS "127.0.0.1"		// Dirección IP de loopback (Local host): 127.0.0.1
#define UDP_PORT 38252

const int client_bytes = 20;		// Size in bytes of the buffer to store the received data (maximum size of the buffer)

class C_server {
public:
	C_server() : socket(io_service) {

		socket.open(udp::v4());
		socket.bind(udp::endpoint(address::from_string(IPADDRESS), UDP_PORT));
	}
	~C_server() {
		socket.close();
	}

	char buffer[client_bytes];

	boost::asio::io_service io_service;
	udp::socket socket;
	udp::endpoint remote_endpoint;

	void Receiver() {									// Reads received data and stores it in the corresponding buffer

	wait();

	io_service.run();		// run() function blocks until all work has finished and there are no more handlers to be dispatched, or until the io_service has been stopped.
}
	void wait() {																				// Receiver() helper
		for (int i = 0; i < client_bytes; i++) buffer[i] = 0;

		auto received = socket.receive_from(boost::asio::buffer(buffer), remote_endpoint);
		/*
		socket.async_receive_from(boost::asio::buffer(buffer),
								  remote_endpoint,
								  boost::bind(&C_server::handle_receive,
											  this,
											  boost::asio::placeholders::error,
											  boost::asio::placeholders::bytes_transferred));
		
		*/
		std::cout << "Received: " << received << " bytes" << std::endl;
	}
	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred) {		// Receiver() helper
		if (error) {
			std::cout << "Receive failed: " << error.message() << "\n";
			return;
		}

		//std::cout << "Received: '" << std::string(recv_buffer_0.begin(), recv_buffer_0.begin() + bytes_transferred) << "' (" << error.message() << ")\n";
		//if (--count > 0) {
		//	std::cout << "Count: " << count << "\n";
		//	wait(); }
	}
};
