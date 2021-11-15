#pragma once

#include "sVertex_types.h"
#include <string>
#include <map>

struct sModelDrawInfo
{
	sModelDrawInfo();

	std::string modelName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	// The Vertex Buffer (CPU side)
	sVertex_XYZW_RGBA_N_UV_T_B* pVertices;

	// The index buffer (CPU side)
	unsigned int* pIndices;
};

class cVAOManager
{
public:

	bool LoadModelIntoVAO(std::string fileName, sModelDrawInfo& drawInfo, unsigned int shaderProgramID);

	bool FindDrawInfoByModelName(std::string filename, sModelDrawInfo& drawInfo);

private:

	// Looking up the name of the model, it returns the 
	// draw information that we had used before to load it
	std::map< std::string, sModelDrawInfo> m_map_ModelName_to_VAOID;

};