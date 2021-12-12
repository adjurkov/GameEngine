#include "globals.h"

//---------------------------- WINDOW SETUP VARIABLES ------------------------------

// Window Properties
GLuint _windowWidth = 1200;
GLuint _windowHeight = 640;
GLFWwindow* _window = NULL;
std::string _appName = "";

// Framebuffer Size
float _ratio = 0;
int _framebufferWidth = 0;
int _framebufferHeight = 0;

// Display changes in the title bar
std::string _appTitleText = "";

//-------------------------- GRAPHICS SETUP VARIABLES ------------------------------

GLuint _shaderProgram = 0;	   // Default to not using a shader
glm::mat4 _worldMatrix;        // Model/World Transform
glm::mat4 _projectionMatrix;   // View Transform
glm::mat4 _viewMatrix;		   // Projection Transform
glm::mat4 _mvp;

// Managers
cVAOManager* _VAOManager = NULL;
cShaderManager * _shaderManager = NULL;
cBasicTextureManager* _textureManager = NULL;

GLint _mvp_location = -1;
GLint World_Matrix_Location = -1;
GLint View_Matrix_Location = -1;
GLint Projection_Matrix_Location = -1;
GLint InverseTranspose_Location = -1;

//-------------------------------- SCENE OBJECTS -----------------------------------

// Files that store information 
std::string _sceneDescriptionFile = "assets/models/currentSceneDescription.txt"; // models, camera
std::string _lightDescriptionFile = "assets/models/lightDescription.txt";        // lights

// Managers
cFileLoader _fileLoader;
cLightManager _allLights;
cLightHelper _lightHelper;

// Debug sphere
cModel* _pDebugSphere = NULL;
bool _showDebugSphere = true;

// Models
std::vector<cModel*> _allModels;
unsigned int _selectedObject = 1;
float _objectMovementSpeed = 1.0f;

// Lights
unsigned int _selectedLight = 0;
float _LightMovementSpeed = 1.0f;

// Camera Default
glm::vec3 _cameraEye = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 _cameraTarget = glm::vec3(0.0f, 0.f, 0.f);
glm::vec3 _upVector = glm::vec3(0.0f, 1.0f, 0.0f);
float _cameraSpeed = 4.0f;

//-------------------------------- KEYBOARD INPUT ----------------------------------

bool _shiftDown = false;
bool _controlDown = false;
bool _altDown = false;







