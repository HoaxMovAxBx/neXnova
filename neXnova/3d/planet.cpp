#include "planet.h"

planet::planet(Qt3DCore::QEntity *root) : Qt3DCore::QEntity(root)
{
   Scale = 1.0;
   MrPickles = new Qt3DRender::QObjectPicker(this);
   MrPickles->setDragEnabled(false);
   MrPickles->setHoverEnabled(false);
   connect(MrPickles,&Qt3DRender::QObjectPicker::clicked,this,&planet::clicked);
   connect(MrPickles,&Qt3DRender::QObjectPicker::clicked,this,&planet::picked);
   addComponent(MrPickles);
   Rotational = nullptr;
}

planet *planet::generateFromJson(QJsonObject data, Qt3DCore::QEntity *parent)
{
    planet * earth = new planet(parent);

    earth->setEid(data["id"].toString());
    earth->setName(data["name"].toString());
    earth->setType(data["type"].toInt());
    earth->setCampus(data["campus"].toInt());
    earth->setTemperature(data["temperature"].toDouble());
    earth->setStructures(data["structures"].toObject());

    Qt3DExtras::QSphereMesh * mesh = new Qt3DExtras::QSphereMesh(earth);
    mesh->setGenerateTangents(true);
    mesh->setRings(64);
    mesh->setSlices(64);
    mesh->setRadius(data["radius"].toDouble());
    earth->setMesh(mesh);

    Qt3DCore::QTransform * transform = new Qt3DCore::QTransform(earth);
    transform->setTranslation(QVector3D(data["x"].toDouble(),
                                        data["y"].toDouble(),
                                        data["z"].toDouble()));

    earth->setTransform(transform);

    RotationParameters * rotation = new RotationParameters(earth);
    rotation->jsonIni(data);

    earth->setRotational(rotation);

    Qt3DExtras::QDiffuseSpecularMaterial * planetmaterial = new Qt3DExtras::QDiffuseSpecularMaterial(earth);


    planetmaterial->setAmbient(QColor(  data["ambientr"].toInt(),
                                        data["ambientg"].toInt(),
                                        data["ambientb"].toInt()));
    planetmaterial->setShininess(data["shininess"].toDouble());


    Qt3DRender::QTexture2D * diffuseMapText = new Qt3DRender::QTexture2D;
    diffuseMapText->setMinificationFilter(Qt3DRender::QTexture2D::LinearMipMapLinear);
    diffuseMapText->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
    diffuseMapText->setMaximumAnisotropy(16.0);//graphics comming
    diffuseMapText->setGenerateMipMaps(true);

    Qt3DRender::QTextureWrapMode diffuseWrapMode;
    diffuseWrapMode.setX(Qt3DRender::QTextureWrapMode::Repeat);
    diffuseWrapMode.setY(Qt3DRender::QTextureWrapMode::Repeat);
    diffuseMapText->setWrapMode(diffuseWrapMode);

    Qt3DRender::QTextureImage * diffuseMapTextureImage = new Qt3DRender::QTextureImage;
    diffuseMapTextureImage->setSource(QUrl(data["diffuseText"].toString()));//change *******
    diffuseMapText->addTextureImage(diffuseMapTextureImage);

    planetmaterial->setDiffuse(QVariant::fromValue(diffuseMapText));

    Qt3DRender::QTexture2D * specularMapText = new Qt3DRender::QTexture2D;
    specularMapText->setMinificationFilter(Qt3DRender::QTexture2D::LinearMipMapLinear);
    specularMapText->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
    specularMapText->setMaximumAnisotropy(16.0);//graphics comming
    specularMapText->setGenerateMipMaps(true);

    Qt3DRender::QTextureWrapMode specularWrapMode;
    specularWrapMode.setX(Qt3DRender::QTextureWrapMode::Repeat);
    specularWrapMode.setY(Qt3DRender::QTextureWrapMode::Repeat);
    specularMapText->setWrapMode(specularWrapMode);

    Qt3DRender::QTextureImage * specularMapTextureImage = new Qt3DRender::QTextureImage;
    specularMapTextureImage->setSource(QUrl(data["specularTexture"].toString()));//change *******
    specularMapText->addTextureImage(specularMapTextureImage);

    planetmaterial->setSpecular(QVariant::fromValue(specularMapText));

    Qt3DRender::QTexture2D * normalMapText = new Qt3DRender::QTexture2D;
    normalMapText->setMinificationFilter(Qt3DRender::QTexture2D::Linear);
    normalMapText->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
    normalMapText->setMaximumAnisotropy(16.0);//graphics comming

    Qt3DRender::QTextureWrapMode normalWrapMode;
    normalWrapMode.setX(Qt3DRender::QTextureWrapMode::Repeat);
    normalWrapMode.setY(Qt3DRender::QTextureWrapMode::Repeat);
    normalMapText->setWrapMode(normalWrapMode);

    Qt3DRender::QTextureImage * normalMapTextureImage = new Qt3DRender::QTextureImage;
    normalMapTextureImage->setSource(QUrl(data["normalTexture"].toString()));//change *******
    normalMapText->addTextureImage(normalMapTextureImage);

    planetmaterial->setNormal(QVariant::fromValue(normalMapText));


    Qt3DRender::QEffect * effect = planetmaterial->effect();

    Qt3DRender::QFilterKey * renderingStyle = new Qt3DRender::QFilterKey;
    renderingStyle->setName(QStringLiteral("renderingStyle"));
    renderingStyle->setValue(QStringLiteral("forward"));

    Qt3DRender::QShaderProgram* gl3Shader = new Qt3DRender::QShaderProgram;
    gl3Shader->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/assets/shaders/gl3/planetDB.vert")));
    gl3Shader->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/assets/shaders/gl3/planetDB.frag")));

    Qt3DRender::QShaderProgram *es2Shader = new Qt3DRender::QShaderProgram;
    es2Shader->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/assets/shaders/es2/planetDB.vert")));
    es2Shader->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl("qrc:/assets/shaders/es2/planetDB.frag")));


    Qt3DRender::QRenderPass* gl3RenderPass = new Qt3DRender::QRenderPass;
    gl3RenderPass->setShaderProgram(gl3Shader);

    Qt3DRender::QTechnique*gl3Technique = new Qt3DRender::QTechnique;
    gl3Technique->addFilterKey(renderingStyle);
    gl3Technique->addRenderPass(gl3RenderPass);
    gl3Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    gl3Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);
    gl3Technique->graphicsApiFilter()->setMajorVersion(3);
    gl3Technique->graphicsApiFilter()->setMinorVersion(2);

    Qt3DRender::QRenderPass*gl2RenderPass = new Qt3DRender::QRenderPass;
    gl2RenderPass->setShaderProgram(es2Shader);

    Qt3DRender::QTechnique*gl2Technique = new Qt3DRender::QTechnique;
    gl2Technique->addFilterKey(renderingStyle);
    gl2Technique->addRenderPass(gl3RenderPass);
    gl2Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    gl2Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::NoProfile);
    gl2Technique->graphicsApiFilter()->setMajorVersion(2);
    gl2Technique->graphicsApiFilter()->setMinorVersion(0);

    Qt3DRender::QRenderPass*es2RenderPass = new Qt3DRender::QRenderPass;
    es2RenderPass->setShaderProgram(es2Shader);

    Qt3DRender::QTechnique*es2Technique = new Qt3DRender::QTechnique;
    es2Technique->addFilterKey(renderingStyle);
    es2Technique->addRenderPass(es2RenderPass);
    es2Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGLES);
    es2Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);
    es2Technique->graphicsApiFilter()->setMajorVersion(2);
    es2Technique->graphicsApiFilter()->setMinorVersion(0);


    //! FIXME
    effect->addTechnique(gl3Technique);
    //effect->addTechnique(gl2Technique);
    //effect->addTechnique(es2Technique);

    Qt3DRender::QParameter * lightpos = new Qt3DRender::QParameter;
    lightpos->setName("light.position");
    lightpos->setValue(QVector4D(20.0,30.0, 0.0, 1.0 ));

    Qt3DRender::QParameter * lightintensity = new Qt3DRender::QParameter;
    lightintensity->setName("light.intensity");
    lightintensity->setValue(QVector3D(1.0, 1.0, 1.0));


    effect->addParameter(lightintensity);
    effect->addParameter(lightpos);

    earth->setMaterial(planetmaterial);

    //! Add moons,rings and stations
    //!

    QJsonArray moonsdata = data["moons"].toArray();
    Q_FOREACH(QJsonValue moondata,moonsdata){
        earth->setMoon(moon::iniFromJson(moondata.toObject(),earth));
    }
    /*
    QJsonArray ringsdata = data["rings"].toArray();
    Q_FOREACH(QJsonValue ringdata,ringsdata){
        earth->setMoon(moon::iniFromJson(ringdata.toObject(),earth));
    }
    QJsonArray stationsdata = data["spacestations"].toArray();
    Q_FOREACH(QJsonValue stationdata,stationsdata){
        earth->setMoon(moon::iniFromJson(stationdata.toObject(),earth));
    }
    */

    qDebug() <<"generated "<<earth->name()<<endl;

    return earth;

}

