#include "skybox.h"

skybox::skybox(Qt3DCore::QEntity *root) : Qt3DCore::QEntity(root)
{
    mesh = new Qt3DRender::QMesh(this);
    mesh->setSource(QUrl("qrc:/assets/meshes/starfield.obj"));


    transform =  new Qt3DCore::QTransform(this);
    transform->setTranslation(QVector3D(0,0,0));
    transform->setScale(850000);


    skymaterial = new Qt3DExtras::QDiffuseMapMaterial(this);
    skymaterial->setAmbient(QColor(0,0,0));
    skymaterial->setSpecular(QColor(0,0,0));
    skymaterial->setShininess(1000000.0);

    Qt3DRender::QTextureWrapMode wmodel;
    wmodel.setX(Qt3DRender::QTextureWrapMode::Repeat);
    wmodel.setY(Qt3DRender::QTextureWrapMode::Repeat);

    Qt3DRender::QTextureImage * textImage = new Qt3DRender::QTextureImage;
    textImage->setSource(QUrl("qrc:/assets/images/solarsystemscope/galaxy_starfield.jpg"));

    Qt3DRender::QTexture2D * texture2d = new Qt3DRender::QTexture2D(skymaterial);
    texture2d->setMinificationFilter(Qt3DRender::QTexture2D::LinearMipMapLinear);
    texture2d->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
    texture2d->setWrapMode(wmodel);
    texture2d->setGenerateMipMaps(true);
    texture2d->setMaximumAnisotropy(16.0);
    texture2d->addTextureImage(textImage);

    skymaterial->setDiffuse(texture2d);

    //effect = this->generateEffects();
    //skymaterial->setEffect(effect);

    this->addComponent(mesh);
    this->addComponent(skymaterial);
    this->addComponent(transform);

    setObjectName("Skybox");

}

Qt3DRender::QEffect *skybox::generateEffects()
{
    Qt3DRender::QEffect * effecto = new Qt3DRender::QEffect(this);


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
    glShader->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/assets/shaders/gl3/planetD.vert")));
    glShader->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/assets/shaders/gl3/planetD.frag")));
    glpass->setShaderProgram(glShader);
    glpass->addFilterKey(forwardFilterKey);

    Qt3DRender::QRenderPass * espass = new Qt3DRender::QRenderPass(this);
    Qt3DRender::QShaderProgram * esShader = new Qt3DRender::QShaderProgram(this);
    esShader->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/assets/shaders/es2/planetD.vert")));
    esShader->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/assets/shaders/es2/planetD.frag")));
    espass->setShaderProgram(esShader);
    espass->addFilterKey(forwardFilterKey);

    Qt3DRender::QTechnique *gl3Technique = new Qt3DRender::QTechnique(this);

    gl3Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    gl3Technique->graphicsApiFilter()->setMajorVersion(3);
    gl3Technique->graphicsApiFilter()->setMinorVersion(0);
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
