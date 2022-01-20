INCLUDE=-I ./ -I ./gen-cpp/ -I ./thrift/include/ -I ./thirdparty/libevent/include/

LIB=-L ./thrift/lib -lthrift -lthriftnb  -L ./thirdparty/libevent/lib -levent -L ./thirdparty/libevent/lib -levent_core-2.1

VPATH+=./ ./gen-cpp/

gen_cpp = ./gen-cpp/Calculator.cpp ./gen-cpp/tutorial_constants.cpp ./gen-cpp/tutorial_types.cpp

all: client server

clean: 
	rm client server

client: ${gen_cpp}
	g++ -g ${INCLUDE} ${LIB} client.cpp ${gen_cpp} -o ./client

server: ${gen_cpp}
	g++ -g ${INCLUDE} ${LIB} server.cpp ${gen_cpp} -o ./server
	