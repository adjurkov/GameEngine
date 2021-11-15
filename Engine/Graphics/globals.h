#pragma once
#include "cModel.h"
#include "cVAOManager.h"
#include "cShaderManager.h"
#include "cFileLoader.h"
#include "cLightManager.h"
#include "cLightHelper.h"
#include "GLCommon.h"

//---------------------------- WINDOW SETUP VARIABLES ------------------------------

// Window Properties
extern GLuint _windowWidth;
extern GLuint _windowHeight;
extern GLFWwindow* _window;
extern std::string _appName;

// Framebuffer Size
extern float _ratio;
extern int _framebufferWidth;
extern int _framebufferHeight;

// Display changes in the title bar
extern std::string _appTitleText;

//-------------------------- GRAPHICS SETUP VARIABLES ------------------------------

extern GLuint _shaderProgram;	   // Default to not using a shader
extern glm::mat4 _worldMatrix;        // Model/World Transform
extern glm::mat4 _projectionMatrix;   // View Transform
extern glm::mat4 _viewMatrix;		   // Projection Transform
extern glm::mat4 _mvp;

extern GLint _mvp_location;
extern GLint World_Matrix_Location;
extern GLint View_Matrix_Location;
extern GLint Projection_Matrix_Location;
extern GLint InverseTranspose_Location;

// Managers
extern cVAOManager* _VAOManager;
extern cShaderManager* _shaderManager;

//-------------------------------- SCENE OBJECTS -----------------------------------

// Files that store information 
extern std::string _sceneDescriptionFile; // models, camera
extern std::string _lightDescriptionFile; // lights

// Managers
extern cFileLoader _fileLoader;
extern cLightManager _allLights;
extern cLightHelper _lightHelper;

// Debug sphere
extern cModel* _pDebugSphere;
extern bool _showDebugSphere;

// Models
extern std::vector<cModel*> _allModels;
extern unsigned int _selectedObject;
extern float _objectMovementSpeed;

// Lights
extern unsigned int _selectedLight;
extern float _LightMovementSpeed;

// Camera Default
extern glm::vec3 _cameraEye;
extern glm::vec3 _cameraTarget;
extern glm::vec3 _upVector;
extern float _cameraSpeed;

//-------------------------------- KEYBOARD INPUT ----------------------------------

extern bool _shiftDown;
extern bool _controlDown;
extern bool _altDown;
void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void glfw_error_callback(int error, const char* description);