QString planet::eid() const
{
    return Eid;
}

void planet::setEid(QString eid)
{
    Eid = eid;
}

Qt3DCore::QTransform *planet::transform() const
{
    return Transform;
}

void planet::setTransform(Qt3DCore::QTransform *newTransform)
{
    if(newTransform != Transform){
        Transform = newTransform;
        addComponent(Transform);
        emit transformChanged();
    }
}

Qt3DRender::QMaterial *planet::material() const
{
    return Material;
}

void planet::setMaterial(Qt3DRender::QMaterial *material)
{
    if(Material!=material){
        Material = material;
        addComponent(Material);
        emit materialChanged();
    }
}

Qt3DExtras::QSphereMesh *planet::mesh() const
{
    return Mesh;
}

void planet::setMesh(Qt3DExtras::QSphereMesh *mesh)
{
    if(Mesh!=mesh){
        Mesh = mesh;
        addComponent(Mesh);
        emit meshChanged();
    }
}

RotationParameters *planet::rotational() const
{
    return Rotational;
}

void planet::setRotational(RotationParameters *rotational)
{
    if(Rotational!=rotational){
        Rotational = rotational;
        emit rotationalChanged();
    }
}

QString planet::name() const
{
    return Name;
}

void planet::setName(QString name)
{
    Name = name;
    setObjectName(Name);
}

