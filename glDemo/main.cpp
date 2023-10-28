
#include "core.h"
#include "TextureLoader.h"
#include "ArcballCamera.h"
#include "PrincipleAxes.h"
#include "AIMesh.h"
#include "Cube.h"


using namespace std;
using namespace glm;


#pragma region Global variables

// Main camera
ArcballCamera* mainCamera = nullptr;

// Mouse tracking
bool				mouseDown = false;
double				prevMouseX, prevMouseY;

// Scene objects
CGPrincipleAxes* principleAxes = nullptr;
Cube* cube = nullptr;
AIMesh* creatureMesh = nullptr;
AIMesh* planetMesh = nullptr;

// Window size
const unsigned int initWidth = 512;
const unsigned int initHeight = 512;

#pragma endregion


// Function prototypes
void renderScene();
void updateScene();
void resizeWindow(GLFWwindow* window, int width, int height);
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseMoveHandler(GLFWwindow* window, double xpos, double ypos);
void mouseButtonHandler(GLFWwindow* window, int button, int action, int mods);
void mouseScrollHandler(GLFWwindow* window, double xoffset, double yoffset);
void mouseEnterHandler(GLFWwindow* window, int entered);


int main() {

	//
	// 1. Initialisation
	//
	
#pragma region OpenGL and window setup

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
	glfwSetCursorPosCallback(window, mouseMoveHandler);
	glfwSetMouseButtonCallback(window, mouseButtonHandler);
	glfwSetScrollCallback(window, mouseScrollHandler);
	glfwSetCursorEnterCallback(window, mouseEnterHandler);

	// Initialise glew
	glewInit();

	
	// Setup window's initial size
	resizeWindow(window, initWidth, initHeight);

#pragma endregion


	// Initialise scene - geometry and shaders etc
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // setup background colour to be black


	//
	// Setup Textures, VBOs and other scene objects
	//

	mainCamera = new ArcballCamera();
	
	principleAxes = new CGPrincipleAxes();

	cube = new Cube();

	creatureMesh = new AIMesh(string("Assets\\beast\\beast.obj"));
	if (creatureMesh) {
		creatureMesh->addTexture(string("Assets\\beast\\beast_texture.bmp"), FIF_BMP);
	}


	planetMesh = new AIMesh(string("Assets\\gsphere.obj"));
	if (planetMesh) {
		planetMesh->addTexture(string("Assets\\Textures\\Hodges_G_MountainRock1.jpg"), FIF_JPEG);
	}


	//
	// 2. Main loop
	// 

	while (!glfwWindowShouldClose(window)) {

		updateScene();
		renderScene();						// Render into the current buffer
		glfwSwapBuffers(window);			// Displays what was just rendered (using double buffering).

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

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	mat4 cameraTransform = mainCamera->projectionTransform() * mainCamera->viewTransform();

	// Render principle axes - no modelling transforms so just use cameraTransform
	glLoadMatrixf((GLfloat*)&cameraTransform);
	principleAxes->render(cameraTransform);

	// Render cube - no modelling transform so leave cameraTransform set in OpenGL and render
	cube->render();


#if 0
	if (creatureMesh) {

		// Setup transforms
		glLoadMatrixf((GLfloat*)&cameraTransform);

		creatureMesh->preRender();
		creatureMesh->render();
		creatureMesh->postRender();
	}
	
	if (planetMesh) {

		// Setup transforms
		mat4 planetTranslate = translate(identity<mat4>(), vec3(2.0f, 2.0f, 2.0f));
		mat4 T = cameraTransform * planetTranslate;
		glLoadMatrixf((GLfloat*)&T);

		planetMesh->preRender();
		planetMesh->render();
		planetMesh->postRender();
	}
#endif

}


// Function called to animate elements in the scene
void updateScene() {
}


#pragma region Event handler functions

// Function to call when window resized
void resizeWindow(GLFWwindow* window, int width, int height)
{
	if (mainCamera) {

		mainCamera->setAspect((float)width / (float)height);
	}

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
		switch (key)
		{
			default:
			{
			}
		}
	}
}


void mouseMoveHandler(GLFWwindow* window, double xpos, double ypos) {

	if (mouseDown) {

		//cout << "x = " << xpos << ", y = " << ypos << endl;
		double dx = xpos - prevMouseX;
		double dy = ypos - prevMouseY;

		if (mainCamera)
			mainCamera->rotateCamera((float)-dy, (float)-dx);

		prevMouseX = xpos;
		prevMouseY = ypos;
	}

}

void mouseButtonHandler(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT) {

		if (action == GLFW_PRESS) {

			mouseDown = true;
			glfwGetCursorPos(window, &prevMouseX, &prevMouseY);
		}
		else if (action == GLFW_RELEASE) {

			mouseDown = false;
		}
	}
}

void mouseScrollHandler(GLFWwindow* window, double xoffset, double yoffset) {

	if (mainCamera) {

		if (yoffset < 0.0)
			mainCamera->scaleRadius(1.1f);
		else if (yoffset > 0.0)
			mainCamera->scaleRadius(0.9f);
	}
}

void mouseEnterHandler(GLFWwindow* window, int entered) {
}

#pragma endregion