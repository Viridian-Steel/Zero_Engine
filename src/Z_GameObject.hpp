#pragma once
#include "Z_Model.hpp"
#include <memory>


namespace ze {

	struct Transform2DComponent {
		glm::vec2 translation{};
		glm::vec2 scale{ 1.f, 1.f };
		float rotation;

		glm::mat2 mat2() { 
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMatrix{ {c,s}, {-s, c} };

			glm::mat2 scaleMat{ {scale.x, 0.f}, {0.f, scale.y} }; //this is a colunm. scale.x and 0 are colunm 1
			return rotMatrix * scaleMat; 
		}
	};

	class ZGameObject {
	public:
		using id_t = unsigned int;

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
		Transform2DComponent transform2d{};

	private:
		ZGameObject(id_t objId) :id{objId} {}

		id_t id;
	};
}
