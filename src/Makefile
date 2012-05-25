CC=g++
CFLAGS= -I/opt/local/include -I.. -g3 -O0 -Wall -c
LDFLAGS=

OUTPUTDIR=/Users/seany/Development/TrafficMon/bin
SOURCES =$(wildcard *.cpp)
HEADERS =$(wildcard *.h)
OBJECTS =$(SOURCES:.cpp=.o)
TARGET= TrafficMon

all: clean $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(OUTPUTDIR)/$@ $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(OUTPUTDIR)/$(TARGET)