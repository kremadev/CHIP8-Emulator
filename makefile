CC     = gcc
CFLAGS = -O2 -g -Wall -Wextra -I.include/glad/include
LIBS   = -lglfw -ldl -lm

OBJS = main.o cpu.o display.o glad.o

chip8: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

main.o: main.c cpu.h display.h
	$(CC) $(CFLAGS) -c $< -o $@

cpu.o: cpu.c cpu.h
	$(CC) $(CFLAGS) -c $< -o $@

display.o: display.c display.h cpu.h
	$(CC) $(CFLAGS) -c $< -o $@

glad.o: .include/glad/src/gl.c
	$(CC) $(CFLAGS) -c $< -o $@

run: chip8
	./chip8

clean:
	rm -f chip8 *.o

.PHONY: run clean
