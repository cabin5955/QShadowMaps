#ifndef SHADOWMAPWIDGET_H
#define SHADOWMAPWIDGET_H

#include <QWidget>
#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QKeyEvent>
#include "shader.h"
#include "simpledraw.h"

class ShadowMapWidget: public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit ShadowMapWidget(QWidget *parent = nullptr);
    ~ShadowMapWidget();

signals:

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *keyEvent) override;

private:
    unsigned int depthMapFBO;
    unsigned int depthMap;

    float aspectRatio;
    glm::vec3 movement;
    float lastX;
    float lastY;
    bool firstMouse = true;
    float deltaTime;
    float lastFrameTime;

    QOpenGLTexture *metalTex;
    QOpenGLTexture *containerTex;
    QOpenGLTexture *marbleTex;
    SimpleDraw *simpleDraw;

    void initDepthMap();
    void renderDepthMap(glm::mat4 lightSpaceMatrix);
    void renderScreen(glm::mat4 lightSpaceMatrix);
    void renderScene(const Shader &shader);
};

#endif // SHADOWMAPWIDGET_H
