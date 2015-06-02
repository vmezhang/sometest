
main : main.o proc.o sched.o sched_FIFO.o wait_queue.o cpu_init.o  Syscalls.o Simulate.o
	g++ main.o proc.o sched.o sched_FIFO.o  wait_queue.o cpu_init.o Syscalls.o Simulate.o -o main -lpthread

main.o : main.cpp proc.h sched.h cpu_init.h wait_queue.h list.h read_byte_code.h
	g++ -c main.cpp

proc.o : proc.cpp proc.h types.h defs.h byte_code.h wait_queue.h
	g++ -c proc.cpp

sched.o : sched.cpp sched.h list.h proc.h sched_FIFO.h cpu_init.h
	g++ -c sched.cpp

sched_FIFO.o : sched_FIFO.c sched_FIFO.h defs.h list.h proc.h
	gcc -c sched_FIFO.c

wait_queue.o : wait_queue.cpp wait_queue.h proc.h sched.h list.h defs.h
	g++ -c wait_queue.cpp

cpu_init.o : cpu_init.cpp cpu_init.h
	g++ -c cpu_init.cpp -lpthread

Simulate.o : Simulate.cpp Simulate.h byte_code.h proc.h sched.h cpu_init.h Syscalls.h
	g++ -c Simulate.cpp -lpthread

Syscalls.o : Syscalls.cpp Syscalls.h Globalcall.h
	g++ -c Syscalls.cpp

.PHONY: clean
clean:
	rm -f *.o

