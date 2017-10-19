#version 120

varying vec2 texCoord0;
varying vec3 eyePos0;
varying vec3 worldPos0; 
varying vec3 normal0;

uniform bool useDiffuse;
uniform sampler2D samDiffuse;

uniform bool useOpacity;
uniform sampler2D samOpacity;

uniform vec4 color;
uniform vec4 eyePos;
uniform vec4 specular;

void main()
{
	vec3 lightDirection = vec3(-0.5777,-0.5777,0.75);
	vec3 viewDirection = worldPos0 - eyePos.xyz; 

	//DIFFUSE
	float dIntensity = clamp(dot(-lightDirection, normal0), 0,1);
	
	vec3 diffuseResult= color.xyz * dIntensity;
	if(useDiffuse)
		diffuseResult = texture2D(samDiffuse,texCoord0).xyz * dIntensity;
	
	//SPECULAR
    vec3 halfVector = -normalize(viewDirection + lightDirection);
    float sIntensity = dot(halfVector, normal0);
    sIntensity = max(sIntensity, 0);
    sIntensity = pow(sIntensity, specular.a);
	
	vec3 specularResult = specular.xyz * sIntensity;
    
	//ALPHA
	float alphaResult = color.a;
	if(useOpacity)
		alphaResult = texture2D(samOpacity,texCoord0).x * dIntensity;
	
	//RESULT
	vec3 result = specularResult + diffuseResult;

	gl_FragColor = vec4(result, alphaResult);  
}