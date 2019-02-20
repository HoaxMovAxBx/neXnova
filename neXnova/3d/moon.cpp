#include "moon.h"

moon::moon(Qt3DCore::QEntity *root) : Qt3DCore::QEntity(root)
{
    Scale = 1.0;
    MrPickles = new Qt3DRender::QObjectPicker(this);
    MrPickles->setDragEnabled(false);
    MrPickles->setHoverEnabled(false);
    connect(MrPickles,&Qt3DRender::QObjectPicker::clicked,this,&moon::clicked);
    addComponent(MrPickles);
}

void moon::ini()
{
    mesh = new Qt3DExtras::QSphereMesh(this);
    mesh->setGenerateTangents(true);
    mesh->setRings(64);
    mesh->setSlices(64);
    mesh->setRadius(1.5424);
    addComponent(mesh);

    transformptr = new Qt3DCore::QTransform(this);
    transformptr->setTranslation(QVector3D(20, 0, 0));
    addComponent(transformptr);
    material = new Qt3DExtras::QNormalDiffuseMapMaterial(this);

    reinterpret_cast<Qt3DExtras::QNormalDiffuseMapMaterial*>(material)->setAmbient(QColor(22,22,22));
    reinterpret_cast<Qt3DExtras::QNormalDiffuseMapMaterial*>(material)->setSpecular(QColor(51,51,51));
    reinterpret_cast<Qt3DExtras::QNormalDiffuseMapMaterial*>(material)->setShininess(50.0);

    Qt3DRender::QTextureWrapMode WrapMode;
    WrapMode.setX(Qt3DRender::QTextureWrapMode::Repeat);
    WrapMode.setY(Qt3DRender::QTextureWrapMode::Repeat);

    Qt3DRender::QTextureImage * moondiffuseMapTextureImage = new Qt3DRender::QTextureImage;
    moondiffuseMapTextureImage->setSource(QUrl("qrc:/assets/images/solarsystemscope/moonmap2k.jpg"));

    Qt3DRender::QTextureImage * moonnormalMapTextureImage = new Qt3DRender::QTextureImage;
    moonnormalMapTextureImage->setSource(QUrl("qrc:/assets/images/solarsystemscope/moonnormal2k.jpg"));


    Qt3DRender::QTexture2D * diffuseMapText = new Qt3DRender::QTexture2D;
    diffuseMapText->setMinificationFilter(Qt3DRender::QTexture2D::LinearMipMapLinear);
    diffuseMapText->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
    diffuseMapText->setMaximumAnisotropy(16.0);//graphics comming
    diffuseMapText->setGenerateMipMaps(true);
    diffuseMapText->addTextureImage(moondiffuseMapTextureImage);
    diffuseMapText->setWrapMode(WrapMode);

    Qt3DRender::QTexture2D * normalMapText = new Qt3DRender::QTexture2D;
    normalMapText->setMinificationFilter(Qt3DRender::QTexture2D::LinearMipMapLinear);
    normalMapText->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
    normalMapText->setMaximumAnisotropy(16.0);//graphics comming
    normalMapText->setGenerateMipMaps(true);
    normalMapText->addTextureImage(moonnormalMapTextureImage);
    normalMapText->setWrapMode(WrapMode);


    reinterpret_cast<Qt3DExtras::QNormalDiffuseMapMaterial*>(material)->setDiffuse(diffuseMapText);
    reinterpret_cast<Qt3DExtras::QNormalDiffuseMapMaterial*>(material)->setNormal(normalMapText);

    addComponent(material);

    moonlightptr = new Qt3DRender::QPointLight(this);
    moonlightptr->setColor(QColor(220,250,200));
    moonlightptr->setIntensity(0.3);

    addComponent(moonlightptr);
}

