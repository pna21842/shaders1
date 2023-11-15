
#include "core.h"
#include "TextureLoader.h"
#include "shader_setup.h"
#include "ArcballCamera.h"
#include "GUClock.h"
#include "PrincipleAxes.h"
#include "AIMesh.h"
#include "Cube.h"
#include "Tetrahedron.h"


using namespace std;
using namespace glm;


#pragma region Global variables

GUClock* gameClock = nullptr;

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
Tetrahedron* tetra1 = nullptr;


// Shaders
GLuint				colourShader;
GLuint				colourShader_mvpMatrix;

GLuint				textureShader;
GLuint				textureShader_transformMat;

GLuint				redGreenShader;
GLuint				redGreenShader_mvpMatrix;
GLuint				redGreenShader_screenHeight;


// Window size
unsigned int windowWidth = 512;
unsigned int windowHeight = 512;

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
	
	gameClock = new GUClock();

#pragma region OpenGL and window setup

	// Initialise glfw and setup window
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "CIS5013", NULL, NULL);

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
	resizeWindow(window, windowWidth, windowHeight);

#pragma endregion


	// Initialise scene - geometry and shaders etc
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // setup background colour to be black
	glClearDepth(1.0f);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//
	// Setup Textures, VBOs and other scene objects
	//
	mainCamera = new ArcballCamera(0.0f, 0.0f, 1.98595f, 55.0f, 1.0f, 0.1f, 500.0f);
	
	principleAxes = new CGPrincipleAxes();

	cube = new Cube();

	tetra1 = new Tetrahedron();

	creatureMesh = new AIMesh(string("Assets\\beast\\beast.obj"));
	if (creatureMesh) {
		creatureMesh->addTexture(string("Assets\\beast\\beast_texture.bmp"), FIF_BMP);
	}

	planetMesh = new AIMesh(string("Assets\\gsphere.obj"));
	if (planetMesh) {
		planetMesh->addTexture(string("Assets\\Textures\\Hodges_G_MountainRock1.jpg"), FIF_JPEG);
	}


	// Setup shaders
	colourShader = setupShaders(string("Assets\\Shaders\\effect1.vs.txt"), string("Assets\\Shaders\\effect1.fs.txt"));
	textureShader = setupShaders(string("Assets\\Shaders\\basic_texture.vs.txt"), string("Assets\\Shaders\\basic_texture.fs.txt"));
	redGreenShader = setupShaders(string("Assets\\Shaders\\red-green-effect.vert"), string("Assets\\Shaders\\red-green-effect.frag"));;

	// Get uniform variable locations to set later
	colourShader_mvpMatrix = glGetUniformLocation(colourShader, "mvpMatrix");
	textureShader_transformMat = glGetUniformLocation(textureShader, "transformMat"); // sane varable but in different shader!
	redGreenShader_mvpMatrix = glGetUniformLocation(redGreenShader, "mvpMatrix");
	redGreenShader_screenHeight = glGetUniformLocation(redGreenShader, "screenHeight");


	//
	// 2. Main loop
	// 

	while (!glfwWindowShouldClose(window)) {

		updateScene();
		renderScene();						// Render into the current buffer
		glfwSwapBuffers(window);			// Displays what was just rendered (using double buffering).

		glfwPollEvents();					// Use this version when animating as fast as possible
	
		// update window title
		char timingString[256];
		sprintf_s(timingString, 256, "CIS5013: Average fps: %.0f; Average spf: %f", gameClock->averageFPS(), gameClock->averageSPF() / 1000.0f);
		glfwSetWindowTitle(window, timingString);
	}

	glfwTerminate();

	if (gameClock) {

		gameClock->stop();
		gameClock->reportTimingData();
	}

	return 0;
}


// renderScene - function to render the current scene
void renderScene()
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 cameraTransform = mainCamera->projectionTransform() * mainCamera->viewTransform();

	glUseProgram(redGreenShader);
	glUniformMatrix4fv(redGreenShader_mvpMatrix, 1, GL_FALSE, (GLfloat*)&cameraTransform);
	glUniform1i(redGreenShader_screenHeight, windowHeight);

	tetra1->render();

	/*
	mat4 T = cameraTransform * glm::translate(identity<mat4>(), vec3(3.0, 0.0, 3.0));
	glUseProgram(colourShader);
	glUniformMatrix4fv(colourShader_mvpMatrix, 1, GL_FALSE, (GLfloat*)&T);
	tetra1->render();
	*/

	glUseProgram(0);
}


// Function called to animate elements in the scene
void updateScene() {

	float tDelta = 0.0f;

	if (gameClock) {

		gameClock->tick();
		tDelta = (float)gameClock->gameTimeDelta();
	}
}


#pragma region Event handler functions

// Function to call when window resized
void resizeWindow(GLFWwindow* window, int width, int height)
{
	if (mainCamera) {

		mainCamera->setAspect((float)width / (float)height);
	}

	glViewport(0, 0, width, height);		// Draw into entire window

	windowWidth = width;
	windowHeight = height;
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

		float dx = float(xpos - prevMouseX);
		float dy = float(ypos - prevMouseY);

		if (mainCamera)
			mainCamera->rotateCamera(-dy, -dx);

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