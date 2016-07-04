CC = clang
LDFLAGS += `pkg-config --libs libsodium`
CFLAGS += `pkg-config --cflags libsodium`
LIBS = sodium

SRCS = selfsign.c
OBJS = $(SRCS:.c=.o)
BINS = $(SRCS:.c=)

all: $(SRCS) $(BINS)

$(BINS): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

.o:
	echo "Building $@"
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJS)

clean-all: clean
	rm $(BINS)
