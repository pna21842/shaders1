
#include "core.h"
#include "TextureLoader.h"
#include "semi-circle-immediate-mode.h"
#include "semi-circle-vertexarray.h"
#include "texture-quad-immediate.h"
#include "texture-quad-vertexarray.h"
#include "texture-quad-interleaved.h"
#include "arwing.h"
#include "star-vbo.h"
#include "RandomStars.h"
#include "PlanetSystem.h"

using namespace std;
using namespace glm;


// global variables

// Example exture object
GLuint playerTexture;

vec3 offset = vec3(0.5f, 0.5f, 0.5f);
mat4 T;

StarField stars = StarField();
SimplePlanetSystem planets = SimplePlanetSystem();

// Window size
const unsigned int initWidth = 512;
const unsigned int initHeight = 512;

// Function prototypes
void renderScene();
void resizeWindow(GLFWwindow* window, int width, int height);
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void updateScene();


int main() {

	//
	// 1. Initialisation
	//
	

	// Initialise glfw and setup window
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);

	GLFWwindow* window = glfwCreateWindow(initWidth, initHeight, "Real-Time Computer Graphics", NULL, NULL);

	// Check window was created successfully
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	

	// Set callback functions to handle different events
	glfwSetFramebufferSizeCallback(window, resizeWindow); // resize window callback
	glfwSetKeyCallback(window, keyboardHandler); // Keyboard input callback


	// Initialise glew
	glewInit();

	
	// Setup window's initial size
	resizeWindow(window, initWidth, initHeight);

	// Initialise scene - geometry and shaders etc
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // setup background colour to be black


	glMatrixMode(GL_PROJECTION);
	//gluOrtho2D(-10.0f, 10.0f, -10.0f, 10.0f);
	gluOrtho2D(-5.0f, 5.0f, -5.0f, 5.0f);

	glMatrixMode(GL_MODELVIEW);


	//
	// Setup Textures, VBOs and other scene objects
	//

	playerTexture = loadTexture(string("Assets\\Textures\\player1_ship.png"), FIF_PNG);

	stars.initialiseStarfield(200);
	planets.initialise(0.5f, 3.0f, radians(0.01f), radians(0.05f));


	//
	// 2. Main loop
	// 
	

	// Loop while program is not terminated.
	while (!glfwWindowShouldClose(window)) {

		updateScene();
		renderScene();						// Render into the current buffer
		glfwSwapBuffers(window);			// Displays what was just rendered (using double buffering).

		// Poll events (key presses, mouse events)
		// glfwWaitEvents();				// Use this if no animation.
		// glfwWaitEventsTimeout(1.0/60.0);	// Use this to animate at 60 frames/sec (timing is NOT reliable)
		glfwPollEvents();					// Use this version when animating as fast as possible
	}

	glfwTerminate();
	return 0;
}


// renderScene - function to render the current scene
void renderScene()
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//static float theta = 0.0f;
	//offset.x = cosf(theta);
	//theta += glm::radians(1.0f) * 0.01f;

	//T = glm::translate(identity<mat4>(), offset);
	//glLoadMatrixf((GLfloat*)(&T));


	//drawSemiCircleImmediate();
	//drawSemiCircleVertexArray();
	//drawTexturedQuadImmediate(playerTexture);
	//drawTextureQuadVertexArray(playerTexture);
	//drawTextureQuadInterleaved(playerTexture);
	//drawArwingImmediate();
	//drawArwingVertexArray();

	//stars.render();
	planets.render();
}




// Function to call when window resized
void resizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);		// Draw into entire window
}


// Function to call to handle keyboard input
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {

		// check which key was pressed...
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, true);
				break;

			default:
			{
			}
		}
	}
	else if (action == GLFW_RELEASE) {
		// handle key release events
	}
}


// Function called to animate elements in the scene
void updateScene() {

	planets.update();
}

