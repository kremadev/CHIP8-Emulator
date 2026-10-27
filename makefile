CC     = gcc
CFLAGS = -O2 -Wall -Wextra -I.include/glad/include -Iext
LIBS   = -lglfw -ldl -lm

chip8: main.o cpu.o glad.o
	$(CC) -o $@ main.o cpu.o glad.o $(LIBS)

main.o: main.c cpu.h $(wildcard ext/*.h)
	$(CC) $(CFLAGS) -c $< -o $@

cpu.o: cpu.c cpu.h
	$(CC) $(CFLAGS) -c $< -o $@

glad.o: .include/glad/src/gl.c
	$(CC) $(CFLAGS) -c $< -o $@

run: chip8
	./chip8

clean:
	rm -f chip8 *.o
