#pragma once

#include "Z_window.hpp"
#include "Z_GameObject.hpp"

namespace ze {
	class KeyBoardMovementController {
	public:
		struct keyMappings {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
		};

        void moveinPlaneXZ(GLFWwindow* window, float dt, ZGameObject& gameObject);

        keyMappings keys{};
        float moveSpeed{3.f};
        float lookSpeed{1.5f};

	};
}