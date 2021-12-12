
#include "GLCommon.h"
#include "globals.h"



void DrawObject(cModel* currentModel, glm::mat4 worldMatrix, GLint World_Matrix_Location, GLint InverseTranspose_Location)
{
    // Alpha transparency (so this turns it off and on)
    glEnable(GL_BLEND);

    // Basic "alpha transparency" (and this is the function we're going to do)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // (Should do this outside the draw object function)
    // Get the uniform 
    GLint wholeObjectAlphaTransparency_LocID = glGetUniformLocation(_shaderProgram, "wholeObjectAlphaTransparency");

    // Set this value here
    glUniform1f(wholeObjectAlphaTransparency_LocID, currentModel->alphaTransparency);

    // Translation
    glm::mat4 Translation_Matrix = glm::translate(glm::mat4(1.0f), currentModel->positionXYZ);

    // Rotation around the Z axis
    glm::mat4 Rotation_Matrix_Z = glm::rotate(glm::mat4(1.0f), currentModel->orientationXYZ.z, glm::vec3(0.0f, 0.0f, 1.0f));

    // Rotation around the Y axis
    glm::mat4 Rotation_Matrix_Y = glm::rotate(glm::mat4(1.0f), currentModel->orientationXYZ.y, glm::vec3(0.0f, 1.0f, 0.0f));

    // Rotation around the X axis
    glm::mat4 Rotation_Matrix_X = glm::rotate(glm::mat4(1.0f), currentModel->orientationXYZ.x, glm::vec3(1.0f, 0.0f, 0.0f));

    // Scale the model
    glm::mat4 Scale_Matrix = glm::scale(glm::mat4(1.0f), glm::vec3(currentModel->scale, currentModel->scale, currentModel->scale));

    worldMatrix = worldMatrix * Translation_Matrix;
    worldMatrix = worldMatrix * Rotation_Matrix_Z;
    worldMatrix = worldMatrix * Rotation_Matrix_Y;
    worldMatrix = worldMatrix * Rotation_Matrix_X;
    worldMatrix = worldMatrix * Scale_Matrix;

    // moved view and projection to outside the draw scene loop.
    // (because they are the same for all objects)

    glUniformMatrix4fv(World_Matrix_Location, 1, GL_FALSE, glm::value_ptr(worldMatrix));

    // Inverse transpose of the model matrix
    // (Used to calculate the normal location in vertex space, using only rotation)
    glm::mat4 InverseTranspose_Matrix = glm::inverse(glm::transpose(worldMatrix));
    glUniformMatrix4fv(InverseTranspose_Location, 1, GL_FALSE, glm::value_ptr(worldMatrix));


    GLint bUseDebugColour_Location = glGetUniformLocation(_shaderProgram, "bUseObjectDebugColour"); // Self note: the getUniforLocation call is slow, so I might want to pull this out of the loop bc it will be the same once shader compiled
    GLint objectDebugColour_Location = glGetUniformLocation(_shaderProgram, "objectDebugColourRGBA");


    //If true, then the lighting contribution is NOT used.
    // This is useful for wireframe object
    GLint bDontLightObject_Location = glGetUniformLocation(_shaderProgram, "bDontLightObject");

    // The "whole object" colour (diffuse and specular)
    GLint wholeObjectDiffuseColour_Location = glGetUniformLocation(_shaderProgram, "wholeObjectDiffuseColour");
    GLint bUseWholeObjectDiffuseColour_Location = glGetUniformLocation(_shaderProgram, "bUseWholeObjectDiffuseColour");
    GLint wholeObjectSpecularColour_Location = glGetUniformLocation(_shaderProgram, "wholeObjectSpecularColour");


    // glUniform3f(objectDebugColour_Location, 0.0f, 1.0f, 1.0f);
    // glUniform1f(bUseDebugColour_Location, (float)GL_TRUE);


     // The "whole object" colour (diffuse and specular)
    if (currentModel->bUseWholeObjectDiffuseColour)
    {
        glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_TRUE);
        glUniform4f(wholeObjectDiffuseColour_Location,
            currentModel->wholeObjectDiffuseRGBA.r,
            currentModel->wholeObjectDiffuseRGBA.g,
            currentModel->wholeObjectDiffuseRGBA.b,
            currentModel->wholeObjectDiffuseRGBA.a);
    }
    else
    {
        glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_FALSE);
    }

    glUniform4f(wholeObjectSpecularColour_Location,
        currentModel->wholeObjectSpecularRGB.r,
        currentModel->wholeObjectSpecularRGB.g,
        currentModel->wholeObjectSpecularRGB.b,
        currentModel->wholeObjectShininess_SpecPower);

    // Is colour override true for this model?
    if (currentModel->bUseObjectDebugColour)
    {
        // Override the colour
        glUniform1f(bUseDebugColour_Location, (float)GL_TRUE);
        glUniform4f(objectDebugColour_Location,
            currentModel->objectDebugColourRGBA.r,
            currentModel->objectDebugColourRGBA.g,
            currentModel->objectDebugColourRGBA.b,
            currentModel->objectDebugColourRGBA.a);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(objectDebugColour_Location, (float)GL_FALSE);
    }


    // See if mesh is wanting the vertex colour override (HACK) to be used?
    if (currentModel->bDontLight)
    {
        // Override the colour...
        glUniform1f(bDontLightObject_Location, (float)GL_TRUE);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bDontLightObject_Location, (float)GL_FALSE);
    }



    // Wireframe or Fill
    if (currentModel->bIsWireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    sModelDrawInfo modelInfo;
    if (_VAOManager->FindDrawInfoByModelName(currentModel->modelName, modelInfo))
    {
        glBindVertexArray(modelInfo.VAO_ID);

        glDrawElements(GL_TRIANGLES,
            modelInfo.numberOfIndices,
            GL_UNSIGNED_INT,
            (void*)0);

        glBindVertexArray(0);
    }

    return;
}

