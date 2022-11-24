#include "shadowmapwidget.h"
#include "resourcemanager.h"
#include "camera.h"
#include <QTime>

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
Camera camera(glm::vec3(0.0f, 3.0f, 10.0f), {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f});

ShadowMapWidget::ShadowMapWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    aspectRatio = (float) 800 / 600.0f;
    setFocusPolicy(Qt::StrongFocus);
}

ShadowMapWidget::~ShadowMapWidget()
{
     delete simpleDraw;
}

void ShadowMapWidget::initializeGL()
{
    glewInit();
    glEnable(GL_DEPTH_TEST);

    ResourceManager::LoadShader(":/shaders/shaders/depth.vs",":/shaders/shaders/depth.fs",nullptr, "depth");
    ResourceManager::LoadShader(":/shaders/shaders/debug.vs",":/shaders/shaders/debug.fs",nullptr, "debug");
    ResourceManager::LoadShader(":/shaders/shaders/light.vs",":/shaders/shaders/light.fs",nullptr, "light");

    metalTex = new QOpenGLTexture(QImage(":/textures/textures/metal.png").mirrored(false,true));
    marbleTex = new QOpenGLTexture(QImage(":/textures/textures/marble.jpg").mirrored(false,true));
    containerTex = new QOpenGLTexture(QImage(":/textures/textures/container.jpg").mirrored(false,true));

    Shader &lightShader = ResourceManager::GetShader("light");
    lightShader.use();
    lightShader.setInt("diffuseTexture", 0);
    lightShader.setInt("shadowMap", 1);

    Shader &debugDepthQuad = ResourceManager::GetShader("debug");
    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);

    simpleDraw = new SimpleDraw();
    initDepthMap();
}

void ShadowMapWidget::initDepthMap()
{
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
}

void ShadowMapWidget::paintGL()
{
    float time = QTime::currentTime().msecsSinceStartOfDay()/1000.0f;
    deltaTime = time - lastFrameTime;
    float movAmt = 10.0f * deltaTime;//camera.MovementSpeed * deltaTime;
    movement.y = 0.0f;
    if(glm::length(movement) > 0){
        movement = glm::normalize(movement);
    }
    //here should be collision check
    if(glm::length(movement) > 0){
        camera.Position += movement * movAmt;
    }
    movement = {0,0,0};

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 1. render depth of scene to texture (from light's perspective)
    // --------------------------------------------------------------
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    renderDepthMap(lightSpaceMatrix);
    renderScreen(lightSpaceMatrix);
    update();
    //simpleDraw->renderDebug(width()*devicePixelRatio(), height()*devicePixelRatio(), depthMap);
}

void ShadowMapWidget::resizeGL(int w, int h)
{
    aspectRatio = (float)w / h;
}

void ShadowMapWidget::renderDepthMap(glm::mat4 lightSpaceMatrix)
{
    // render scene from light's point of view
    Shader &simpleDepthShader = ResourceManager::GetShader("depth");
    simpleDepthShader.use();
    simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, containerTex->textureId());
        renderScene(simpleDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
}

void ShadowMapWidget::renderScreen(glm::mat4 lightSpaceMatrix)
{
    // 2. render scene as normal using the generated depth/shadow map
    // --------------------------------------------------------------
    const int devicePixelRatio = this->devicePixelRatio();
    const int w = this->width() * devicePixelRatio;
    const int h = this->height() * devicePixelRatio;
    glViewport(0, 0, w, h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Shader &lightShader = ResourceManager::GetShader("light");
    lightShader.use();
    glm::mat4 projection = glm::perspective(45.0f, aspectRatio, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    lightShader.setMat4("projection", projection);
    lightShader.setMat4("view", view);

    // set light uniforms
    lightShader.setVec3("viewPos", camera.Position);
    lightShader.setVec3("lightPos", lightPos);
    lightShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, containerTex->textureId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    renderScene(lightShader);
}

// renders the 3D scene
// --------------------
void ShadowMapWidget::renderScene(const Shader &shader)
{
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    simpleDraw->renderPlane(shader, model);
    // cubes
    model = glm::mat4(1.0f);
    float radius = 3.0f;
    float time = QTime::currentTime().msecsSinceStartOfDay()/1000.0f;
    float px = sin(time) * radius;
    float pz = cos(time) * radius;
    model = glm::translate(model, glm::vec3(px, 2.0f, pz));
    model = glm::scale(model, glm::vec3(0.5f));
    simpleDraw->renderCube(shader, model);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(1.5f));
    simpleDraw->renderCube(shader, model);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    simpleDraw->renderCube(shader, model);
}

void ShadowMapWidget::keyPressEvent(QKeyEvent *keyEvent){
    movement = {0,0,0};
    switch (keyEvent->key()){
        case Qt::Key_A:
            movement += camera.Right;
            break;
        case Qt::Key_D:
            movement -= camera.Right;
            break;
        case Qt::Key_W:
            movement += camera.Front;
            break;
        case Qt::Key_S:
            movement -= camera.Front;
            break;
        case Qt::Key_Space:
            break;
    }
    update();
}

void ShadowMapWidget::mousePressEvent(QMouseEvent *event)
{
    firstMouse = true;
}

void ShadowMapWidget::mouseMoveEvent(QMouseEvent *event)
{
    float xpos = event->position().x();
    float ypos = event->position().y();

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
    update();
}

