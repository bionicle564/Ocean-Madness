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


uniform sampler2D depthMap;		// GL_TEXTURE_2D
uniform sampler2D normalMap;		// GL_TEXTURE_2D


uniform vec4 textureRatios0to3;		//  = vec4( 1.0f, 0.0f, 0.0f, 0.0f );
uniform vec4 textureRatios4to7;		//  = vec4( 1.0f, 0.0f, 0.0f, 0.0f );


uniform samplerCube cubeMap;

layout (location = 0) out vec4 pixelColour;			// RGB Alpha   (0 to 1) 
layout (location = 1) out vec4 pixelOutputMaterialColour;		// = 1;		rga (w unused)
layout (location = 2) out vec4 pixelOutputNormal;				// = 2;		xyz (w unused)
layout (location = 3) out vec4 pixelOutputWorldPos;			// = 3;		xyz w = 0 if lit, 1 if unlit
layout (location = 4) out vec4 pixelOutputSpecular;			// = 4;		rgb, w = power
layout (location = 5) out vec4 pixelOutputDepth;			// = depth;		

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

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{

	// This is the pixel colour on the screen.
	// Just ONE pixel, though.
	
	// HACK: See if the UV coordinates are actually being passed in
	pixelColour = vec4(0.0f, 0.0f, 0.0, 1.0f); 
//	pixelColour.rg = fUVx2.xy;		// S or U as red, T or V as blue

	
	if(passNumber == PASS_1_QUAD_ONLY)
	{
		vec2 UVlookup;
		UVlookup.x = gl_FragCoord.x / screenWidthHeight.x;	// Width
		UVlookup.y = gl_FragCoord.y / screenWidthHeight.y;	// Height
		vec4 tempDepth = texture(depthMap, UVlookup.xy);
		if(tempDepth.x > .9995)
		{
			//pixelColour.rgb = (texture(texture_07, UVlookup.xy).rgb) * .5;
			vec3 colour = (texture(texture_07, UVlookup.xy).rgb) * weight[0];
			
			
			for(int i = 1; i < 5; ++i)
			{
				colour += texture(texture_07, UVlookup.xy + vec2(i, 0.0) * .001).rgb * weight[i];
				colour += texture(texture_07, UVlookup.xy - vec2(i, 0.0) * .001).rgb * weight[i];
				colour += texture(texture_07, UVlookup.xy + vec2(0.0, i) * .001).rgb * weight[i];
				colour += texture(texture_07, UVlookup.xy - vec2(0.0, i) * .001).rgb * weight[i];
			}
			
			vec3 colour2 = (texture(texture_07, UVlookup.xy).rgb) * weight[0];
			
			
			pixelColour.rgb = colour * .575;
		}
		else
		{
			pixelColour.rgb = (texture(texture_07, UVlookup.xy).rgb);
		}
		
			
		
		
		//pixelColour.rgb = vec3(1,0,0);
		return;
	}
	
	
	if(bIsSkyBox)
	{
		pixelColour.rgb += texture( cubeMap, fNormal.xyz).rgb;
		//pixelColour = vec4(1.0f,0.0f,0.0f,1.0f);
		return;
	}
	
	
	
	// Copy model vertex colours?
	vec4 vertexDiffuseColour = fVertexColour;
	
	// Use model vertex colours or not?
	if ( bUseWholeObjectDiffuseColour )
	{
		vertexDiffuseColour = wholeObjectDiffuseColour;
	}
	
	// Use debug colour?
	if ( bUseDebugColour )
	{
		// Overwrite the vertex colour with this debug colour
		vertexDiffuseColour = objectDebugColour;	
	}
	
	
	
	
	// Use the texture values as the "diffuse" colour	
	
	//float texture[4];
	//vec4 textureRatios0to3 = vec4( 1.0f, 0.0f, 0.0f, 0.0f );

//	if ( bUseTextureAsDiffuse )

	// Makes this "black" but not quite...
	vertexDiffuseColour.rgb *= 0.0001f;
	
	if(texture( texture_03, fUVx2.xy ).r >= 0.1)
	{
		discard;
	}
	
	
	
	//vertexDiffuseColour.rgb += 	
	//		(texture( texture_00, fUVx2.xy ).rgb * textureRatios0to3.x)  + 
	//	    (texture( texture_01, fUVx2.xy ).rgb * textureRatios0to3.y) ;
			
	vertexDiffuseColour.rgb = fVertexColour.rgb;
	
			//(texture( texture_02, fUVx2.xy ).rgb * textureRatios0to3.z)  + 
			//(texture( texture_03, fUVx2.xy ).rgb * textureRatios0to3.w);
			// + etc... the other 4 texture units

// Used for drawing "debug" objects (usually wireframe)
	if ( bDontLightObject )
	{
		pixelColour = vertexDiffuseColour;
		pixelColour.a = wholeObjectAlphaTransparency;
		// Early exit from shader
		return;
	}
	
	vec3 outColour = calcualteLightContribMine( vertexDiffuseColour.rgb,		
	                                        fNormal.xyz, 		// Normal at the vertex (in world coords)
                                            fVertWorldLocation.xyz,	// Vertex WORLD position
											wholeObjectSpecularColour.rgba,
											viewVec);
											
	//pixelColour = outColour;


	pixelOutputMaterialColour = vec4(normalize(fNormal.xyz), 1.0f);
	//pixelOutputNormal = vec4(fNormal.xyz, 1.0f);
	pixelOutputWorldPos.xyz = vec3(0.0f, 0.0f, 0.0f);
	pixelOutputWorldPos.w = 1;
	pixelOutputSpecular = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	pixelColour.rgb = outColour.rgb;
	//pixelColour = vec4(normalize(fNormal.xyz), 1.0f);;
 
	pixelOutputNormal = vec4(normalize(fNormal.xyz), 1.0f);
	
	pixelOutputDepth.x = gl_FragDepth;
	
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

// Calculates the colour of the vertex based on the lighting and vertex information:
vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular )
{
	vec3 norm = normalize(vertexNormal);
	
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( theLights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}
		
		// Cast to an int (note with c'tor)
		int intLightType = int(theLights[index].param1.x);
		
		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = theLights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].direction.xyz,  
									   normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1
		
			lightContrib *= dotProduct;		
			
