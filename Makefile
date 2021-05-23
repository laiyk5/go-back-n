all: bin/send bin/recv

bin/send: src/send.cpp include/Socket.hpp include/Timer.hpp include/Packet.hpp
	g++ -Wall -g -Iinclude src/send.cpp -o $@

bin/recv: src/recv.cpp include/Socket.hpp include/Timer.hpp include/Packet.hpp
	g++ -Wall -g -Iinclude src/recv.cpp -o $@

clean: 
	rm bin/*
sender: sender.cpp FileSender.hpp RDTSender.hpp Protocol.hpp Timer.hpp
	g++ -Wall -g sender.cpp -o $@
receiver: receiver.cpp FileReceiver.hpp RDTReceiver.hpp Protocol.hpp Timer.hpp
	g++ -Wall -g receiver.cpp -o $@

debug-sender: sender
	gdb -q -x tools/gdbinit_sender

debug-receiver: receiver
	gdb -q -x tools/gdbinit_receiver