void DrawDebugSphere(GLint World_Matrix_Location, glm::mat4 worldMatrix, GLint InverseTranspose_Location, cLightManager _allLights, int _selectedLight)
{
    // Draw a small white sphere at the location of the light
    _pDebugSphere->positionXYZ = _allLights.theLights[_selectedLight].position;
    _pDebugSphere->scale = 0.1f;
    _pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    DrawObject(_pDebugSphere, worldMatrix, World_Matrix_Location, InverseTranspose_Location);

    // Save the old debug sphere model name
    std::string oldDebugSphereModel = _pDebugSphere->modelName;

    const float LOW_RES_SPHERE_DISTANCE = 50.0f;
    const std::string LOW_RES_SPHERE_MODEL = "ISO_Shphere_flat_3div_xyz_n_rgba.ply";
    const std::string HIGH_RES_SPHERE_MODEL = "ISO_Shphere_flat_4div_xyz_n_rgba.ply";

    if (_showDebugSphere)
    {
        sLight& currentLight = _allLights.theLights[_selectedLight];

        // How far is 95% brightness?
        float distTo95Percent = _lightHelper.calcApproxDistFromAtten(0.95, // target light level
            0.01f, //how close to 0.95f(on atten graph), or how accurate it is (within 0.01 of 0.95)
            10000.0f, // infinite distance  
            currentLight.atten.x, //constant
            currentLight.atten.y, //  linear
            currentLight.atten.z, //quadratic
            cLightHelper::DEFAULTMAXITERATIONS); //how many "guesses do we want" (numerical method)

        // Draw a red sphere at 95%
        _pDebugSphere->scale = distTo95Percent;
        _pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        _pDebugSphere->modelName = (_pDebugSphere->scale < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);
        DrawObject(_pDebugSphere, worldMatrix, World_Matrix_Location, InverseTranspose_Location);


        // How far is 50% brightness?
        float distTo50Percent = _lightHelper.calcApproxDistFromAtten(0.50, // target light level
            0.01f, //how close to 0.50f(on atten graph), or how accurate it is (within 0.01 of 0.50)
            10000.0f, // infinite distance  
            currentLight.atten.x, //constant
            currentLight.atten.y, //  linear
            currentLight.atten.z, //quadratic
            cLightHelper::DEFAULTMAXITERATIONS); //how many "guesses do we want" (numerical method)

        // Draw a yellow sphere at 50%
        _pDebugSphere->scale = distTo50Percent;
        _pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        _pDebugSphere->modelName = (_pDebugSphere->scale < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);
        DrawObject(_pDebugSphere, worldMatrix, World_Matrix_Location, InverseTranspose_Location);

        // How far is 25% brightness?
        float distTo25Percent = _lightHelper.calcApproxDistFromAtten(0.25, // target light level
            0.01f, //how close to 0.25f(on atten graph), or how accurate it is (within 0.01 of 0.25)
            10000.0f, // infinite distance  
            currentLight.atten.x, //constant
            currentLight.atten.y, //  linear
            currentLight.atten.z, //quadratic
            cLightHelper::DEFAULTMAXITERATIONS); //how many "guesses do we want" (numerical method)

        // Draw a green sphere at 25%
        _pDebugSphere->scale = distTo25Percent;
        _pDebugSphere->objectDebugColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        _pDebugSphere->modelName = (_pDebugSphere->scale < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);
        DrawObject(_pDebugSphere, worldMatrix, World_Matrix_Location, InverseTranspose_Location);

        // How far is 5% brightness?
        float distTo5Percent = _lightHelper.calcApproxDistFromAtten(0.05, // target light level
            0.01f, //how close to 0.05f(on atten graph), or how accurate it is (within 0.01 of 0.05)
            10000.0f, // infinite distance  
            currentLight.atten.x, //constant
            currentLight.atten.y, //  linear
            currentLight.atten.z, //quadratic
            cLightHelper::DEFAULTMAXITERATIONS); //how many "guesses do we want" (numerical method)

        // Draw a blue sphere at 5%
        _pDebugSphere->scale = distTo5Percent;
        _pDebugSphere->objectDebugColourRGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        _pDebugSphere->modelName = (_pDebugSphere->scale < LOW_RES_SPHERE_DISTANCE ? LOW_RES_SPHERE_MODEL : HIGH_RES_SPHERE_MODEL);
        DrawObject(_pDebugSphere, worldMatrix, World_Matrix_Location, InverseTranspose_Location);

        _pDebugSphere->modelName = oldDebugSphereModel;
    }
}