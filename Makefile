CC=g++
CFLAGS=-c -Wall
LDFLAGS= -lpthread
SOURCES=tsWordTree.c wordCounter.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=wordCounter
all: $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o: 
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $(CFLAGS) $< > $*.d

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) *.d

