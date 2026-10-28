#include "display.h"
#include "cpu.h"

#define BACK 0, 0, 0, 1
#define FRONT 1, 1, 1, 1

void display_draw(GLFWwindow *window) {
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	glViewport(0, 0, w, h);
	int pixel_w, pixel_h;
	pixel_h = h / 32;
	pixel_w = w / 64;
	
	glDisable(GL_SCISSOR_TEST);
	glClearColor(BACK);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_SCISSOR_TEST);
	glClearColor(FRONT);

	for (int r = 0; r < 32; r++) {
		for (int c = 0; c < 64; c++) {
			if (buffer[r * 64 + c] == 1) {
				glScissor(c * pixel_w, (31 - r) * pixel_h, pixel_w, pixel_h);
				glClear(GL_COLOR_BUFFER_BIT);
			}
		}
	}
}