moon *moon::iniFromJson(QJsonObject data, Qt3DCore::QEntity* parent)
{
    moon * themoon = new moon(parent);

    themoon->setEid(data["id"].toString());
    themoon->setName(data["name"].toString());
    themoon->setDescription(data["description"].toString());
    themoon->setType(data["type"].toInt());
    //themoon->setStructures(data["structures"].toObject());
    qDebug() << "step1"<<endl;
    //! 3D
    Qt3DExtras::QSphereMesh * xmesh = new Qt3DExtras::QSphereMesh(themoon);
    xmesh->setGenerateTangents(true);
    xmesh->setRings(64);
    xmesh->setSlices(64);
    //! TODO put in graphics setings

    //!Radius
    xmesh->setRadius(data["radius"].toDouble());
    themoon->setMoonmesh(xmesh);
    qDebug() << "step2"<<endl;
    Qt3DCore::QTransform * xtransform = new Qt3DCore::QTransform(themoon);
    xtransform->setTranslation(QVector3D(25.0,25.0,0.0));
    themoon->setMoontransform(xtransform);

    Qt3DRender::QPointLight * xlight = new Qt3DRender::QPointLight(themoon);

    switch (data["type"].toInt()) {
    case 0:
        xlight->setColor(QColor(220,250,200));
        xlight->setIntensity(0.3);
        break;
    default:
        xlight->setColor(QColor(220,250,200));
        xlight->setIntensity(0.3);
        break;
    }
    themoon->setMoonlight(xlight);
    qDebug() << "step3"<<endl;
    Qt3DExtras::QDiffuseSpecularMaterial * xmaterial =
                new Qt3DExtras::QDiffuseSpecularMaterial(themoon);

    xmaterial->setAmbient(QColor(   data["ambientr"].toInt(),
                                    data["ambientg"].toInt(),
                                    data["ambientb"].toInt()));
    xmaterial->setSpecular(QColor(  data["specularr"].toInt(),
                                    data["specularg"].toInt(),
                                    data["specularb"].toInt()));
    xmaterial->setShininess(data["shininess"].toDouble());

    Qt3DRender::QTextureWrapMode xWrapMode;
    xWrapMode.setX(Qt3DRender::QTextureWrapMode::Repeat);
    xWrapMode.setY(Qt3DRender::QTextureWrapMode::Repeat);

    Qt3DRender::QTextureImage * xmoondiffuseMapTextureImage = new Qt3DRender::QTextureImage;
    xmoondiffuseMapTextureImage->setSource(QUrl(data["diffuseTexture"].toString()));

    Qt3DRender::QTextureImage * xmoonnormalMapTextureImage = new Qt3DRender::QTextureImage;
    xmoonnormalMapTextureImage->setSource(QUrl(data["normalTexture"].toString()));


    Qt3DRender::QTexture2D * xdiffuseMapText = new Qt3DRender::QTexture2D;
    xdiffuseMapText->setMinificationFilter(Qt3DRender::QTexture2D::LinearMipMapLinear);
    xdiffuseMapText->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
    xdiffuseMapText->setMaximumAnisotropy(16.0);//graphics comming
    xdiffuseMapText->setGenerateMipMaps(true);
    xdiffuseMapText->addTextureImage(xmoondiffuseMapTextureImage);
    xdiffuseMapText->setWrapMode(xWrapMode);

    Qt3DRender::QTexture2D * xnormalMapText = new Qt3DRender::QTexture2D;
    xnormalMapText->setMinificationFilter(Qt3DRender::QTexture2D::LinearMipMapLinear);
    xnormalMapText->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
    xnormalMapText->setMaximumAnisotropy(16.0);//graphics comming
    xnormalMapText->setGenerateMipMaps(true);
    xnormalMapText->addTextureImage(xmoonnormalMapTextureImage);
    xnormalMapText->setWrapMode(xWrapMode);

    xmaterial->setDiffuse(QVariant::fromValue(xdiffuseMapText));
    xmaterial->setNormal(QVariant::fromValue(xnormalMapText));

    themoon->setMoonmaterial(xmaterial);
    qDebug() << "step4"<<endl;
    RotationParameters * rotationp = new RotationParameters(themoon);
    rotationp->jsonIni(data);

    //themoon->setMoonrotation(rotationp);
    qDebug() << "step5"<<endl;
    return themoon;

}

void moon::animate(float day)
{
    qDebug() << "animating moon..."<<endl;
    rotational->calculateTransform(transformptr,day);

#if D3Debug
    qDebug() << "Moon pos "<<transformptr->translation() << endl;
#endif
}

RotationParameters *moon::moonrotation()
{
    return rotational;
}

Qt3DRender::QPointLight *moon::moonlight()
{
    return moonlightptr;
}

Qt3DRender::QMaterial *moon::moonmaterial()
{
    return material;
}

Qt3DCore::QTransform *moon::moontransform()
{
    return transformptr;
}

Qt3DExtras::QSphereMesh *moon::moonmesh()
{
    return mesh;
}

float moon::scale()
{
    return Scale;
}

QString moon::name()
{
    return Name;
}

QString moon::description()
{
    return Description;
}

int moon::type()
{
    return Type;
}

QJsonObject moon::structures()
{
    return Structures;
}

QString moon::eid() const
{
    return Eid;
}

void moon::setEid(QString eid)
{
    Eid = eid;
}

void moon::setMoonrotation(RotationParameters *rotation)
{
    rotational = rotation;
}

void moon::setMoonlight(Qt3DRender::QPointLight *light)
{
    if(light!=moonlightptr){
        moonlightptr = light;
        addComponent(moonlightptr);
        emit moonlightChanged();
    }
}

void moon::setMoonmaterial(Qt3DRender::QMaterial *mater)
{
    if(material!=mater){
        material = mater;
        addComponent(material);
        emit moonmaterialChanged();
    }
}

void moon::setMoontransform(Qt3DCore::QTransform *transform)
{
    if(transformptr!=transform){
        transformptr = transform;
        addComponent(transformptr);
        emit moontransformChanged();
    }
}

void moon::setMoonmesh(Qt3DExtras::QSphereMesh *m)
{
    if(mesh!=m){
        mesh = m;
        addComponent(mesh);
        emit moonmeshChanged();
    }
}

void moon::setScale(float newscale)
{
    if(!qFuzzyCompare(newscale,Scale)){
        Scale = newscale;
        transformptr->setScale(Scale);
        emit scaleChanged();
    }
}

void moon::setName(QString newname)
{
    if(newname != Name){
        Name = newname;
        emit nameChanged();
    }
}

void moon::setDescription(QString newdes)
{
    if(Description!=newdes){
        Description = newdes;
        emit descriptionChanged();
    }
}

void moon::setStructures(QJsonObject newstructures)
{
    if(Structures != newstructures){
        Structures = newstructures;
        emit structuresChanged();
    }
}

void moon::setType(int type)
{
    Type = type;
}



