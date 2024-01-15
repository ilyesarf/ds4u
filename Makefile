BUILDDIR=$(CURDIR)/bin

CC = clang
CFLAGS = -Wall -g
LDFLAGS = -lusb-1.0

SOURCES = ds4u.c reader.c
OBJECTS = $(addprefix $(BUILDDIR)/,$(SOURCES:.c=.o))

.PHONY: all clean

all: $(BUILDDIR)/ds4u

$(BUILDDIR)/ds4u: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILDDIR)/%.o: %.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)
