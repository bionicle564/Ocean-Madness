// Vertex shader
#version 430

//uniform mat4 MVP;
uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModelInverseTranspose;	// For normal calculation
uniform vec3 cameraPosition;

//uniform bool bUseVertexColour;		// Will default to GL_FALSE, which is zero (0)
//uniform vec3 vertexColourOverride;

in vec4 vColour;
in vec4 vPosition;
in vec4 vNormal;				// Vertex normal X,Y,Z (W ignored)
in vec4 vUVx2;					// 2 x Texture coords (vec4) UV0, UV1
in vec4 vTangent;				// For bump mapping X,Y,Z (W ignored)
in vec4 vBiNormal;				// For bump mapping X,Y,Z (W ignored)
in ivec4 vBoneId;
in vec4 vBoneWeight;

out vec4 fVertexColour;			// used to be "out vec3 color"
out vec4 fVertWorldLocation;
out vec4 fNormal;
out vec4 fUVx2;
out vec3 reflectedVector;
out vec3 refractedVector;
out vec3 viewVec;
out vec4 testing;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;

uniform uint NumberOfBones;
uniform mat4 BoneMatrices[MAX_BONES];

layout (location = 6) uniform float timer;

void main()
{
	// Order of these is important
	//mvp = p * v * matModel; from C++ code
	
	mat4 MVP = matProjection * matView * matModel;
	
	//vec4 position = vPosition;

	testing.x = timer;
	vec4 totalNormal = vec4(0.0f);
	vec4 totalPosition = vec4(0.0f);

	
	totalNormal = vNormal;
	totalPosition = vPosition;
	
	

	//totalPosition -= vec4(0,0,h,0);
	
	vec4 offsetAmount = vec4(2,0,0,0);
	vec4 offsetAmount2 = vec4(0,2,0,0);
	
	int width = 300;
	float xWeight = -1;
	float yWeight = -1;
	
	float adjustedTimer = 0;
	if(xWeight > 0){
		adjustedTimer = timer + radians(720);
	}
	else{
		adjustedTimer = timer + radians(-720);
		adjustedTimer += radians(720 * 50);
	}
	
	
	
	
	vec4 xVec = (offsetAmount * (gl_InstanceID % width));
	vec4 yVec = (offsetAmount2 * int(gl_InstanceID / width));
	
	int side = 1; //gl_InstanceID % 2 == 0 ? 1 : -1;
	
	float x = ((totalPosition + xVec).x * xWeight) + ((totalPosition + yVec).y * yWeight ) + adjustedTimer;
	float h = .4* sin(x) * side;
	
	//((totalPosition + yVec).y * 0 )
	//vec4 direction = vec4(((totalPosition + yVec).y * 1 ), ((totalPosition + xVec).x * 1) + 10, 0,0);
	vec4 direction = vec4( xWeight, yWeight, 0,0);
	
	direction.xyz = cross(direction.xyz, vec3(0,0,1));
	direction = normalize(direction);
	
	//vec4 newXVec = (offsetAmount * (gl_InstanceID % width == 0 ? 1 : gl_InstanceID % width));
	float slope = .4 * cos(((totalPosition + xVec).x * xWeight) + ((totalPosition + yVec).y * yWeight ) + adjustedTimer) * side;
	
	vec4 newNormal = vec4(x, 0, slope,0);
	newNormal = normalize(newNormal);
	
	newNormal.xyz = cross(newNormal.xyz,direction.xyz);
	
	totalPosition.z += h;
	
    gl_Position = MVP * (totalPosition + xVec + yVec); 		// Used to be: vec4(vPosition, 1.0f);	// Used to be vPos
	
	// The location of the vertex in "world" space (not screen space)
	fVertWorldLocation = matModel * (totalPosition + xVec + yVec);
	

    //fVertexColour = vColour;		// Used to be vCol
	fVertexColour = vColour;
	
	
	// Calculate the normal based on any rotation we've applied.
	// This inverse transpose removes scaling and tranlation (movement) 
	// 	from the matrix.
	fNormal = matModelInverseTranspose * normalize(vNormal);
	//fNormal = mat4(transpose(inverse(matModel))) * normalize(totalNormal);
	fNormal = normalize(fNormal);
	//fVertexColour = fNormal;
	// Copy the rest of the vertex values:
	fUVx2 = vUVx2;
	
	
	vec3 viewVector = normalize(fVertWorldLocation.xyz - cameraPosition);
	viewVec = viewVector;
	reflectedVector = reflect(viewVector, fNormal.xyz);
	refractedVector = refract(viewVector, fNormal.xyz, 1.0f/1.3f);
	
};
