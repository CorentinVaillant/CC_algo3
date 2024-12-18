CC=gcc
CFLAGS=-std=c99 -Wall -pedantic
LDFLAGS=-lm

ifeq ($(DEBUG),no)
	CFLAGS += -O3 -DNDEBUG
	LDFLAGS +=
else
	CFLAGS += -g
	LDFLAGS +=
endif

EXEC=quadtree_test
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all:
ifeq ($(DEBUG),no)
	@echo "Generating in release mode"
else
	@echo "Generating in debug mode"
endif
	@$(MAKE) $(EXEC)

$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC) ../Test/*.dot ../Test/*.pdf

pdf :
	@dot -Tpdf ../Test/*.dot -O

quadtree.o : quadtree.h
main.o : quadtree.o
