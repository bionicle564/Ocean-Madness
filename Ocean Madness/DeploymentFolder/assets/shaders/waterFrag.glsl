// Fragment shader
#version 430

in vec4 fVertexColour;			// The vertex colour from the original model
in vec4 fVertWorldLocation;
in vec4 fNormal;
in vec4 fUVx2;
in vec3 reflectedVector;
in vec3 refractedVector;
in vec4 testing;
in vec3 viewVec;
// Replaces gl_FragColor
out vec4 pixelColour;			// RGB Alpha   (0 to 1) 

// The "whole object" colour (diffuse and specular)
uniform vec4 wholeObjectDiffuseColour;	// Whole object diffuse colour
uniform bool bUseWholeObjectDiffuseColour;	// If true, the whole object colour is used (instead of vertex colour)
uniform vec4 wholeObjectSpecularColour;	// Colour of the specular highlight (optional)

// Alpha transparency value
uniform float wholeObjectAlphaTransparency;

// This is used for wireframe or whole object colour. 
// If bUseDebugColour is TRUE, then the fragment colour is "objectDebugColour".
uniform bool bUseDebugColour;	
uniform vec4 objectDebugColour;		

// This will not modulate the colour by the lighting contribution.
// i.e. shows object colour "as is". 
// Used for wireframe or debug type objects
uniform bool bDontLightObject;			// 1 if you want to AVOID lighting


// This is the camera eye location (update every frame)
uniform vec4 eyeLocation;
const uint PASS_0_ENTIRE_SCENE = 0;
const uint PASS_1_QUAD_ONLY = 1;
uniform uint passNumber;

uniform vec2 screenWidthHeight;

struct sLight
{
	vec4 position;			
	vec4 diffuse;	
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
					// y = ambient power
};


const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;


const int NUMBEROFLIGHTS = 20;
uniform sLight theLights[NUMBEROFLIGHTS];  	// 80 uniforms
// 
// uniform vec4 theLights[0].position;
// uniform vec4 theLights[0].diffuse;
// ...
// uniform vec4 theLights[1].position;
// uniform vec4 theLights[1].diffuse;
// ...

layout (location = 300) uniform sampler2D texture_00;		// GL_TEXTURE_2D
uniform sampler2D texture_01;		// GL_TEXTURE_2D
uniform sampler2D texture_02;		// GL_TEXTURE_2D
uniform sampler2D texture_03;		// GL_TEXTURE_2D
uniform sampler2D texture_04;		// GL_TEXTURE_2D
uniform sampler2D texture_05;		// GL_TEXTURE_2D
uniform sampler2D texture_06;		// GL_TEXTURE_2D
uniform sampler2D texture_07;		// GL_TEXTURE_2D


uniform vec4 textureRatios0to3;		//  = vec4( 1.0f, 0.0f, 0.0f, 0.0f );
uniform vec4 textureRatios4to7;		//  = vec4( 1.0f, 0.0f, 0.0f, 0.0f );


uniform samplerCube cubeMap;

layout (location = 2) out vec4 pixelOutputNormal;		

uniform bool bIsSkyBox;
uniform bool bReflect;
uniform bool bRefract;
uniform float rand;

uniform bool bIsFbo;

vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular );

vec3 calcualteLightContribMine( vec3 vertexMaterialColour, vec3 vertexNormal, 
                                vec3 vertexWorldPos, vec4 vertexSpecular,
								vec3 viewingVector);


