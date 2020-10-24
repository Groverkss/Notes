IDIR = include
SDIR = .
CC = gcc
CFLAGS = -I$(IDIR) -Wall
TARGET = q3
ODIR = $(SDIR)/obj
LIBS = -lpthread

_DEPS = util.h libs.h structdefs.h workers.h

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = q3.o util.o workers.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(TARGET)
