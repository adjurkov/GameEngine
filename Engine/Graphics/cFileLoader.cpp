#include "cFileLoader.h"
#include "cModel.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <istream>
#include <iomanip> 



// Loads .ply model from file and formats it so the GPU understands
bool cFileLoader::LoadPLYFromFile(std::string fileName, sModelDrawInfo& drawInfo)
{
    std::ifstream file(fileName.c_str());
    std::string nextFileItem;

    // Check if file is open
    if (!file.is_open())
        return false;

    // Read file until the word "vertex" is found
    while (file >> nextFileItem)
    {
        if (nextFileItem == "vertex")
            break;
    }
    // Read the number of vertices
    file >> drawInfo.numberOfVertices;

    // Read file until the word "face" is found
    while (file >> nextFileItem)
    {
        if (nextFileItem == "face")
            break;
    }
    // Read the number of triangles/faces
    file >> drawInfo.numberOfTriangles;

    // Read file until the word "end_header" is found
    while (file >> nextFileItem)
    {
        if (nextFileItem == "end_header")
            break;
    }

    // Store vertices into a vector (CPU)
    std::vector<sVertex_XYZW_RGBA_N_UV_T_B> vertexVector;
    for (unsigned int i = 0; i < drawInfo.numberOfVertices; i++)
    {
        sVertex_XYZW_RGBA_N_UV_T_B tempVertex;

        file >> tempVertex.x;
        file >> tempVertex.y;
        file >> tempVertex.z;

        file >> tempVertex.nx;
        file >> tempVertex.ny;
        file >> tempVertex.nz;

        file >> tempVertex.r;
        file >> tempVertex.g;
        file >> tempVertex.b;
        file >> tempVertex.a;

        // Load UVs
        file >> tempVertex.u0; // maybe try nx if this doesnt work
        file >> tempVertex.v0; // ny?


        vertexVector.push_back(tempVertex);
    }


    struct sTriangle
    {
        unsigned int vertIndex[3];
    };

    // Store triangles into a vector (CPU)
    std::vector<sTriangle> triangleVector;
    for (unsigned int i = 0; i < drawInfo.numberOfTriangles; i++)
    {
        sTriangle tempTri;

        // Don't store the "3" at the start of the file line
        int discardThis;

        // example: 3 3087 3639 5315 
        file >> discardThis;
        file >> tempTri.vertIndex[0];
        file >> tempTri.vertIndex[1];
        file >> tempTri.vertIndex[2];

        triangleVector.push_back(tempTri);
    }

    // Allocate the amount of space we need for the GPU side arrays
    // Pointer to array of vertices
    drawInfo.pVertices = new sVertex_XYZW_RGBA_N_UV_T_B[drawInfo.numberOfVertices];

    // 3 indices per triangle
    drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;
    // Pointer to array of indices
    drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];

    // Copy format that we read from file to format GPU understands
    for (unsigned int i = 0; i != drawInfo.numberOfVertices; i++)
    {
        // Copy position
        drawInfo.pVertices[i].x = vertexVector[i].x;
        drawInfo.pVertices[i].y = vertexVector[i].y;
        drawInfo.pVertices[i].z = vertexVector[i].z;
        // If unsure about the 4th value of a vec4, make it 1.0f
        drawInfo.pVertices[i].w = 1.0f;

        // Copy colour and convert from 0-255 into 0-1
        drawInfo.pVertices[i].r = (vertexVector[i].r / 255.0f);
        drawInfo.pVertices[i].g = (vertexVector[i].g / 255.0f);
        drawInfo.pVertices[i].b = (vertexVector[i].b / 255.0f);
        drawInfo.pVertices[i].a = (vertexVector[i].a / 255.0f);

        drawInfo.pVertices[i].nx = vertexVector[i].nx;
        drawInfo.pVertices[i].ny = vertexVector[i].ny;
        drawInfo.pVertices[i].nz = vertexVector[i].nz;
        drawInfo.pVertices[i].nw = 1.0f;		

        drawInfo.pVertices[i].u0 = vertexVector[i].u0;
        drawInfo.pVertices[i].v0 = vertexVector[i].v0;
        drawInfo.pVertices[i].u1 = 0.0f;
        drawInfo.pVertices[i].v1 = 0.0f;
    }

    // Copy the triangle ("index") values to the index (element) array
    unsigned int elementIndex = 0;
    for (unsigned int i = 0; i < drawInfo.numberOfTriangles; i++, elementIndex += 3)
    {
        drawInfo.pIndices[elementIndex + 0] = triangleVector[i].vertIndex[0];
        drawInfo.pIndices[elementIndex + 1] = triangleVector[i].vertIndex[1];
        drawInfo.pIndices[elementIndex + 2] = triangleVector[i].vertIndex[2];
    }

    return true;
}

