#include "display.h"
#include "cpu.h"

void display_draw(GLFWwindow *window) {
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	glViewport(0, 0, w, h);
	glDisable(GL_SCISSOR_TEST);
	glClearColor(0, 0, 0, 1);
	glEnable(GL_SCISSOR_TEST);
}
