#ifndef SIMPLEDRAW_H
#define SIMPLEDRAW_H

#include <GL/glew.h>
#include "shader.h"

class SimpleDraw
{
public:
    SimpleDraw();
    ~SimpleDraw();

    void renderPlane(const Shader &shader, glm::mat4 model);
    void renderCube(const Shader &shader, glm::mat4 model);
    void renderDebug(int screenWidth,int screenHeight, unsigned int texID);

private:
    unsigned int debugQuadVBO = 0, debugQuadVAO = 0;
    unsigned int planeVAO = 0, planeVBO = 0;
    unsigned int cubeVAO = 0, cubeVBO = 0;
};

#endif // SIMPLEDRAW_H
