#pragma once

#include <string>
#include <vector>
#include "cVAOManager.h"
#include "cModel.h"
#include "cLightManager.h"

class cFileLoader
{
public:

	// Loads .ply model from file and formats it so the GPU understands
	bool LoadPLYFromFile(std::string fileName, sModelDrawInfo& drawInfo);

	// Loads the current scene description from a file and updates all models
	bool LoadCurrentScene(std::string fileName, std::vector<cModel*>& allModels);

	// Write all modified scene information to file (model positions, rotations etc.)
	bool UpdateCurrentScene(std::string fileName, std::vector<cModel*>& allModels, glm::vec3& cameraEyeStartPosition, glm::vec3& cameraInitialTarget, glm::vec3& upVector, float& cameraSpeed);

	// Load Initial Lighting Properties
	bool SetupLights(std::string fileName, cLightManager& allLights);

	// Write all modified lighting information to file
	bool UpdateLights(std::string fileName, cLightManager& allLights);

	// Load initial camera properties from scene description
	bool SetupCamera(std::string fileName, glm::vec3& cameraEyeStartPosition, glm::vec3& cameraInitialTarget, glm::vec3& upVector, float& cameraSpeed);
};