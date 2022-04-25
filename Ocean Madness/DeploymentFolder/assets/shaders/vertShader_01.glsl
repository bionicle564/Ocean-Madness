// Vertex shader
#version 420

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

uniform bool isRain;
uniform vec4 RainOffsets[10];

void main()
{
	// Order of these is important
	//mvp = p * v * matModel; from C++ code
	
	mat4 MVP = matProjection * matView * matModel;
	
	vec4 position = vPosition;

	testing.y = NumberOfBones;
	vec4 totalNormal = vec4(0.0f);
	vec4 totalPosition = vec4(0.0f);
	if(NumberOfBones > 0)
	{
		for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
		{
			if(vBoneId[i] == -1) 
				continue;
			if(vBoneId[i] >=MAX_BONES) 
			{
				totalPosition = vPosition;
				break;
			}
			vec4 localPosition = BoneMatrices[vBoneId[i]] * vPosition;
			totalPosition += localPosition * vBoneWeight[i];
			vec4 localNormal = mat4(transpose(inverse(BoneMatrices[vBoneId[i]]))) * vNormal;
			//vec4 localNormal = BoneMatrices[vBoneId[i]] * vNormal;
			totalNormal += localNormal;
		}
	
	}
	else
	{
		totalNormal = vNormal;
		totalPosition = vPosition;
	}
	
	if(isRain)
	{
		totalPosition += RainOffsets[gl_InstanceID];
	}
	
    gl_Position = MVP * totalPosition; 		// Used to be: vec4(vPosition, 1.0f);	// Used to be vPos
	
	// The location of the vertex in "world" space (not screen space)
	fVertWorldLocation = matModel * totalPosition;
	

    //fVertexColour = vColour;		// Used to be vCol
	fVertexColour = vColour;
	
	
	// Calculate the normal based on any rotation we've applied.
	// This inverse transpose removes scaling and tranlation (movement) 
	// 	from the matrix.
	fNormal = matModelInverseTranspose * normalize(totalNormal);
	//fNormal = mat4(transpose(inverse(matModel))) * normalize(totalNormal);
	fNormal = normalize(fNormal);
	
	// Copy the rest of the vertex values:
	fUVx2 = vUVx2;
	
	
	vec3 viewVector = normalize(fVertWorldLocation.xyz - cameraPosition);
	viewVec = viewVector;
	reflectedVector = reflect(viewVector, fNormal.xyz);
	refractedVector = refract(viewVector, fNormal.xyz, 1.0f/1.3f);
	
};