void main()
{

	// This is the pixel colour on the screen.
	// Just ONE pixel, though.
	
	// HACK: See if the UV coordinates are actually being passed in
	pixelColour = vec4(0.0f, 0.0f, 0.0, 1.0f); 
//	pixelColour.rg = fUVx2.xy;		// S or U as red, T or V as blue

	
	
	
	
	
	// Copy model vertex colours?
	vec4 vertexDiffuseColour = fVertexColour;
	

	//vertexDiffuseColour.rgb *= .001;
	
	float texOffset = testing.x / (3.14 * 2);
	vec2 changedUVs = fUVx2.xy;
	
	changedUVs.x += texOffset + .02;
	//changedUVs.x +=  .02;
	changedUVs.y += .931;
	
	vertexDiffuseColour.rgb += (texture( texture_00, fUVx2.xy).rgb) + 
		   (texture( texture_00, changedUVs.xy + texOffset).rgb * .5) ;
			

	
			//(texture( texture_02, fUVx2.xy ).rgb * textureRatios0to3.z)  + 
			//(texture( texture_03, fUVx2.xy ).rgb * textureRatios0to3.w);
			// + etc... the other 4 texture units


	vec3 outColour = calcualteLightContribMine( vertexDiffuseColour.rgb,		
	                                        fNormal.xyz, 		// Normal at the vertex (in world coords)
                                            fVertWorldLocation.xyz,	// Vertex WORLD position
											wholeObjectSpecularColour.rgba,
											viewVec);
											
	pixelColour.rgb = outColour.rgb * .5;
	//pixelColour.rgb = reflectedVector.rgb;

	
	//pixelColour.rgb = vertexDiffuseColour.rgb;
	//pixelColour.rgb = vec3(0,0,1);
	
	
	pixelOutputNormal = fNormal;
	pixelColour.a = 1.0f;
	
	if(bReflect)
	{
		//pixelColour.rgb = vec3(0,1,1);
		vec4 reflection = texture(cubeMap, reflectedVector);
		pixelColour += reflection;
		return;
	}
	
	if(bRefract)
	{
		
		vec4 refraction = texture(cubeMap, -reflectedVector);
		pixelColour += refraction;
		return;
	}
	
	//pixelColour.a =  texture( texture_02, fUVx2.xy ).g;	
	
	// Set the alpha value
	// 0.0 = clear
	// 1.0 = solid
	//pixelColour.a = wholeObjectAlphaTransparency;		
		
};



vec3 calcualteLightContribMine( vec3 vertexMaterialColour, vec3 vertexNormal, 
                                vec3 vertexWorldPos, vec4 vertexSpecular,
								vec3 viewingVector)
{
	vec3 rtn = vec3(0,0,0);

	for(int i=0;i<NUMBEROFLIGHTS;i++)
	{
		if ( theLights[i].param2.x < 0.5f )
		{	// it's off
			continue;
		}
		
		float ambientStrength = theLights[i].param2.y;
		vec3 ambient = ambientStrength * theLights[i].diffuse.rgb;

		vec3 lightDir = vec3(0,0,0);
		
		lightDir = normalize(theLights[i].position.xyz - vertexWorldPos.xyz);
		float distanceToLight    = length(theLights[i].position.xyz - vertexWorldPos.xyz);
		float attenuation;
		
		attenuation = 1.0 / (theLights[i].atten.x + 
									   theLights[i].atten.y * distanceToLight + 
									   theLights[i].atten.z * (distanceToLight * distanceToLight)); 
		
		int intLightType = int(theLights[i].param1.x);
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )
		{
			lightDir = normalize(-theLights[i].direction.xyz);
			
			attenuation = 1.0;
		}
		else if(intLightType == SPOT_LIGHT_TYPE)
		{
			
			float theta = dot(lightDir, normalize(-theLights[i].direction.xyz));
			
			float epsilon   = cos(theLights[i].param1.y) - cos(theLights[i].param1.z);
			float intensity = clamp((theta - cos(theLights[i].param1.z)) / epsilon, 0.0, 1.0);    
			
			if(theta > cos(theLights[i].param1.z))
			{
				float diff = max(dot(vertexNormal.xyz, lightDir), 0.0);
				vec3 diffuse = diff * theLights[i].diffuse.rgb;
				
				vec3 viewDir = normalize(viewingVector);
				vec3 reflectDir = reflect(-lightDir, vertexNormal.xyz); 
				
				float spec = pow(max(dot(viewDir, reflectDir), 0.0),  32);
				vec3 specular = vertexSpecular.a * spec * theLights[i].diffuse.rgb;
				
				ambient *= attenuation;
				diffuse *= attenuation * intensity;
				specular *= attenuation* intensity;
				
				rtn += (ambient + diffuse + specular) * vertexMaterialColour.rgb; //outColour;
			}
			else
			{
				rtn += (ambient) * vertexMaterialColour.rgb; //outColour;
			}
			
			continue;
		}
			
		float diff = max(dot(vertexNormal.xyz, lightDir), 0.0);
		vec3 diffuse = diff * theLights[i].diffuse.rgb;
		
		vec3 viewDir = normalize(viewingVector);
		vec3 reflectDir = reflect(-lightDir, vertexNormal.xyz); 
		
		float spec = pow(max(dot(viewDir, reflectDir), 0.0),  32);
		vec3 specular = vertexSpecular.a * spec * theLights[i].diffuse.rgb;
		
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;
		
		rtn += (ambient + diffuse + specular) * vertexMaterialColour.rgb; //outColour;
		
	}
	return rtn;
}

