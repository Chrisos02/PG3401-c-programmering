# Makefile template

INCLDIR = ./include
CC = gcc
CFLAGS = -O2 -pthread -include stdio.h
CFLAGS += -I$(INCLDIR)

OBJDIR = obj

_DEPS =
DEPS = $(patsubst %, $(INCLDIR)/%, $(_DEPS))

_OBJS = task4_threads.o dbj2.o tea.o
OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

task4: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~

