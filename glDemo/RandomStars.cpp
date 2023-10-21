#include "RandomStars.h"
#include "star-vbo.h"


using namespace std;
using namespace glm;


static bool starVBOInitialised = false;

//
// StarInstance implementation
//

// Default constructor
StarInstance::StarInstance() {

	position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	size = 0.0f;
}


//
// StarField implementation
//

// Constructor.  numStars specifies how main points to store
StarField::StarField() {
}

void StarField::initialiseStarfield(GLuint numStars) {

	stars = vector<StarInstance>(numStars, StarInstance());

	random_device rd;
	mt19937 randomEngine = mt19937(rd());
	uniform_real_distribution<float> posRange = uniform_real_distribution<float>(-8.0f, 8.0f);
	uniform_real_distribution<float> sizeRange = uniform_real_distribution<float>(0.5f, 2.0f);

	for (GLuint i = 0; i < numStars; i++) {

		float x = posRange(randomEngine);
		float y = posRange(randomEngine);

		stars[i].position = glm::vec4(x, y, 0.0f, 1.0f); // z=0 since we're doing 2D rendering here
		stars[i].size = sizeRange(randomEngine);
	}

	if (!starVBOInitialised) {

		setupStarVBO();
		starVBOInitialised = true; // flag this so we don't initialise again!
	}
}

// Draw star field.  For each star we create a new transform matrix based on the instance position and size.
void StarField::render(mat4 currentTransform) {

	for (int i = 0; i < stars.size(); i++) {

		mat4 translateMatrix = translate(identity<mat4>(), vec3(stars[i].position.x, stars[i].position.y, stars[i].position.z));
		mat4 scaleMatrix = scale(identity<mat4>(), vec3(stars[i].size, stars[i].size, 1.0f));

		mat4 T = currentTransform * translateMatrix * scaleMatrix;
		glLoadMatrixf((GLfloat*)(&T));

		drawStarVBO();
	}
}