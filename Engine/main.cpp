// Ana Djurkovic, 2021
// ana_djurkovic@hotmail.com

#include "GLCommon.h"  // GLFW, glad, glm
#include "globals.h"   // Graphics-related globals

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>   
#include <string>

//------------------------------ FUNCTION PROTOTYPES ----------------------------------

// Functions implemented inside "Draw.cpp"
void DrawObject(cModel* currentModel, glm::mat4 worldMatrix, GLint World_Matrix_Location, GLint InverseTranspose_Location);
void DrawDebugSphere(GLint World_Matrix_Location, glm::mat4 worldMatrix, GLint InverseTranspose_Location, cLightManager _allLights, int _selectedLight);

// Functions implemented inside "SetupGraphics.cpp"
void setupShader();
void setupDebugSphere();
void setupLoadModels();
void setupTextures();

// Update models, lights, window text, camera
void updateVisuals();

// Handle async input
void asyncIO(GLFWwindow* _window);

bool init();
bool initGLFW();
bool initGL();
void shutdown();



int main(int argc, char** argv)
{
    // Initialize all dependencies
    if (!init())
    {
        fprintf(stderr, "Unable to initialize app");
        return -1;
    }

    // Setup everything needed for graphics
    setupShader();
    setupDebugSphere();
    setupLoadModels();
    setupTextures();

    // Loading textures
    _textureManager->SetBasePath("assets/textures");

    if (_textureManager->Create2DTextureFromBMPFile("woodTexture.bmp", true))
    {
        std::cout << "Loaded the texture" << std::endl;
    }
    else
    {
        std::cout << "Didn't load the texture" << std::endl;
    }


    
    // Load initial scene properties from files
    _fileLoader.SetupCamera(_sceneDescriptionFile, _cameraEye, _cameraTarget, _upVector, _cameraSpeed);
    _fileLoader.LoadCurrentScene(_sceneDescriptionFile, _allModels);
    _fileLoader.SetupLights(_lightDescriptionFile, _allLights);

    

    // Main Loop
    while (!glfwWindowShouldClose(_window))
    {
        // Draw scene
        updateVisuals();

        // Copy the light information into the shader to draw the scene
        _allLights.CopyLightInfoToShader();
        
        glfwSwapBuffers(_window);
        glfwPollEvents();

        // Handle keyboard, mouse etc
        asyncIO(_window);
    }
	shutdown();
	return 0;
}

// Initialize Dependencies 
bool init()
{
    if (!initGLFW())
        return false;

    if (!initGL())
        return false;
}

// Initialize GLFW
bool initGLFW()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Unable to initialize GLFW");
        return false;
    }

    // Set error callbacks
    glfwSetErrorCallback(glfw_error_callback);

    // Setting Minimum OpenGL Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create Window
    _window = glfwCreateWindow(_windowWidth, _windowHeight, _appName.c_str(), NULL, NULL);
    if (!_window)
    {
        fprintf(stderr, "Unable to create window");
        return false;
    }

    // Set keyboard callback
    glfwSetKeyCallback(_window, glfw_key_callback);

    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);

    return true;
}

// Initialize Glad
bool initGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Unable to initialize glad");
        return false;
    }

    glEnable(GL_DEPTH);        // Enable Depth Buffer
    glEnable(GL_DEPTH_TEST);   // Is pixel already closer

    return true;
}



void updateVisuals()
{
    // Framebuffer Size
    glfwGetFramebufferSize(_window, &_framebufferWidth, &_framebufferHeight);
    _ratio = _framebufferWidth / (float)_framebufferHeight;

    glViewport(0, 0, _framebufferWidth, _framebufferHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update title text
    glfwSetWindowTitle(_window, _appTitleText.c_str());

    // Projection matrix
    _projectionMatrix = glm::perspective(0.6f, _ratio, 0.1f, 1000.0f);

    // View matrix
    _viewMatrix = glm::mat4(1.0f);
    _viewMatrix = glm::lookAt(_cameraEye, _cameraTarget, _upVector);

    glUniformMatrix4fv(View_Matrix_Location, 1, GL_FALSE, glm::value_ptr(_viewMatrix));
    glUniformMatrix4fv(Projection_Matrix_Location, 1, GL_FALSE, glm::value_ptr(_projectionMatrix));

    // Update Debug Sphere position
    _pDebugSphere->positionXYZ = _allLights.theLights[_selectedLight].position;

    // Update Scene (save/write to file)
    _fileLoader.UpdateCurrentScene(_sceneDescriptionFile, _allModels, _cameraEye, _cameraTarget, _upVector, _cameraSpeed);
    _fileLoader.UpdateLights(_lightDescriptionFile, _allLights);
    

    // TEMPORARY - setting alpha transparency for this model
    _allModels.at(2)->alphaTransparency = 0.4f;


    // Draw all the models
    for (unsigned int i = 0; i != _allModels.size(); i++)
    {
        cModel* currentModel = _allModels.at(i);

        // Set worldMatrix to identity
        _worldMatrix = glm::mat4(1.0f);

        DrawObject(currentModel, _worldMatrix, World_Matrix_Location, InverseTranspose_Location);
    }

    // Draw a debug sphere for light
    DrawDebugSphere(World_Matrix_Location, _worldMatrix, InverseTranspose_Location, _allLights, _selectedLight);

}

void shutdown()
{
    delete _VAOManager;
    delete _shaderManager;
    delete _pDebugSphere;
    delete _textureManager;

	glfwDestroyWindow(_window); 
	glfwTerminate();
	exit(EXIT_SUCCESS);
}