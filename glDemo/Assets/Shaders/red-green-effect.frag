#version 410

//
// Example screen space rendering effect producing a red/green effect
//

uniform int screenHeight;

in SimplePacket {

	vec4 colour;

} inputFragment;

layout (location=0) out vec4 fragColour;


//
// rgb2hsv and hsv2rgb colour space conversion - from http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
//
vec3 rgb2hsv(vec3 c) {

    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c) {

    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


void main(void) {

	vec3 rgb = inputFragment.colour.rgb;

    // change hue to blue-ish colour
    vec3 hsv = rgb2hsv(rgb);
    
    if (gl_FragCoord.y < (screenHeight / 2)) {
        hsv.x = 0.0;
    }
    else {
        hsv.x = 0.34;
    }


    rgb = hsv2rgb(hsv);

    // Based on pixel y coordinate, perform mod function so ym=0 every few pixel rows
    // When performing rgb * ym is performed, the colour is zeroed out (so it becomes black)
    // on those ym=0 rows
    float ym = mod(gl_FragCoord.y / 2.0, 3);
	rgb *= ym;

    // Set final fragment colour by combining converted rgb values with alpha value of input colour
	fragColour = vec4(rgb, inputFragment.colour.a);
}
