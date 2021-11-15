#include "globals.h"
#include <sstream>

// Handle async IO (keyboard, joystick, mouse)
void asyncIO(GLFWwindow* _window)
{
    // SELF NOTE: this doesnt pass the modifier so you will have to query this with a GFLW call
    // SELF NOTE: if you are using a bunch of combos, maybe make a set
    // of functions like "isShiftDownByItself()" and "isShiftDown()", etc

    // Basic camera control if none of the control keys are pressed
    if ((!_shiftDown) && (!_controlDown) && (!_altDown))
    {
        // If [W] or [UP] pressed, move camera forward
        if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS) { _cameraEye.z += _cameraSpeed; }
        // If [A] or [LEFT] pressed, move camera left
        if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS) { _cameraEye.x -= _cameraSpeed; }
        // If [S] or [DOWN] pressed, move camera backwards
        if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS) { _cameraEye.z -= _cameraSpeed; }
        // If [D] or [RIGHT] pressed, move camera right
        if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS) { _cameraEye.x += _cameraSpeed; }
        // If [Q] is pressed, pan camera down
        if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS) { _cameraEye.y -= _cameraSpeed; }
        // If [E] is pressed, pan camera up
        if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS) { _cameraEye.y += _cameraSpeed; }

        std::stringstream ssTitle;
        ssTitle << "Camera: " << _cameraEye.x << ", " << _cameraEye.y << ", " << _cameraEye.z;
        _appTitleText = ssTitle.str();
    }
    return;
}