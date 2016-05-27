
sources := Murmur.cpp main.cpp
objects := $(sources:.cpp=.o)
cflags += -I. -std=c++14 -DHAVE_ICE=1 -Wall -ggdb
cflags += $(shell pkg-config --cflags ice)
#ldflags += $(shell pkg-config --libs ice)
ldflags += -lIce -lIceUtil -pthread

CFLAGS ?= $(cflags)
LDFLAGS ?= $(ldflags)

collected-mumble: $(objects)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $(objects)

$(objects): $(@:.o=.cpp)
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $(@:.o=.cpp)

