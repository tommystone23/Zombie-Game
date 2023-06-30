C=g++
CFLAGS=-Wall
LDLIBS=-lGL -lGLEW -lSDL2 -lfreetype -std=c++11
INCDIRS=-I../include

PRGM=out
SRCS := $(wildcard src/*.cpp src/**/*.cpp src/**/**/*.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

BUILD_DIR=bin

.PHONY: all clean

all: $(PRGM)

$(PRGM): $(OBJS)
		$(C) $(OBJS) $(LDLIBS) -o $(BUILD_DIR)/$@

%.o: %.cpp
		$(C) $(CFLAGS) $(INCDIRS) -c $< -o $@ 

run: all
	./$(BUILD_DIR)/$(PRGM)

clean:
	rm -rf $(OBJS) $(DEPS) $(BUILD_DIR)/$(PRGM)
