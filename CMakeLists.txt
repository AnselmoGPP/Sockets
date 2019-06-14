CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(Sockets)

FILE(GLOB all_libs 
	../boost_1_70_0/_BUILD/lib/*.a
)


ADD_EXECUTABLE(asynchronous_TCP
	src/asynchronous_TCP.cpp
)
TARGET_INCLUDE_DIRECTORIES(asynchronous_TCP PUBLIC
	../boost_1_70_0/_BUILD/include
)	
TARGET_LINK_LIBRARIES(asynchronous_TCP
	${all_libs}
	-lpthread
)


ADD_EXECUTABLE(asynchronous_UDP
	src/asynchronous_UDP.cpp
)
TARGET_INCLUDE_DIRECTORIES(asynchronous_UDP PUBLIC
	../boost_1_70_0/_BUILD/include
)	
TARGET_LINK_LIBRARIES(asynchronous_UDP
	${all_libs}
	-lpthread
)


ADD_EXECUTABLE(synchronous_TCP
	src/synchronous_TCP.cpp
)
TARGET_INCLUDE_DIRECTORIES(synchronous_TCP PUBLIC
	../boost_1_70_0/_BUILD/include
)	
TARGET_LINK_LIBRARIES(synchronous_TCP
	${all_libs}
	-lpthread
)


ADD_EXECUTABLE(synchronous_TCP_2
	src/synchronous_TCP_2.cpp
)
TARGET_INCLUDE_DIRECTORIES(synchronous_TCP_2 PUBLIC
	../boost_1_70_0/_BUILD/include
)	
TARGET_LINK_LIBRARIES(synchronous_TCP_2
	${all_libs}
	-lpthread
)


ADD_EXECUTABLE(synchronous_UDP
	src/synchronous_UDP.cpp
)
TARGET_INCLUDE_DIRECTORIES(synchronous_UDP PUBLIC
	../boost_1_70_0/_BUILD/include
)	
TARGET_LINK_LIBRARIES(synchronous_UDP
	${all_libs}
	-lpthread
)


ADD_EXECUTABLE(asynchronous_message
	src/asynchronous_message/Basics.cpp
	src/asynchronous_message/C_client.h
	src/asynchronous_message/C_server.h
)
TARGET_INCLUDE_DIRECTORIES(asynchronous_message PUBLIC
	../boost_1_70_0/_BUILD/include
)	
TARGET_LINK_LIBRARIES(asynchronous_message
	${all_libs}
	-lpthread
)
