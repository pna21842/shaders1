
#include "PlanetSystem.h"
#include "TextureLoader.h"


using namespace std;
using namespace glm;


#pragma region Geometry data to setup VBOs for rendering - use triangles not quads here

static float positions[] = {

	-1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, -1.0f,
	-1.0f, -1.0f
};

static float texCoords[] = {

	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};

// Indices for 2 triangles
static GLuint indices[] = { 0, 2, 1, 0, 3, 2 };

#pragma endregion


// Constructor
SimplePlanetSystem::SimplePlanetSystem() {}


// Initialisation, update (called per-frame) and render functions
void SimplePlanetSystem::initialise(float moonScale, float moonOrbitDistance, float moonOrbitSpeed, float moonRotationSpeed) {

	// Load textures
	// Textures from Master of Orion II: Battle at Antares (https://www.spriters-resource.com/pc_computer/masteroforioniibattleatantares/sheet/206151/)
	planetTexture = loadTexture(string("Assets\\Textures\\planet01.png"), FIF_PNG);
	moonTexture = loadTexture(string("Assets\\Textures\\moon01.png"), FIF_PNG);

	// Setup VBOs for geometry
	glGenBuffers(1, &posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &texCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

	glGenBuffers(1, &indicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Unbind buffers once done
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// Initialise moon orbit parameters
	moonRotationAngle = 0.0f;
	moonOrbitAngle = 0.0f;
	this->moonScale = moonScale;
	this->moonOrbitDistance = moonOrbitDistance;
	this->moonOrbitSpeed = moonOrbitSpeed;
	this->moonRotationSpeed = moonRotationSpeed;
}


void SimplePlanetSystem::update() {

	moonRotationAngle += moonRotationSpeed;
	moonOrbitAngle += moonOrbitSpeed;
}


void SimplePlanetSystem::render() {

	// Enable Texturing
	glEnable(GL_TEXTURE_2D);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup VBOs ready for rendering
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glVertexPointer(2, GL_FLOAT, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
	glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)0);

	// Bind (and leave bound) the index array for drawing
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);

	// Declare which arrays are needed for the semi-circle object
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	// Transform and render planet

	glBindTexture(GL_TEXTURE_2D, planetTexture);

	mat4 transform = identity<mat4>();
	glLoadMatrixf((GLfloat*)&transform);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);


	// Transform and render moon

	glBindTexture(GL_TEXTURE_2D, moonTexture);

	mat4 moonScaleMatrix = scale(identity<mat4>(), vec3(moonScale, moonScale, 1.0f));
	mat4 moonRotation = rotate(identity<mat4>(), moonRotationAngle, vec3(0.0f, 0.0f, 1.0f));
	mat4 moonTranslation = translate(identity<mat4>(), vec3(moonOrbitDistance, 0.0f, 0.0f));
	mat4 moonOrbitRotation = rotate(identity<mat4>(), moonOrbitAngle, vec3(0.0f, 0.0f, 1.0f));

	transform = moonOrbitRotation * moonTranslation * moonRotation * moonScaleMatrix;
	glLoadMatrixf((GLfloat*)&transform);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);


	// Unbind buffers once done
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}