// Loads all scene information (model positions, rotations etc.)
bool cFileLoader::LoadCurrentScene(std::string fileName, std::vector<cModel*> &allModels)
{
    allModels.clear();

    std::ifstream file(fileName.c_str());

    // Did it open?
    if (!file.is_open())
    {
        std::cout << "Couldn't open file" << std::endl;
        return false;
    }
        

    std::string nextFileItem;

    // Read until end of file
    while (!file.eof())
    {
        cModel* currentModel = new cModel();

        // Read file until the word "Model Name:" is found
        while (file >> nextFileItem)
        {
            if (nextFileItem == "ModelName:")
            {
                break;
            }    
        }
        file >> currentModel->modelName;
        

        // Read file until the word "isWireframe:" is found
        while (file >> nextFileItem)
        {
            if (nextFileItem == "isWireframe:")
                break;
        }
        file >> currentModel->bIsWireframe;

        // Read file until the word "isColOverride:" is found
        while (file >> nextFileItem)
        {
            if (nextFileItem == "isColOverride:")
                break;
        }
        file >> currentModel->bUseObjectDebugColour;

        // Read file until the word "Colour r:" is found
        while (file >> nextFileItem)
        {
            if (nextFileItem == "ColourR:")
                break;
        }
        file >> currentModel->objectDebugColourRGBA.r;

        // Read file until the word "Colour g:" is found
        while (file >> nextFileItem)
        {
            if (nextFileItem == "ColourG:")
                break;
        }
        file >> currentModel->objectDebugColourRGBA.g;

        // Read file until the word "Colour b:" is found
        while (file >> nextFileItem)
        {
            if (nextFileItem == "ColourB:")
                break;
        }
        file >> currentModel->objectDebugColourRGBA.b;

        // Read file until the word "Pos x:" is found
        while (file >> nextFileItem)
        {
            if (nextFileItem == "PosX:")
                break;
        }
        file >> currentModel->positionXYZ.x;

        // Read file until the word "Pos y:" is found
        while (file >> nextFileItem)
        {
            if (nextFileItem == "PosY:")
                break;
        }
        file >> currentModel->positionXYZ.y;

        // Read file until the word "Pos z:" is found
        while (file >> nextFileItem)
        {
            if (nextFileItem == "PosZ:")
                break;
        }
        file >> currentModel->positionXYZ.z;

        // Read file until the word "Scale:" is found
        while (file >> nextFileItem)
        {
            if (nextFileItem == "Scale:")
                break;
        }
        file >> currentModel->scale;

        // Read file until the word "Rot x:" is found
        while (file >> nextFileItem)
        {
            if (nextFileItem == "RotX:")
                break;
        }
        file >> currentModel->orientationXYZ.x; // in radians

        // Read file until the word "Rot y:" is found
        while (file >> nextFileItem)
        {
            if (nextFileItem == "RotY:")
                break;
        }
        file >> currentModel->orientationXYZ.y;

        // Read file until the word "Rot z:" is found
        while (file >> nextFileItem)
        {
            if (nextFileItem == "RotZ:")
                break;
        }
        file >> currentModel->orientationXYZ.z;

        allModels.push_back(currentModel);
    }
    return true;
}

// Load Initial Camera Properties
bool cFileLoader::SetupCamera(std::string fileName, glm::vec3& cameraEyeStartPosition, glm::vec3& cameraInitialTarget, glm::vec3& upVector, float& cameraSpeed)
{
    std::ifstream File(fileName.c_str());

    // Did it open?
    if (!File.is_open())
    {
        std::cout << "Couldn't open file" << std::endl;
        return false;
    }

    std::string nextFileItem;

    while (!File.eof())
    {
        // Read file until the word "CameraStartX:" is found
        while (File >> nextFileItem)
        {
            if (nextFileItem == "CameraStartX:")
            {
                break;
            }
        }
        File >> cameraEyeStartPosition.x;

        // Read file until the word "CameraStartY:" is found
        while (File >> nextFileItem)
        {
            if (nextFileItem == "CameraStartY:")
            {
                break;
            }
        }
        File >> cameraEyeStartPosition.y;

        // Read file until the word "CameraStartZ:" is found
        while (File >> nextFileItem)
        {
            if (nextFileItem == "CameraStartZ:")
            {
                break;
            }
        }
        File >> cameraEyeStartPosition.z;

        // Read file until the word "CameraSpeed:" is found
        while (File >> nextFileItem)
        {
            if (nextFileItem == "CameraSpeed:")
            {
                break;
            }
        }
        File >> cameraSpeed;

        // Read file until the word "TargetX:" is found
        while (File >> nextFileItem)
        {
            if (nextFileItem == "TargetX:")
            {
                break;
            }
        }
        File >> cameraInitialTarget.x;

        // Read file until the word "TargetY:" is found
        while (File >> nextFileItem)
        {
            if (nextFileItem == "TargetY:")
            {
                break;
            }
        }
        File >> cameraInitialTarget.y;

        // Read file until the word "TargetZ:" is found
        while (File >> nextFileItem)
        {
            if (nextFileItem == "TargetZ:")
            {
                break;
            }
        }
        File >> cameraInitialTarget.z;

        // Read file until the word "UpVectorX:" is found
        while (File >> nextFileItem)
        {
            if (nextFileItem == "UpVectorX:")
            {
                break;
            }
        }
        File >> upVector.x;

        // Read file until the word "UpVectorY:" is found
        while (File >> nextFileItem)
        {
            if (nextFileItem == "UpVectorY:")
            {
                break;
            }
        }
        File >> upVector.y;

        // Read file until the word "UpVectorZ:" is found
        while (File >> nextFileItem)
        {
            if (nextFileItem == "UpVectorZ:")
            {
                break;
            }
        }
        File >> upVector.z;
    }
    return true;
}

