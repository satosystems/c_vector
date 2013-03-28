VPATH = src test

INCLUDES = -Isrc -ICUnit-2.1-2/CUnit/Headers

LIBS = -LCUnit-2.1-2/CUnit/Sources/.libs -lcunit

OBJS = test.o

CFLAGS = -g -Wall $(INCLUDES)

TARGET = out/test

$(TARGET): $(patsubst %,out/%,$(OBJS))
	$(CC) -o $(TARGET) $^ $(LIBS)

all: $(TARGET)

out/%.o: %.c $(wildcard src/*.h)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(patsubst %,out/%,$(OBJS)) out/*

test: all
	out/test
