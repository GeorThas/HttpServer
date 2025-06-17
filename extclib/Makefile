CC=gcc
LN=ld
CFLAGS=-Wall -std=c99

HEADERS=net.h http.h
SOURCES=net.c http.c
OBJECTS=net.o http.o

.PHONY: default clean


default: extclib.o

# Сборка объектных файлов
$(OBJECTS): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -c $(SOURCES)

# Линковка в единый объектный файл
extclib.o: $(OBJECTS)
	$(LN) -r $^ -o $@

clean:
	rm -f *.o *.i *.s