// Load Initial Lighting Properties
bool cFileLoader::SetupLights(std::string fileName, cLightManager& allLights)
{
    int numLights = 2;
    std::ifstream File(fileName.c_str());

    // Did it open?
    if (!File.is_open())
    {
        std::cout << "Couldn't open file" << std::endl;
        return false;
    }

    std::string nextFileItem;

    while (!File.eof())
    {
        for (int i = 0; i < numLights; i++)
        {
            // Read file until the word "PosX:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "PosX:"){break;}
            }
            File >> allLights.theLights[i].position.x;

            // Read file until the word "PosY:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "PosY:"){break;}
            }
            File >> allLights.theLights[i].position.y;

            // Read file until the word "PosZ:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "PosZ:"){break;}
            }
            File >> allLights.theLights[i].position.z;

            // Read file until the word "diffuseR:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "diffuseR:"){break;}
            }
            File >> allLights.theLights[i].diffuse.r;

            // Read file until the word "diffuseG:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "diffuseG:") { break; }
            }
            File >> allLights.theLights[i].diffuse.g;

            // Read file until the word "diffuseG:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "diffuseB:") { break; }
            }
            File >> allLights.theLights[i].diffuse.b;

            // Read file until the word "diffuseA:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "diffuseA:") { break; }
            }
            File >> allLights.theLights[i].diffuse.a;

            // Read file until the word "specularR:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "specularR:") { break; }
            }
            File >> allLights.theLights[i].specular.r;

            // Read file until the word "specularG:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "specularG:") { break; }
            }
            File >> allLights.theLights[i].specular.g;

            // Read file until the word "specularB:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "specularB:") { break; }
            }
            File >> allLights.theLights[i].specular.b;

            // Read file until the word "specularPower:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "specularPower:") { break; }
            }
            File >> allLights.theLights[i].specular.w;

            // Read file until the word "attenConst:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "attenConst:") { break; }
            }
            File >> allLights.theLights[i].atten.x;

            // Read file until the word "attenLin:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "attenLin:") { break; }
            }
            File >> allLights.theLights[i].atten.y;

            // Read file until the word "attenQuad:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "attenQuad:") { break; }
            }
            File >> allLights.theLights[i].atten.z;

            // Read file until the word "attenDistCut:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "attenDistCut:") { break; }
            }
            File >> allLights.theLights[i].atten.w;

            // Read file until the word "dirX:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "dirX:") { break; }
            }
            File >> allLights.theLights[i].direction.x;

            // Read file until the word "dirY:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "dirY:") { break; }
            }
            File >> allLights.theLights[i].direction.y;

            // Read file until the word "dirZ:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "dirZ:") { break; }
            }
            File >> allLights.theLights[i].direction.z;

            // Read file until the word "param1X:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "param1X:") { break; }
            }
            File >> allLights.theLights[i].param1.x;

            // Read file until the word "param1Y:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "param1Y:") { break; }
            }
            File >> allLights.theLights[i].param1.y;

            // Read file until the word "param1Z:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "param1Z:") { break; }
            }
            File >> allLights.theLights[i].param1.z;

            // Read file until the word "param2X:" is found
            while (File >> nextFileItem)
            {
                if (nextFileItem == "param2X:") { break; }
            }
            File >> allLights.theLights[i].param2.x;
        }
    }
    return true;
}

