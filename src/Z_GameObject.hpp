#pragma once
#include "Z_Model.hpp"
#include <memory>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

namespace ze {

	struct TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation;
		
		// Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
		// Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
		// https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};

	class ZGameObject {
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, ZGameObject>;

		static ZGameObject createGameObject() {
			static id_t currentId = 0;
			return ZGameObject(currentId++);
		}

		ZGameObject(const ZGameObject&) = delete;
		ZGameObject& operator=(const ZGameObject&) = delete;
		ZGameObject(ZGameObject&&) = default;
		ZGameObject& operator=(ZGameObject&&) = default;

		id_t getId() { return id; }

		std::shared_ptr<ZModel> model{};
		glm::vec3 color{};
		TransformComponent transform{};

	private:
		ZGameObject(id_t objId) :id{objId} {}

		id_t id;
	};
}
