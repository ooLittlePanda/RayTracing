#include "Renderer.h"
#include "Walnut/Random.h"

namespace Utils {
	static uint32_t ConvertToRGBA(const glm::vec4& color) {
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);
		return (a << 24) | (b << 16) | (g << 8) | r;
	}
}

void Renderer::OnResize(const uint32_t& width, const uint32_t& height) {

	if (m_FinalImage) {
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)return;
		m_FinalImage->Resize(width, height);
	}
	else {
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}
	delete[] m_ImageData;
	m_ImageData = new uint32_t[height * width];
}

void Renderer::Render(const Scene& scene, const Camera& camera) {
	m_ActiveScene = &scene;
	m_ActiveCamera = &camera;
	Ray ray;
	ray.Origin = camera.GetPosition();

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
			ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];

			glm::vec4 color = PerPixel();
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

std::shared_ptr<Walnut::Image> Renderer::GetFinalImage() {
	return m_FinalImage;
}

Renderer::HitPayload Renderer::TraceRay(const Scene& scene, const Ray& ray) {
	if (scene.Spheres.size() == 0)return glm::vec4(0, 0, 0, 1);

	const Sphere* closestSphere = nullptr;
	float hitDistance = FLT_MAX;
	const glm::vec3& rayDirection = ray.Direction;
	for (const Sphere& sphere : scene.Spheres) {
		glm::vec3 rayOrigin = ray.Origin - sphere.Position;
		// (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0 ， 二维直线与球相交
		// a为直线（光线）originPos
		// b为光线direction
		// r为球半径
		// t为光线与球的相交参数(hit distance)

		float p1 = glm::dot(rayDirection, rayDirection);
		float p2 = 2.0f * glm::dot(rayOrigin, rayDirection);
		float p3 = glm::dot(rayOrigin, rayOrigin) - sphere.radius * sphere.radius;

		//p2^2 - p1*p3
		//根为(-p2 +- sqrt(discriminant))/2p1
		float discriminant = p2 * p2 - 4.0f * p1 * p3;
		if (discriminant < 0.0f)continue;

		float t0 = (-p2 + glm::sqrt(discriminant)) / (2.0f * p1);
		float closedT = (-p2 - glm::sqrt(discriminant)) / (2.0f * p1);
		if (closedT < hitDistance) {
			hitDistance = closedT;
			closestSphere = &sphere;
		}
	}
	if (closestSphere == nullptr) return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::vec3 rayOrigin = ray.Origin - closestSphere->Position;

	glm::vec3 hitPoint = rayOrigin + rayDirection * hitDistance;
	glm::vec3 normal = glm::normalize(hitPoint);
	glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));

	float d = glm::max(glm::dot(normal, -lightDir), 0.0f);

	glm::vec3 sphereColor = closestSphere->Albedo;
	sphereColor *= d;
	return glm::vec4(sphereColor, 1.0f);
}

glm::vec4 Renderer::PerPixel(const Ray& ray)
{
	return HitPayload();
}

Renderer::HitPayload Renderer::ClosestHit(const Ray& ray, float hitDistance, uint32_t objectIndex)
{
	return HitPayload();
}

Renderer::HitPayload Renderer::Miss(const Ray& ray)
{
	return HitPayload();
}
