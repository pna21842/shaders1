#version 410

//
// Example screen space rendering effect producing red/green effect
//

uniform mat4 mvpMatrix;

layout (location=0) in vec4 vertexPos;
layout (location=1) in vec4 vertexColour;

out SimplePacket {

	vec4 colour;

} outputVertex;


void main(void) {

	outputVertex.colour = vertexColour;
	gl_Position = mvpMatrix * vertexPos;
}
