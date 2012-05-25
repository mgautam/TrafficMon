CC=g++
CFLAGS= -I/opt/local/include -I.. -lglut -lGL -lGLU -g3 -O0 -Wall
LDFLAGS= -lglut -lGL -lGLU

OUTPUTDIR=../bin
SOURCES =$(wildcard *.cpp)
HEADERS =$(wildcard *.h)
OBJECTS =$(SOURCES:.cpp=.o)
TARGET= TrafficMon

all: clean $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(OUTPUTDIR)/$@ $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) -c  $< -o $@

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(OUTPUTDIR)/$(TARGET)