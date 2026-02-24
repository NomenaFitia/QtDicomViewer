// Documentation TODO

#pragma once
#include <cmath>
#include <cctype>
#include <glm/vec3.hpp>

namespace math {

static inline glm::vec3 cross(const glm::vec3& a, const glm::vec3& b) { return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; }
static inline float dot(const glm::vec3& a, const glm::vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline float norm(const glm::vec3& v) { return std::sqrt(dot(v, v)); }
static inline glm::vec3 normalize(const glm::vec3& v) { float n = norm(v); return (n > 0.f) ? glm::vec3{ v.x / n, v.y / n, v.z / n } : glm::vec3{ 0,0,1 }; }
static inline void setDirection(float M[9], const glm::vec3& r, const glm::vec3& c, const glm::vec3& n) {
    M[0] = r.x; M[1] = r.y; M[2] = r.z; M[3] = c.x; M[4] = c.y; M[5] = c.z; M[6] = n.x; M[7] = n.y; M[8] = n.z;
}

} // namespace math
