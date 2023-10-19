#pragma once

#include "core.h"

// Model single instance of a star
struct StarInstance {

	glm::vec4 position;
	glm::vec4 colour;
	float size;

	// Default constructor - zero everything out!
	StarInstance();
};


// Class to encapsulate storage, creation and rendering of a collection of random stars
class StarField {

	std::vector<StarInstance> stars;

public:

	// Constructor
	StarField();

	void initialiseStarfield(GLuint numStars);

	// Render stars
	void render();
};