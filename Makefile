#-----------------------------------------------------------------------------
# Copyright (C) 2023, Tony Mountifield
#-----------------------------------------------------------------------------
#
# File:		Makefile
#
# Description:	Makefile for firewall-cmd wrapper
#
# Author:	Tony Mountifield
# Date:		14-Jul-2023
#
#-----------------------------------------------------------------------------

CC = gcc
INCLUDES = 
WRAPSRCS = fwcwrap.c cmd.c
WRAPOBJS = $(WRAPSRCS:.c=.o)
WRAPDEPS = $(WRAPSRCS:.c=.d)
WRAPPROG = fwcwrap

BINDIR=/usr/local/bin

CFLAGS = -Wall -Werror -g -O2 -MMD -MP $(INCLUDES) -D_GNU_SOURCE -D_REENTRANT -D_THREAD_SAFE
LFLAGS = -Wl,-Map,$@.map -Wl,--cref

all: $(WRAPPROG)

-include $(WRAPDEPS)

$(WRAPPROG): $(WRAPOBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $(WRAPOBJS) $(LIBS)
	@echo "NB: $@ needs to be installed suid root"

clean:
	rm -f $(WRAPPROG) $(WRAPPROG).map $(WRAPOBJS) $(WRAPDEPS)

install: all
	install -m 4755 $(WRAPPROG) $(BINDIR)
