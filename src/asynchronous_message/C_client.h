#pragma once

// Server and client boost socket: https://stackoverflow.com/questions/44273599/udp-communication-using-c-boost-asio
// Boost socket class: https://www.boost.org/doc/libs/1_41_0/doc/html/boost_asio/reference/ip__udp/socket.html
// Asynchronous: https://stackoverflow.com/questions/44273599/udp-communication-using-c-boost-asio
// Bit padding

/*
----- SEND -----
	boost::asio::io_service io_service;
	udp::socket socket;
	udp::endpoint remote_endpoint = udp::endpoint(address::from_string(IPADDRESS), UDP_PORT);

	socket(io_service)
	socket.open(udp::v4());
	
	boost::system::error_code err;
	auto sent = socket.send_to(boost::asio::buffer(&m_data, bytes_size), remote_endpoint, 0, err);

	socket.close();
*/

#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
//#include <thread>

using boost::asio::ip::udp;
using boost::asio::ip::address;

#define IPADDRESS "127.0.0.1"			// Loopback IP direction (Local host): 127.0.0.1
#define UDP_PORT 38252

const int server_bytes = 20;			// Number of bytes contained in the package we send. Internal buffer max.: ~3274

class C_client {
public:
	C_client() : socket(io_service) {
		socket.open(udp::v4());						// boost::asio::ip::udp::v4() -> Construct to represent the IPv4 UDP protocol
	}
	~C_client() {
		socket.close();
	}

	boost::asio::io_service io_service;				// Link to the OS's I/O services
	udp::socket socket;								// The I/O object
	udp::endpoint remote_endpoint = udp::endpoint(address::from_string(IPADDRESS), UDP_PORT);	// Destination: client IP, server Port

	char data[server_bytes] = "Hello world!\0 ABC";
	void SendData() {								// Sends the data
	
		//string abc = "hello socket";
		//boost::asio::buffer(abc);					// Va en socket.send_to()		Envía una string
		//boost::asio::buffer(&data, server_bytes)	// Va en socket.send_to()		Envía un array de X bytes

		boost::system::error_code err;

		auto sent = socket.send_to(boost::asio::buffer(&data, server_bytes), remote_endpoint, 0, err);	// <<<<<<<<<< Sent all or only internal buffer size??
		std::cout << "Sent: " << sent << " bytes" << std::endl;
		std::cout << "Error data: [" << err.value() << "] " << err.message() << std::endl;
	}
};

