#include "globals.h"
#include <sstream>

void printLightInfo();

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Press [ESC] to exit program
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    _controlDown = false;
    _altDown = false;
    _shiftDown = false;
    // Use bitwise mask to filter out just the shift
    if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
    {
        // Shift down and maybe other things, too
        _shiftDown = true;
    }
    if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
    {
        // Ctrl down and maybe other things, too
        _controlDown = true;
    }
    if ((mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
    {
        // Shift down and maybe other things, too
        _altDown = true;
    }

    // if JUST shift is down, move object
    if ((_shiftDown) && (!_controlDown) && (!_altDown))
    {
        if (key == GLFW_KEY_W || key == GLFW_KEY_UP) { _allModels.at(_selectedObject)->positionXYZ.z += _objectMovementSpeed; }
        if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) { _allModels.at(_selectedObject)->positionXYZ.x -= _objectMovementSpeed; }
        if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) { _allModels.at(_selectedObject)->positionXYZ.z -= _objectMovementSpeed; }
        if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) { _allModels.at(_selectedObject)->positionXYZ.x += _objectMovementSpeed; }
        if (key == GLFW_KEY_Q) { _allModels.at(_selectedObject)->positionXYZ.y -= _objectMovementSpeed; }
        if (key == GLFW_KEY_E) { _allModels.at(_selectedObject)->positionXYZ.y += _objectMovementSpeed; }

        std::stringstream ssTitle;
        ssTitle << "allModels[" << _selectedObject << "].positionxyz: "
            << _allModels.at(_selectedObject)->positionXYZ.x << ", "
            << _allModels.at(_selectedObject)->positionXYZ.y << ", "
            << _allModels.at(_selectedObject)->positionXYZ.z;
        _appTitleText = ssTitle.str();

        // Change selected model
        if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
        {
            _selectedObject++;
            if (_selectedObject >= _allModels.size()) { _selectedObject = _allModels.size() - 1; }
        }
        if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
        {
            _selectedObject--;
            if (_selectedObject >= _allModels.size()) { _selectedObject = 0; }
        }
    }

    // if JUST alt is down, move light
    if (!_shiftDown && !_controlDown && _altDown)
    {
        if (key == GLFW_KEY_W || key == GLFW_KEY_UP) { _allLights.theLights[_selectedLight].position.z += _LightMovementSpeed; }
        if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) { _allLights.theLights[_selectedLight].position.x -= _LightMovementSpeed; }
        if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) { _allLights.theLights[_selectedLight].position.z -= _LightMovementSpeed; }
        if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) { _allLights.theLights[_selectedLight].position.x += _LightMovementSpeed; }
        if (key == GLFW_KEY_Q) { _allLights.theLights[_selectedLight].position.y -= _LightMovementSpeed; }
        if (key == GLFW_KEY_E) { _allLights.theLights[_selectedLight].position.y += _LightMovementSpeed; }

        std::stringstream ssTitle;
        ssTitle << "_allLights[" << _selectedLight << "].positionxyz: "
            << _allLights.theLights[_selectedLight].position.x << ", "
            << _allLights.theLights[_selectedLight].position.y << ", "
            << _allLights.theLights[_selectedLight].position.z;
        _appTitleText = ssTitle.str();

        // Change selected light
        if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
        {
            _selectedLight++;
            if (_selectedLight >= cLightManager::NUMBER_OF_LIGHTS)
            {
                _selectedLight = cLightManager::NUMBER_OF_LIGHTS - 1;
            }
            std::stringstream ssTitle;
            ssTitle << "Selected Light = [" << _selectedLight << "]";
            _appTitleText = ssTitle.str();
        }
        if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
        {
            _selectedLight--;
            if (_selectedLight >= cLightManager::NUMBER_OF_LIGHTS)
            {
                _selectedLight = 0;
            }
            std::stringstream ssTitle;
            ssTitle << "Selected Light = [" << _selectedLight << "]";
            _appTitleText = ssTitle.str();
        }

        // Constant Attenuation
        if (key == GLFW_KEY_1)
        {
            _allLights.theLights[_selectedLight].atten.x *= 0.99f; // -1% less
            printLightInfo();
        }
        if (key == GLFW_KEY_2)
        {
            _allLights.theLights[_selectedLight].atten.x *= 1.01f; // +1% more
             // Is it at (or below) zero?
            if (_allLights.theLights[_selectedLight].atten.x <= 0.0f)
            {
                // Set it to some really small initial attenuation
                _allLights.theLights[_selectedLight].atten.x = 0.001f;
            }
            _allLights.theLights[_selectedLight].atten.x *= 1.01f; // +1% more
            printLightInfo();

        }
        // Linear Attenuation
        if (key == GLFW_KEY_3)
        {
            _allLights.theLights[_selectedLight].atten.y *= 0.99f; // -1% less
            printLightInfo();
        }
        if (key == GLFW_KEY_4)
        {
            _allLights.theLights[_selectedLight].atten.y *= 1.01f; // +1% more
            printLightInfo();
        }
        // Quadratic Attenuation
        if (key == GLFW_KEY_5)
        {
            _allLights.theLights[_selectedLight].atten.z *= 0.99f; // -1% less
            printLightInfo();
        }
        if (key == GLFW_KEY_6)
        {
            _allLights.theLights[_selectedLight].atten.z *= 1.01f; // +1% more
            printLightInfo();
        }

        // Change penumbra of spotlight
        if (key == GLFW_KEY_U)
        {
            _allLights.theLights[_selectedLight].param1.y -= 0.5f; // Inner cone angle
            printLightInfo();
        }
        if (key == GLFW_KEY_I)
        {
            _allLights.theLights[_selectedLight].param1.y += 0.5f; // Inner cone angle
            printLightInfo();
        }
        if (key == GLFW_KEY_O)
        {
            _allLights.theLights[_selectedLight].param1.z -= 0.5f; // Outer cone angle
            printLightInfo();
        }
        if (key == GLFW_KEY_P)
        {
            _allLights.theLights[_selectedLight].param1.z += 0.5f; // Outer cone angle
            printLightInfo();
        }
    }



    // Turn debug sphere on and off
    if (key == GLFW_KEY_PAGE_UP)
    {
        _showDebugSphere = true;
    }
    if (key == GLFW_KEY_PAGE_DOWN)
    {
        _showDebugSphere = false;
    }



    // Turn light off and on
    if (key == GLFW_KEY_KP_MULTIPLY)
    {
        _allLights.theLights[_selectedLight].param2.x = 1.0f; //ON
        printLightInfo();
    }
    if (key == GLFW_KEY_KP_DIVIDE)
    {
        _allLights.theLights[_selectedLight].param2.x = 0.0f; //OFF
        printLightInfo();
    }
}


// If a GLFW function fails, get error callback
void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// Setup print information about currently selected light
void printLightInfo()
{
    std::stringstream ssTitle;
    ssTitle << "attenuation (C, L, Q): "
        << _allLights.theLights[_selectedLight].atten.x << ", "        // Const
        << _allLights.theLights[_selectedLight].atten.y << ", "        // Linear
        << _allLights.theLights[_selectedLight].atten.z << "  "        // Quadratic
        << (_allLights.theLights[_selectedLight].param2.x > 0.0f ? " is on" : " is off");
    _appTitleText = ssTitle.str();
}