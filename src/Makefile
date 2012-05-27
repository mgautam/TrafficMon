CC=g++
CFLAGS= -I/opt/local/include -I.. -g3 -O0 -Wall
LDFLAGS= -lglut -lGL -lGLU
LDFLAGS =-framework GLUT -framework OpenGL

OUTPUTDIR=../bin
SOURCES =$(wildcard *.cpp)
HEADERS =$(wildcard *.h)
OBJECTS =$(SOURCES:.cpp=.o)
TARGET= TrafficMon

all: clean $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(OUTPUTDIR)/$@ $(OBJECTS) $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) -c  $< -o $@

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(OUTPUTDIR)/$(TARGET)