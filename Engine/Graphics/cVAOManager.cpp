#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "cVAOManager.h"
#include "cFileLoader.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>


sModelDrawInfo::sModelDrawInfo()
{
    VAO_ID = 0;

    VertexBufferID = 0;
    VertexBuffer_Start_Index = 0;
    numberOfVertices = 0;

    IndexBufferID = 0;
    IndexBuffer_Start_Index = 0;
    numberOfIndices = 0;
    numberOfTriangles = 0;

    pVertices = NULL;	
    pIndices = NULL;	

    return;
}

// Load model and create VAO
bool cVAOManager::LoadModelIntoVAO(std::string fileName, sModelDrawInfo& drawInfo, unsigned int shaderProgramID)
{
//------------------  Load Model From File  -------------------

    drawInfo.modelName = fileName;
    cFileLoader plyLoader;
   
    std::string fullFileName = "assets/models/" + fileName;

    plyLoader.LoadPLYFromFile(fullFileName, drawInfo);

 //--------------------  Generate Buffers  --------------------

    // Generate VAO
    glGenVertexArrays(1, &(drawInfo.VAO_ID));
    // Binds VAO with name of the VAO returned from glGenVertexArrays
    glBindVertexArray(drawInfo.VAO_ID);
    
    // (All bindings below are remembered/now accociated with this VAO object)

    // Generate Vertex Buffer Object
    glGenBuffers(1, &(drawInfo.VertexBufferID));
    // Vertex Buffer Object is bound to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
    // Copy Vertex Buffer to GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(sVertex_XYZW_RGBA_N_UV_T_B) * drawInfo.numberOfVertices, (GLvoid*)drawInfo.pVertices, GL_STATIC_DRAW);

    // Generate Index Buffer Object
    glGenBuffers(1, &(drawInfo.IndexBufferID));
    // Index Buffer Object is bound to GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);
    // Copy Vertex Buffer to GPU
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * drawInfo.numberOfIndices, (GLvoid*)drawInfo.pIndices, GL_STATIC_DRAW);

 //-------------------  Set Vertex Attributes  -------------------

    GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPosition");
    glEnableVertexAttribArray(vpos_location);	   
    glVertexAttribPointer(vpos_location, 4, GL_FLOAT, GL_FALSE, sizeof(sVertex_XYZW_RGBA_N_UV_T_B), (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, x));

    GLint vcol_location = glGetAttribLocation(shaderProgramID, "vColour");	
    glEnableVertexAttribArray(vcol_location);	
    glVertexAttribPointer(vcol_location, 4,	GL_FLOAT, GL_FALSE, sizeof(sVertex_XYZW_RGBA_N_UV_T_B), (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, r));

    GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal"); 
    glEnableVertexAttribArray(vNormal_location);	   
    glVertexAttribPointer(vNormal_location, 4, GL_FLOAT, GL_FALSE, sizeof(sVertex_XYZW_RGBA_N_UV_T_B), (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, nx));

    GLint vUVx2_location = glGetAttribLocation(shaderProgramID, "vUVx2");	
    glEnableVertexAttribArray(vUVx2_location);	  
    glVertexAttribPointer(vUVx2_location, 4, GL_FLOAT, GL_FALSE, sizeof(sVertex_XYZW_RGBA_N_UV_T_B), (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, u0));

    GLint vTangent_location = glGetAttribLocation(shaderProgramID, "vTangent");	
    glEnableVertexAttribArray(vTangent_location);	  
    glVertexAttribPointer(vTangent_location, 4, GL_FLOAT, GL_FALSE, sizeof(sVertex_XYZW_RGBA_N_UV_T_B), (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, tx));
		
    GLint vBiNormal_location = glGetAttribLocation(shaderProgramID, "vBiNormal");	
    glEnableVertexAttribArray(vBiNormal_location);	      
    glVertexAttribPointer(vBiNormal_location, 4, GL_FLOAT, GL_FALSE, sizeof(sVertex_XYZW_RGBA_N_UV_T_B), (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, bx));

//-------------------  Set VAO to Zero and Disable  -------------------

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(vcol_location);
    glDisableVertexAttribArray(vNormal_location);	    
    glDisableVertexAttribArray(vUVx2_location);	      
    glDisableVertexAttribArray(vTangent_location);	 
    glDisableVertexAttribArray(vBiNormal_location);	     

    // Store the name of the model into the map
    this->m_map_ModelName_to_VAOID[drawInfo.modelName] = drawInfo;

    return true;
}

// We don't want to return an int, likely
bool cVAOManager::FindDrawInfoByModelName(std::string filename, sModelDrawInfo& drawInfo)
{
    std::map< std::string, sModelDrawInfo>::iterator

    // find this particular model
    itDrawInfo = this->m_map_ModelName_to_VAOID.find(filename); 

    // Find it? 
    if (itDrawInfo == this->m_map_ModelName_to_VAOID.end())
        return false;

    // If we did find it, copy its draw information
    // (Returning drawInfo by reference)
    drawInfo = itDrawInfo->second; 
    return true;
}


