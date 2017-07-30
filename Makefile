CC := gcc
C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
CFLAGS := -c -std=c99 -Wall -pedantic
LDFLAGS :=
RM := rm -f
MKDIR := mkdir -p
NAME := sudoku

# Make entire program
all: obj $(NAME)

# Makes necessary directories
obj:
	$(MKDIR) obj/

# Main program
$(NAME): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ $<

# Clean up
clean:
	$(RM) obj/*.o $(NAME)

