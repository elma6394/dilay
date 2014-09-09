#ifndef DILAY_PRIMITIVE_SPHERE
#define DILAY_PRIMITIVE_SPHERE

#include <glm/fwd.hpp>
#include <iosfwd>
#include "macro.hpp"

class PrimSphere {
  public:
    DECLARE_BIG4COPY (PrimSphere, const glm::vec3&, float);

    const glm::vec3& center () const;
    float            radius () const;

  private:
    IMPLEMENTATION
};

std::ostream& operator<<(std::ostream&, const PrimSphere&);

#endif
