#version 120

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal; 

varying vec2 texCoord0;
varying vec3 worldPos0;
varying vec3 eyePos0;
varying vec3 normal0;

uniform mat4 viewProj;
uniform mat4 world;

void main()
{
	texCoord0 = texCoord;
	
	normal0 = (world * vec4(normal, 0.0)).xyz;
	worldPos0 = (world * vec4(position, 1.0)).xyz;

	gl_Position = viewProj * vec4(position, 1.0);
}