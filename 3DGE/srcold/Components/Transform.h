#ifndef _COMPONENTS_TRANSFORM_
#define _COMPONENTS_TRANSFORM_

#include <glm/glm.hpp>

namespace GE {

class Transform : public Component {
public: // functions
    Transform();
    Transform(const glm::vec3& pos);
    Transform(const glm::vec3& pos, const glm::vec3& euler);
    Transform(const glm::vec3& pos, const glm::vec3& euler, const glm::vec3& scale);

    ~Transform();

    glm::mat4 Model() const;

    // TODO move these somwhere nicer
    //glm::mat4 MVP() const;
    //glm::mat4 ModelView() const;

public: // variables
    glm::vec3 position;
    glm::vec3 eulerAngles;
    glm::vec3 scale;
};

}

#endif
