#include "CubeInst.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"

void CubeInst::Render()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    program.uniformMatrix("model", 1, GL_FALSE, glm::value_ptr(model));
    program.uniform3f("cube_color", color);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}