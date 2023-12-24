#include "Renderer.h"

void Renderer::Render() {

}

void Renderer::OnResize(const uint32_t& width, const uint32_t& height) {

	if (m_Image) {
		if (m_Image->GetWidth() == width && m_Image->GetHeight() == height)return;
		m_Image->Resize(width, height);
	}
	else {
		m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}
	delete[] m_ImageData;
	m_ImageData = new uint32_t[height * width];
}