quint16 planet::campus() const
{
    return usable_campus;
}

void planet::setCampus(quint16 campus)
{
    if(campus!=usable_campus){
        usable_campus = campus;
        emit campusChanged();
    }
}

double planet::temperature() const
{
    return Temperature;
}


void planet::setTemperature(double temperature)
{
    if(!qFuzzyCompare(temperature,Temperature)){
        Temperature = temperature;
        emit temperatureChanged();
    }
}

int planet::type() const
{
    return Type;
}

void planet::setType(int type)
{
    if(type!=Type){
        Type = type;
        emit typeChanged();
    }
}

void planet::setStructures(QJsonObject structures)
{
    if(Structres != structures){
        Structres = structures;
        emit structuresChanged();
    }
}

QJsonObject planet::structures() const
{
    return Structres;
}

void planet::setScale(float scale)
{
    if(!qFuzzyCompare(scale,Scale)){
        Scale=scale;
        Transform->setScale(Scale);
        emit scaleChanged();
    }
}

float planet::scale() const
{
    return Scale;
}

void planet::setMoon(moon *newmoon)
{
    moons.append(newmoon);
}

void planet::removeMoon(moon *moonToRemove)
{
    moons.removeOne(moonToRemove);
}

void planet::setRing(ring *newring)
{
    rings.append(newring);
}

void planet::removeRing(ring *ringToRemove)
{
    rings.removeOne(ringToRemove);
}

void planet::setSpaceStation(spacestation *newspacestation)
{
    spacestations.append(newspacestation);
}

void planet::removeSpaceStation(spacestation *spacestationToRemove)
{
    spacestations.removeOne(spacestationToRemove);
}

void planet::picked(Qt3DRender::QPickEvent *ev)
{
    qDebug() << "Planet  "<<Name<<" picked"<<endl;
}

void planet::animate(float day)
{
    if(Rotational==nullptr)
        return;
    Rotational->calculateTransform(Transform,day);
   // Q_FOREACH(moon*themoon,moons){
    //    themoon->animate(day);
   // }
}

