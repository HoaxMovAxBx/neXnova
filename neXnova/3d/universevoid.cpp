#include "universevoid.h"


UniverseVoid::UniverseVoid(Qt3DCore::QEntity *root):Qt3DCore::QEntity(root)
{

}

Qt3DRender::QEffect *UniverseVoid::generateVoidEffect()
{
    Qt3DRender::QEffect * effecto = new Qt3DRender::QEffect(this);
    Qt3DRender::QParameter*lightViewProjection = new Qt3DRender::QParameter("lightViewProjection",
                                                                            lightCameraLens->projectionMatrix());
    Qt3DRender::QParameter*lightPosition = new Qt3DRender::QParameter("lightPosition",
                                                                            QVector3D(0.0,0.0,0.0));
    Qt3DRender::QParameter*lightIntensity = new Qt3DRender::QParameter("lightIntensity",
                                                                            QVector3D(1.0,1.0,1.0));
    Qt3DRender::QParameter*shadowTextureEffecto = new Qt3DRender::QParameter("shadowTexture",
                                                                            shadowTexture);

    effecto->addParameter(lightViewProjection);
    effecto->addParameter(lightPosition);
    effecto->addParameter(lightIntensity);
    effecto->addParameter(shadowTextureEffecto);


    //!RenderPass for GLES and Mesa GL
    Qt3DRender::QFilterKey * ESFilterKey = new Qt3DRender::QFilterKey(this);
    ESFilterKey->setName(QStringLiteral("name"));
    ESFilterKey->setValue(QStringLiteral("ES2"));

    Qt3DRender::QFilterKey * GLFilterKey = new Qt3DRender::QFilterKey(this);
    GLFilterKey->setName(QStringLiteral("name"));
    GLFilterKey->setValue(QStringLiteral("Desktop"));

    Qt3DRender::QFilterKey * forwardFilterKey = new Qt3DRender::QFilterKey(this);
    forwardFilterKey->setName(QStringLiteral("pass"));
    forwardFilterKey->setValue(QStringLiteral("forward"));


    Qt3DRender::QRenderPass * glpass = new Qt3DRender::QRenderPass(this);
    Qt3DRender::QShaderProgram * glShader = new Qt3DRender::QShaderProgram(this);
    glShader->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/assets/shaders/gl3/shadowmap.vert")));
    glShader->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/assets/shaders/gl3/shadowmap.frag")));
    glpass->setShaderProgram(glShader);
    glpass->addFilterKey(forwardFilterKey);

    Qt3DRender::QRenderPass * espass = new Qt3DRender::QRenderPass(this);
    Qt3DRender::QShaderProgram * esShader = new Qt3DRender::QShaderProgram(this);
    esShader->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/assets/shaders/es2/planetD.vert")));
    esShader->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/assets/shaders/es2/planetD.vert")));
    espass->setShaderProgram(esShader);
    espass->addFilterKey(forwardFilterKey);

    Qt3DRender::QTechnique *gl3Technique = new Qt3DRender::QTechnique(this);

    gl3Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    gl3Technique->graphicsApiFilter()->setMajorVersion(3);
    gl3Technique->graphicsApiFilter()->setMinorVersion(2);
    gl3Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);

    gl3Technique->addFilterKey(GLFilterKey);
    gl3Technique->addRenderPass(glpass);

    Qt3DRender::QTechnique *es2Technique = new Qt3DRender::QTechnique(this);

    es2Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    es2Technique->graphicsApiFilter()->setMajorVersion(2);

    es2Technique->addFilterKey(ESFilterKey);
    es2Technique->addRenderPass(espass);

    Qt3DRender::QTechnique *gles2Technique = new Qt3DRender::QTechnique(this);

    gles2Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGLES);
    gles2Technique->graphicsApiFilter()->setMajorVersion(2);
    gles2Technique->graphicsApiFilter()->setMinorVersion(0);

    gles2Technique->addFilterKey(ESFilterKey);
    gles2Technique->addRenderPass(espass);

    effecto->addTechnique(gl3Technique);
    effecto->addTechnique(es2Technique);
    effecto->addTechnique(gles2Technique);

    return effecto;
}
