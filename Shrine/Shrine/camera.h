#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
	UP,
	DOWN
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
	float LastToggleLight = 0;
	bool IsLightOn = false;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

	bool inLegalArea(glm::vec3 pos, int rockState) {
		if (pos.x >= -1.15 && pos.x <= 1.15 && pos.z >= -1.0 && pos.z <= 1.3)
			return false;
		if (pos.x >= 1.0 && pos.x <= 4.2 && pos.z >= 5.7 && pos.z <= 8.7)
			return false;
		if (pos.x >= -4.2 && pos.x <= -0.65 && pos.z >= 5.7 && pos.z <= 8.7)
			return false;
		if (pos.x >= -4.2 && pos.x <= -0.65 && pos.z >= -8.4 && pos.z <= -5.7)
			return false;
		if (pos.x >= 1.0 && pos.x <= 4.2 && pos.z >= -8.4 && pos.z <= -5.7)
			return false;
		if (pos.x >= -4.2 && pos.x <= -2.75 && pos.z >= -5.7 && pos.z <= 5.7)
			return false;
		if (pos.x >= 2.15 && pos.x <= 4.2 && pos.z >= -5.7 && pos.z <= 5.7)
			return false;
		if ((rockState == 0 || rockState == 2) && pos.x >= -0.65 && pos.x <= 1.0 && pos.z >= 4.5 && pos.z <= 8.0)
			return false;
		if ((rockState == 0 || rockState == 1) && pos.x >= -1.3 && pos.x <= 1.35 && pos.z >= -7.5 && pos.z <= -4.0)
			return false;
		return true;
	}

	float updatePosY(glm::vec3 pos) {
		if (pos.x >= -2.75 && pos.x <= 2.15 && pos.z >= -5.7 && pos.z <= 5.7)
			return 2.0f;
		if (pos.x >= -0.65 && pos.x <= 1.0 && pos.z >= -8.4 && pos.z <= -5.7)
			return (float(pos.z - (-8.4)) / float((-5.7) - (-8.4))) + 1.0;
		if (pos.x >= -0.65 && pos.x <= 1.0 && pos.z >= 5.7 && pos.z <= 8.7)
			return 2.0 - (float(pos.z - 5.7) / float(8.7 - 5.7));
		return 1.0f;
	}

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime, int rockState)
    {
		const glm::vec3 oldPos = Position;
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
		if (direction == UP)
			Position += Up * velocity;
		if (direction == DOWN)
			Position -= Up * velocity;

		if (inLegalArea(oldPos, rockState) && !inLegalArea(Position, rockState))
			Position = oldPos;
		Position.y = updatePosY(Position);
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif