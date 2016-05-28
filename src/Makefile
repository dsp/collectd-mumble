
sources := Murmur.cpp collector.cpp main.cpp
objects := $(sources:.cpp=.o)
cflags += -I. -std=c++14 \
          -DHAVE_ICE=1 \
          -Wall -Wextra -Wfloat-equal -Wconversion \
          -Wundef -Wshadow -Wpointer-arith -Wcast-align \
		  -Werror \
          -ggdb 
cflags += -DHAVE_COLLECTD=1
cflags += $(shell pkg-config --cflags ice)
#ldflags += $(shell pkg-config --libs ice)
ldflags += -lIce -lIceUtil -pthread -fPIC

CFLAGS ?= $(cflags)
LDFLAGS ?= $(ldflags)

all:: $(SOURCES) mumble.so
	sudo cp mumble.so /usr/lib/collectd/mumble.so

mumble.so: $(objects)
	$(CXX) $(CFLAGS) -shared $(LDFLAGS) -o $@ $(objects)

mumble: $(objects)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $(objects)

collectd-mumble: $(objects)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $(objects)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $<

clean:
	rm -f *.o mumble.so mumble
.PHONY: clean
