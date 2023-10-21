#pragma once

#include "core.h"

//
// Model a simple planet system with a single planet in the centre and a single moon orbiting the planet
//

class SimplePlanetSystem {

	GLuint		planetTexture;
	GLuint		moonTexture;

	GLuint		posVBO;
	GLuint		texCoordVBO;
	GLuint		indicesVBO;

	float		moonScale;
	float		moonRotationAngle;
	float		moonOrbitAngle;
	float		moonOrbitDistance;
	float		moonOrbitSpeed;
	float		moonRotationSpeed;

public:

	// Constructor
	SimplePlanetSystem();
	
	// Initialisation, update (called per-frame) and render functions
	void initialise(float moonScale, float moonOrbitDistance, float moonOrbitSpeed, float moonRotationSpeed);
	void update();
	void render(glm::mat4 currentTransform);
};
