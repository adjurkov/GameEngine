#include "cLightHelper.h"
#include "cLightManager.h"
#include "GLCommon.h"
#include <sstream>
#include <string>

cLightManager::cLightManager(){}

cLightHelper cLightManager::lightHelper;

sLight::sLight()
{
	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	this->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);     // White light
	this->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);    // White specular highlight
	this->atten = glm::vec4(0.0f, 0.1f, 0.01f, 100000.0f); // x = constant, y = linear, z = quadratic, w = DistanceCutOff
	this->direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);  
	this->param1.x = 0;		// x = lightType (0 = pointlight, 1 = spotlight, 2 = directional light)
	this->param1.y = 0.0f;  // y = inner angle
	this->param1.z = 0.0f;  // z = outer angle
	this->param1.w = 1.0f;	// not used, so set to 1.0f

	this->param2 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // x = 0 for off, 1 for on

	// Here's the uniform locations of the light values in the shader
	// Settting these to -1 which is the "not found" or unknown uniform location
	this->position_uniform_location = -1;
	this->diffuse_uniform_location = -1;
	this->specular_uniform_location = -1;
	this->atten_uniform_location = -1;
	this->direction_uniform_location = -1;
	this->param1_uniform_location = -1;
	this->param2_uniform_location = -1;
}

// This sets up the initial uniform locations from the shader
void cLightManager::SetUpUniformLocations(unsigned int shaderProgram)
{
	for (unsigned int i = 0; i != NUMBER_OF_LIGHTS; i++)
	{
		std::stringstream ssLight;
		ssLight << "theLights[" << i << "].";

		this->theLights[i].position_uniform_location = glGetUniformLocation(shaderProgram, (ssLight.str() + "position").c_str());
		this->theLights[i].diffuse_uniform_location = glGetUniformLocation(shaderProgram, (ssLight.str() + "diffuse").c_str());
		this->theLights[i].specular_uniform_location = glGetUniformLocation(shaderProgram, (ssLight.str() + "specular").c_str());
		this->theLights[i].atten_uniform_location = glGetUniformLocation(shaderProgram, (ssLight.str() + "atten").c_str());
		this->theLights[i].direction_uniform_location = glGetUniformLocation(shaderProgram, (ssLight.str() + "direction").c_str());
		this->theLights[i].param1_uniform_location = glGetUniformLocation(shaderProgram, (ssLight.str() + "param1").c_str());
		this->theLights[i].param2_uniform_location = glGetUniformLocation(shaderProgram, (ssLight.str() + "param2").c_str());
	}
	return;
}

// Copies the values from the array into the shader
void cLightManager::CopyLightInfoToShader(void)
{
	for (unsigned int i = 0; i != NUMBER_OF_LIGHTS; i++)
	{
		glUniform4f(this->theLights[i].position_uniform_location,
			this->theLights[i].position.x,
			this->theLights[i].position.y,
			this->theLights[i].position.z,
			this->theLights[i].position.w);

		glUniform4f(this->theLights[i].diffuse_uniform_location,
			this->theLights[i].diffuse.x,
			this->theLights[i].diffuse.y,
			this->theLights[i].diffuse.z,
			this->theLights[i].diffuse.w);

		glUniform4f(this->theLights[i].specular_uniform_location,
			this->theLights[i].specular.x,
			this->theLights[i].specular.y,
			this->theLights[i].specular.z,
			this->theLights[i].specular.w);

		glUniform4f(this->theLights[i].atten_uniform_location,
			this->theLights[i].atten.x,
			this->theLights[i].atten.y,
			this->theLights[i].atten.z,
			this->theLights[i].atten.w);

		glUniform4f(this->theLights[i].direction_uniform_location,
			this->theLights[i].direction.x,
			this->theLights[i].direction.y,
			this->theLights[i].direction.z,
			this->theLights[i].direction.w);

		glUniform4f(this->theLights[i].param1_uniform_location,
			this->theLights[i].param1.x,
			this->theLights[i].param1.y,
			this->theLights[i].param1.z,
			this->theLights[i].param1.w);

		glUniform4f(this->theLights[i].param2_uniform_location,
			this->theLights[i].param2.x,
			this->theLights[i].param2.y,
			this->theLights[i].param2.z,
			this->theLights[i].param2.w);
	}
	return;
}

//void cLightManager::TurnOnLight(unsigned int lightNumber)
//{
//	if (lightNumber < cLightManager::NUMBER_OF_LIGHTS)
//	{
//		this->theLights[lightNumber].param2.x = 1.0f;
//	}
//	return;
//}
//
//void cLightManager::TurnOffLight(unsigned int lightNumber)
//{
//	if (lightNumber < cLightManager::NUMBER_OF_LIGHTS)
//	{
//		this->theLights[lightNumber].param2.x = 0.0f;
//	}
//	return;
//}