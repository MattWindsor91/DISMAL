##########################################################################
#                                                                        #
#  Copyright 2010       CaptainHayashi etc.                              #
#                                                                        #
#  This file is part of DISMAL.                                          #
#                                                                        #
#  DISMAL is free software: you can redistribute it and/or modify        #
#  it under the terms of the GNU General Public License as published by  #
#  the Free Software Foundation, either version 3 of the License, or     #
#  (at your option) any later version.                                   #
#                                                                        #
#  DISMAL is distributed in the hope that it will be useful,             #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
#  GNU General Public License for more details.                          #
#                                                                        #
#  You should have received a copy of the GNU General Public License     #
#  along with DISMAL.  If not, see <http://www.gnu.org/licenses/>.       #
#                                                                        #
##########################################################################

BIN       = uNsaneGame

SOURCES  = main.c graphics.c
OBJ      = $(subst .c,.o,$(SOURCES))
DEPFILES = $(subst .c,.d,$(SOURCES))

CC        = clang

DISMALROOT = ../../

LIBS      = `sdl-config --libs` -lSDL_image -lSDL_mixer -g
CFLAGS    = `sdl-config --cflags` -ansi -pedantic -O2 -g -DDEBUG \
            -I$(DISMALROOT) -Wall -Wextra

include $(DISMALROOT)dismal/Makefile

.PHONY: clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o "$(BIN)" $(LIBS)
	rm -rf *.d
	cd $(DISMALROOT)dismal && rm -rf *.d

-include $(DEPFILES)

%.o : %.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f *.o *.d
	cd $(DISMALROOT)dismal && rm -f *.o *.d

%.d: %.c
	@echo "Generating dependency makefile for $<."
	@$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
