CXX = g++
CXXFLAGS = -std=c++11 -Wall
OPENCV = $(shell pkg-config --cflags --libs opencv4)
TARGET = my_app
SRCS = main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(OPENCV)

run: $(TARGET)
	LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libpthread.so.0 LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu ./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean