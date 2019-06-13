/*
	- Client sends a message. Before sending the message, the server must be conected (bind) to that socket. That's why
	we create both the client and the server objects before sending the message.
	- Whenever we want, we can make the server go to see if he received a message in the specified port. 
			- receive_from(): If a message arrived, he takes it. If not, he waits until a messages arrives.
			- async_receive_from(): If a message arrived, he takes it. If not, he doesn't wait for it.
*/

#include "C_server.h"
#include "C_client.h"

int main()
{
	C_client clie;	
	C_server serv;
	clie.SendData();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));


	serv.Receiver();

	std::cout << "Received data: [" << serv.buffer << "]" << std::endl;	// 20 bytes are received, but cout only prints until \0.
		
	return 0;
}