// Write all modified scene information to file (model positions, rotations etc.)
bool cFileLoader::UpdateCurrentScene(std::string fileName, std::vector<cModel*>& allModels, glm::vec3& cameraEyeStartPosition, glm::vec3& cameraInitialTarget, glm::vec3& upVector, float& cameraSpeed)
{
    std::ofstream file(fileName.c_str());

    // Did it open?
    if (!file.is_open())
    {
        std::cout << "Couldn't open file" << std::endl;
        return false;
    }

    file << "------- CAMERA SETUP ---------" << std::endl;
    file << "CameraStartX: " << cameraEyeStartPosition.x << std::endl;
    file << "CameraStartY: " << cameraEyeStartPosition.y << std::endl;
    file << "CameraStartZ: " << cameraEyeStartPosition.z << std::endl;
    file << "CameraSpeed: " << cameraSpeed << std::endl;
    file << "TargetX: " << cameraInitialTarget.x << std::endl;
    file << "TargetY: " << cameraInitialTarget.y << std::endl;
    file << "TargetZ: " << cameraInitialTarget.z << std::endl;
    file << "UpVectorX: " << upVector.x << std::endl;
    file << "UpVectorY: " << upVector.y << std::endl;
    file << "UpVectorZ: " << upVector.z << std::endl;

    file << "---------- MODELS -----------" << std::endl;
    for (int i = 0; i < allModels.size()-1; i++)
    {
        file << "ModelName: " << allModels.at(i)->modelName << std::endl;
        file << "isWireframe: " << allModels.at(i)->bIsWireframe << std::endl;
        file << "isColOverride: " << allModels.at(i)->bUseObjectDebugColour << std::endl;
        file << "ColourR: " << allModels.at(i)->objectDebugColourRGBA.r << std::endl;
        file << "ColourG: " << allModels.at(i)->objectDebugColourRGBA.g << std::endl;
        file << "ColourB: " << allModels.at(i)->objectDebugColourRGBA.b << std::endl;
        file << "PosX: " << allModels.at(i)->positionXYZ.x << std::endl;
        file << "PosY: " << allModels.at(i)->positionXYZ.y << std::endl;
        file << "PosZ: " << allModels.at(i)->positionXYZ.z << std::endl;
        file << "Scale: " << allModels.at(i)->scale << std::endl;
        file << "RotX: " << allModels.at(i)->orientationXYZ.x << std::endl;
        file << "RotY: " << allModels.at(i)->orientationXYZ.y << std::endl;
        file << "RotZ: " << allModels.at(i)->orientationXYZ.z << std::endl;
        file << "----------------------------" << std::endl;
    }


    return true;
}

// Write all modified lighting information to file
bool cFileLoader::UpdateLights(std::string fileName, cLightManager& allLights)
{
    std::ofstream file(fileName.c_str());

    // Did it open?
    if (!file.is_open())
    {
        std::cout << "Couldn't open file" << std::endl;
        return false;
    }

    std::stringstream ss;
    for (int i = 0; i < allLights.NUMBER_OF_LIGHTS; i++)
    {
        ss << "---------- LIGHT " << i << " -----------\n";
        file << ss.str();
        ss.str("");

        file << "PosX: " << std::fixed << std::setprecision(1) << allLights.theLights[i].position.x << std::endl;
        file << "PosY: " << allLights.theLights[i].position.y << std::endl;
        file << "PosZ: " << allLights.theLights[i].position.z << std::endl;
        file << "diffuseR: " << allLights.theLights[i].diffuse.r << std::endl;
        file << "diffuseG: " << allLights.theLights[i].diffuse.g << std::endl;
        file << "diffuseB: " << allLights.theLights[i].diffuse.b << std::endl;
        file << "diffuseA: " << allLights.theLights[i].diffuse.a << std::endl;
        file << "specularR: " << allLights.theLights[i].specular.r << std::endl;
        file << "specularG: " << allLights.theLights[i].specular.g << std::endl;
        file << "specularB: " << allLights.theLights[i].specular.b << std::endl;
        file << "specularPower: " << allLights.theLights[i].specular.w << std::endl;
        file << "attenConst: " << std::fixed << std::setprecision(2) << allLights.theLights[i].atten.x << std::endl;
        file << "attenLin: " << std::fixed << std::setprecision(7) << allLights.theLights[i].atten.y << std::endl;
        file << "attenQuad: " << allLights.theLights[i].atten.z << std::endl;
        file << "attenDistCut: " << allLights.theLights[i].atten.w << std::endl;
        file << "dirX: " << std::fixed << std::setprecision(1) << allLights.theLights[i].direction.x << std::endl;
        file << "dirY: " << allLights.theLights[i].direction.y << std::endl;
        file << "dirZ: " << allLights.theLights[i].direction.z << std::endl;
        file << "param1X: " << allLights.theLights[i].param1.x << std::endl;
        file << "param1Y: " << allLights.theLights[i].param1.y << std::endl;
        file << "param1Z: " << allLights.theLights[i].param1.z << std::endl;
        file << "param2X: " << allLights.theLights[i].param2.x << std::endl;
    }
}