//			finalObjectColour.rgb += (vertexMaterialColour.rgb * theLights[index].diffuse.rgb * lightContrib); 
			finalObjectColour.rgb += (vertexMaterialColour.rgb * lightContrib); 
									 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
			// NOTE: There isn't any attenuation, like with sunlight.
			// (This is part of the reason directional lights are fast to calculate)
			
			return finalObjectColour;		
		}
		
		// Assume it's a point light 
		// intLightType = 0
		
		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertexWorldPos.xyz;
		float distanceToLight = length(vLightToVertex);	
		vec3 lightVector = normalize(vLightToVertex);
		float dotProduct = dot(lightVector, vertexNormal.xyz);	 
		
		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;
			

		// Specular 
		vec3 lightSpecularContrib = vec3(0.0f);
			
		vec3 reflectVector = reflect( -lightVector, normalize(norm.xyz) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);

		// To simplify, we are NOT using the light specular value, just the object’s.
		float objectSpecularPower = vertexSpecular.w; 
		

		if(dotProduct >= 0.8)
		{
			lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
			                   * theLights[index].specular.rgb;
		}
		
		// Attenuation
		float attenuation = 1.0f / 
				( theLights[index].atten.x + 										
				  theLights[index].atten.y * distanceToLight +						
				  theLights[index].atten.z * distanceToLight*distanceToLight );  	
				  
		// total light contribution is Diffuse + Specular
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
		
		
		// But is it a spot light
		if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
		{	
		

			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertexWorldPos.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
					= dot( vertexToLight.xyz, theLights[index].direction.xyz );
					
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));
							
			// Is it completely outside of the spot?
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
									  
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
						
		}// if ( intLightType == 1 )
		
		
					
		finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb)
								  + (vertexSpecular.rgb  * lightSpecularContrib.rgb );

	}//for(intindex=0...
	
	finalObjectColour.a = 1.0f;
	
	return finalObjectColour;
}
