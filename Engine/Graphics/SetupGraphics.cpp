#include "globals.h"
#include "GLCommon.h"
#include <iostream>


void setupShader()
{
    // Create the texture manager
    _textureManager = new cBasicTextureManager();

    // Create the shader manager
    _shaderManager = new cShaderManager();

    cShaderManager::cShader vertShader;
    vertShader.fileName = "assets/shaders/vertShader_01.glsl";

    cShaderManager::cShader fragShader;
    fragShader.fileName = "assets/shaders/fragShader_01.glsl";

    if (_shaderManager->createProgramFromFile("Shader#1", vertShader, fragShader))
    {
        std::cout << "Shader compiled OK" << std::endl;
        // Set the "program" variable to the one the Shader Manager used
        _shaderProgram = _shaderManager->getIDFromFriendlyName("Shader#1");
    }
    else
    {
        std::cout << "Error making shader program: " << std::endl;
        std::cout << _shaderManager->getLastError() << std::endl;
    }

    // Select the shader program we want to use (only one shader right now)
    glUseProgram(_shaderProgram);

    // Get "uniform locations" (aka the registers these are in)
    _mvp_location = glGetUniformLocation(_shaderProgram, "MVP");
    World_Matrix_Location = glGetUniformLocation(_shaderProgram, "matModel");
    View_Matrix_Location = glGetUniformLocation(_shaderProgram, "matView");
    Projection_Matrix_Location = glGetUniformLocation(_shaderProgram, "matProjection");
    InverseTranspose_Location = glGetUniformLocation(_shaderProgram, "matModelInverseTranspose");


    // Get the uniform locations of the light shader values
    _allLights.SetUpUniformLocations(_shaderProgram);

   
}

// Set up the debug sphere
void setupDebugSphere()
{
    _pDebugSphere = new cModel();
    _pDebugSphere->modelName = "sphere.ply";
    _pDebugSphere->bIsWireframe = true;
    _pDebugSphere->bUseObjectDebugColour = true;
    _pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    _pDebugSphere->bDontLight = true;
    _pDebugSphere->scale = 1.0f;
    _pDebugSphere->positionXYZ = _allLights.theLights[_selectedLight].position;
}

void setupTextures()
{
    // FOR NOW, set all the texture to this texture
    GLuint wood_textureNumber = _textureManager->getTextureIDFromName("woodTexture.bmp");
    // Texture binding..
    GLuint textureUnit = 14;   // Texture unit go from 0 to 79
    glActiveTexture(textureUnit + GL_TEXTURE0);   // GL_TEXTURE0 = 33984
    glBindTexture(GL_TEXTURE_2D, wood_textureNumber);
    // glBindTextureUnit(texture00Unit, texture00Number);   OpenGL 4.5+ only
    // Set texture unit in the shader, too

    GLint texture_00_LocID = glGetUniformLocation(_shaderProgram, "texture_00");
    glUniform1i(texture_00_LocID, textureUnit);
}

void setupLoadModels()
{


    _VAOManager = new cVAOManager();

    // Loading Models
    sModelDrawInfo modelChair;
    _VAOManager->LoadModelIntoVAO("chair.ply", modelChair, _shaderProgram);

    sModelDrawInfo modelTable;
    _VAOManager->LoadModelIntoVAO("table.ply", modelTable, _shaderProgram);

    sModelDrawInfo modelGhost;
    _VAOManager->LoadModelIntoVAO("ghost.ply", modelGhost, _shaderProgram);

    sModelDrawInfo modelCandle;
    _VAOManager->LoadModelIntoVAO("candle.ply", modelCandle, _shaderProgram);

    sModelDrawInfo modelCandleM;
    _VAOManager->LoadModelIntoVAO("candleM.ply", modelCandleM, _shaderProgram);

    sModelDrawInfo modelCandleL;
    _VAOManager->LoadModelIntoVAO("candleL.ply", modelCandleL, _shaderProgram);

    sModelDrawInfo modelDoll;
    _VAOManager->LoadModelIntoVAO("doll.ply", modelDoll, _shaderProgram);

    sModelDrawInfo modelBook;
    _VAOManager->LoadModelIntoVAO("book.ply", modelBook, _shaderProgram);

    sModelDrawInfo modelSphere;
    _VAOManager->LoadModelIntoVAO("ISO_Shphere_flat_3div_xyz_n_rgba.ply", modelSphere, _shaderProgram);
    _VAOManager->LoadModelIntoVAO("ISO_Shphere_flat_4div_xyz_n_rgba.ply", modelSphere, _shaderProgram);
}