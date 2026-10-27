#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdio.h>
#include "cpu.h"

int main(int ac, char **av) {
	if (ac != 2) {
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
	
	gladLoadGL(glfwGetProcAddress);
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		cpu_cycle();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
