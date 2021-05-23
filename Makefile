all: bin/send bin/recv

bin/send: src/send.cpp include/StopNWait.hpp include/Socket.hpp include/Timer.hpp include/Packet.hpp
	g++ -Wall -g -Iinclude src/send.cpp -o $@

bin/recv: src/recv.cpp include/StopNWait.hpp include/Socket.hpp include/Timer.hpp include/Packet.hpp
	g++ -Wall -g -Iinclude src/recv.cpp -o $@

clean: 
	rm bin/*

debug-send: bin/send
	gdb -q -x tools/gdbinit_send

debug-recv: bin/recv
	gdb -q -x tools/gdbinit_recv