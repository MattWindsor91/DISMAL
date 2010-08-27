BIN       = uNsaneGame

SOURCES  = main.c graphics.c
OBJ      = $(subst .c,.o,$(SOURCES))
DEPFILES = $(subst .c,.d,$(SOURCES))

CC        = clang

LIBS      = `sdl-config --libs` -lSDL_image -lSDL_mixer -g
CFLAGS    = `sdl-config --cflags` -Wall -std=c89 -pedantic -g -DDEBUG

include dismal/Makefile

.PHONY: clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o "$(BIN)" $(LIBS)
	rm -rf *.d
	cd dismal && rm -rf *.d

-include $(DEPFILES)

%.o : %.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f *.o
	cd dismal && rm -f *.o

%.d: %.c
	@echo "Generating dependency makefile for $<."
	@$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
