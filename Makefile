CC = clang
#CC = [path here]/afl-2.19b/afl-clang
LDFLAGS += `pkg-config --libs libsodium`
CFLAGS += `pkg-config --cflags libsodium`
LIBS = sodium

SRCS = gencert.c checkcert.c
OBJS = $(SRCS:.c=.o)
BINS = gencert checkcert

all: $(BINS)

gencert: gencert.o
	$(CC) gencert.o $(LDFLAGS) -o $@

checkcert: checkcert.o
	$(CC) checkcert.o $(LDFLAGS) -o $@

.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJS)

clean-all:
	rm $(BINS) $(OBJS)
