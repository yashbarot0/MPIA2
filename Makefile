CC = mpicc
CFLAGS = -Wall -O2

all: q1_send_recv q1_collective

q1_send_recv: q1_send_recv.c
	$(CC) $(CFLAGS) -o q1_send_recv q1_send_recv.c

q1_collective: q1_collective.c
	$(CC) $(CFLAGS) -o q1_collective q1_collective.c

q2_decomp: q2_decomp.c
    $(CC) $(CFLAGS) -o q2_decomp q2_decomp.c

clean:
	rm -f q1_send_recv q1_collective
