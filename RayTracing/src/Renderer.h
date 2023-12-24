#pragma once
#include "Walnut/Image.h"
#include <memory>
#include <glm/glm.hpp>

class Renderer {
public:
	Renderer() = default;
	void OnResize(const uint32_t& width, const uint32_t& height);
	void Render();
	std::shared_ptr<Walnut::Image> GetFinalImage();
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
	glm::vec4 PerPixel(glm::vec2 coord);
};