IDIR = include
SDIR = .
CC = gcc
CFLAGS = -I$(IDIR) -Wall
TARGET = q2
ODIR = $(SDIR)/obj
LIBS = -lpthread

_DEPS = util.h queue.h libs.h workers.h structdefs.h

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = q2.o util.o queue.o workers.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(TARGET)
