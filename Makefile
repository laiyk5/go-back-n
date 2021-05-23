all: send recv

send: debug.hpp send.cpp
	g++ -Wall -g send.cpp -o $@

recv: debug.hpp recv.cpp
	g++ -Wall -g recv.cpp -o $@

clean: 
	rm sender receiver
sender: sender.cpp FileSender.hpp RDTSender.hpp Protocol.hpp Timer.hpp
	g++ -Wall -g sender.cpp -o $@
receiver: receiver.cpp FileReceiver.hpp RDTReceiver.hpp Protocol.hpp Timer.hpp
	g++ -Wall -g receiver.cpp -o $@

debug-sender: sender
	gdb -q -x tools/gdbinit_sender

debug-receiver: receiver
	gdb -q -x tools/gdbinit_receiver
