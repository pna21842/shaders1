#pragma once

#include "core.h"

// Model single instance of a star (not the geometry - this is stored in star-vbo!
struct StarInstance {

	glm::vec4 position;
	glm::vec4 colour;
	float size;

	// Default constructor - zero everything out!
	StarInstance();
};


// Class to encapsulate storage, creation and rendering of a collection of random stars
class StarField {

	std::vector<StarInstance> stars; // use std vector container to store store instances

public:

	// Constructor
	StarField();

	// Initialise star instances (based on random points example from week3 solutions)
	void initialiseStarfield(GLuint numStars);

	// Render stars - we bring in any previous transformations (usually the camera) we need to apply
	void render(glm::mat4 currentTransform);
};