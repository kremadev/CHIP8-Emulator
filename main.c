#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdio.h>
#include "cpu.h"
#include "display.h"

int main(int ac, char **av) {
	if (ac < 2) {
		printf("Usage: %s [ROM FILE]\n", av[0]);
		return 1;
	}	
	if (cpu_load_rom(av[1]) != 0) {
		return 1;
	}

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "CHIP8 Emulator", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	gladLoadGL(glfwGetProcAddress);
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();


		keypad[0x0] = (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS);
		keypad[0x1] = (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS);
		keypad[0x2] = (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS);
		keypad[0x3] = (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS);
		keypad[0x4] = (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS);
		keypad[0x5] = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
		keypad[0x6] = (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS);
		keypad[0x7] = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
		keypad[0x8] = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
		keypad[0x9] = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
		keypad[0xA] = (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS);
		keypad[0xB] = (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS);
		keypad[0xC] = (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS);
		keypad[0xD] = (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS);
		keypad[0xE] = (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS);
		keypad[0xF] = (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS);

		for (int i = 0; i < 10; i++) cpu_cycle();
		cpu_tick_timers();
		display_draw(window);

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
