#include "ring.h"

ring::ring(Qt3DCore::QEntity *parent) : Qt3DCore::QEntity(parent)
{

}

ring * ring::generateFromJson(QJsonObject ring_data,Qt3DCore::QEntity*parent)
{   
    ring * xring = new ring(parent);
    xring->setEid(ring_data["id"].toString());

    int type = ring_data["type"].toInt();

    xring->setType(type);
    xring->setName(ring_data["name"].toString());

    Qt3DRender::QMesh * mesh = new Qt3DRender::QMesh(xring);
    //! TODO add more rings
    switch (type) {
    case 0:
        mesh->setSource(QUrl("qrc:/assets/meshes/ring.obj"));
        break;
    default:
        mesh->setSource(QUrl("qrc:/assets/meshes/ring.obj"));
        break;
    }
    xring->setMesh(mesh);

    //! TODO picth,roll...
    Qt3DCore::QTransform * xtransform = new Qt3DCore::QTransform(xring);
    xtransform->setScale(ring_data["scale"].toDouble());

    xring->setTransform(xtransform);

    //! TODO 5 o 6 materials

    Qt3DRender::QTextureWrapMode diffuseWrapMode;
    diffuseWrapMode.setX(Qt3DRender::QTextureWrapMode::Repeat);
    diffuseWrapMode.setY(Qt3DRender::QTextureWrapMode::Repeat);

    Qt3DRender::QParameter * opacity = new Qt3DRender::QParameter;
    opacity->setName("opacity");
    opacity->setValue(ring_data["opacity"].toDouble());

    if(type == 0){

        Qt3DRender::QTextureImage * diffuseMapTextureImage = new Qt3DRender::QTextureImage;
        diffuseMapTextureImage->setSource(QUrl("qrc:/assets/images/solarsystemscope/saturnringcolortrans.png"));//change *******

        Qt3DExtras::QDiffuseMapMaterial * xmaterial =
                new Qt3DExtras::QDiffuseMapMaterial(xring);
        xmaterial->diffuse()->addTextureImage(diffuseMapTextureImage);
        xmaterial->diffuse()->setMinificationFilter(Qt3DRender::QTexture2D::LinearMipMapLinear);
        xmaterial->diffuse()->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
        xmaterial->diffuse()->setMaximumAnisotropy(16.0);//graphics comming
        xmaterial->diffuse()->setGenerateMipMaps(true);
        xmaterial->diffuse()->setWrapMode(diffuseWrapMode);

        xmaterial->setAmbient(QColor("#111111"));
        QColor specularc;
        specularc.setRgbF(0.01, 0.01, 0.01, 1.0);
        xmaterial->setSpecular(specularc);
        xmaterial->setShininess(1.0);
        xmaterial->addParameter(opacity);

        xring->setMaterial(xmaterial);
    }
    else{

        Qt3DRender::QTextureImage * diffuseMapTextureImage = new Qt3DRender::QTextureImage;
        diffuseMapTextureImage->setSource(QUrl("qrc:/assets/images/solarsystemscope/saturnringcolortrans.png"));

        Qt3DExtras::QDiffuseMapMaterial * xmaterial =
                new Qt3DExtras::QDiffuseMapMaterial(xring);
        xmaterial->diffuse()->addTextureImage(diffuseMapTextureImage);
        xmaterial->diffuse()->setMinificationFilter(Qt3DRender::QTexture2D::LinearMipMapLinear);
        xmaterial->diffuse()->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
        xmaterial->diffuse()->setMaximumAnisotropy(16.0);//graphics comming
        xmaterial->diffuse()->setGenerateMipMaps(true);
        xmaterial->diffuse()->setWrapMode(diffuseWrapMode);

        xmaterial->setAmbient(QColor("#111111"));
        QColor specularc;
        specularc.setRgbF(0.01, 0.01, 0.01, 1.0);
        xmaterial->setSpecular(specularc);
        xmaterial->setShininess(1.0);
        xmaterial->addParameter(opacity);

        xring->setMaterial(xmaterial);
    }

    return xring;
}

void ring::defaultRing(float parentRadius)
{
    Mesh = new Qt3DRender::QMesh(this);
    Mesh->setSource(QUrl("qrc:/assets/meshes/ring.obj"));

    Transform = new Qt3DCore::QTransform(this);

    //!var saturnOuterRadius = 120.700;
    //!var uranusOuterRadius = 40;

    float scalefactor = (parentRadius + 2.0 + 40.0) / 1.75;

    Transform->setScale(scalefactor);

    //! material
    Qt3DRender::QTextureWrapMode diffuseWrapMode;
    diffuseWrapMode.setX(Qt3DRender::QTextureWrapMode::Repeat);
    diffuseWrapMode.setY(Qt3DRender::QTextureWrapMode::Repeat);

    Qt3DRender::QTextureImage * diffuseMapTextureImage = new Qt3DRender::QTextureImage;
    diffuseMapTextureImage->setSource(QUrl("qrc:/assets/images/solarsystemscope/saturnringcolortrans.png"));//change *******

    Qt3DRender::QParameter * opacity = new Qt3DRender::QParameter;
    opacity->setName("opacity");
    opacity->setValue(0.4);

    Material = new Qt3DExtras::QDiffuseMapMaterial(this);
    reinterpret_cast<Qt3DExtras::QDiffuseMapMaterial*>(Material)->diffuse()->addTextureImage(diffuseMapTextureImage);
    reinterpret_cast<Qt3DExtras::QDiffuseMapMaterial*>(Material)->diffuse()->setMinificationFilter(Qt3DRender::QTexture2D::LinearMipMapLinear);
    reinterpret_cast<Qt3DExtras::QDiffuseMapMaterial*>(Material)->diffuse()->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
    reinterpret_cast<Qt3DExtras::QDiffuseMapMaterial*>(Material)->diffuse()->setMaximumAnisotropy(16.0);//graphics comming
    reinterpret_cast<Qt3DExtras::QDiffuseMapMaterial*>(Material)->diffuse()->setGenerateMipMaps(true);
    reinterpret_cast<Qt3DExtras::QDiffuseMapMaterial*>(Material)->diffuse()->setWrapMode(diffuseWrapMode);

    reinterpret_cast<Qt3DExtras::QDiffuseMapMaterial*>(Material)->setAmbient(QColor("#111111"));
    QColor specularc;
    specularc.setRgbF(0.01, 0.01, 0.01, 1.0);
    reinterpret_cast<Qt3DExtras::QDiffuseMapMaterial*>(Material)->setSpecular(specularc);
    reinterpret_cast<Qt3DExtras::QDiffuseMapMaterial*>(Material)->setShininess(1.0);
    Material->addParameter(opacity);

    addComponent(Mesh);
    addComponent(Transform);
    addComponent(Material);

}

Qt3DRender::QMesh *ring::mesh()
{
    return Mesh;
}

void ring::setMesh(Qt3DRender::QMesh *newmesh)
{
    if(Mesh!=newmesh){
        Mesh = newmesh;
        addComponent(Mesh);
        emit meshChanged();
    }
}

Qt3DRender::QMaterial *ring::material()
{
    return Material;
}

void ring::setMaterial(Qt3DRender::QMaterial *newmaterial)
{
    if(Material!=newmaterial){
        Material = newmaterial;
        addComponent(Material);
        emit materialChanged();
    }
}

Qt3DCore::QTransform *ring::transform()
{
    return Transform;
}

void ring::setTransform(Qt3DCore::QTransform *newtransform)
{
    if(Transform!=newtransform){
        Transform = newtransform;
        addComponent(Transform);
        emit transformChanged();
    }
}

void ring::setName(QString newname)
{
    Name = newname;
}

QString ring::name()
{
    return Name;
}

void ring::setType(int type)
{
    Type = type;
}

int ring::type()
{
    return Type;
}

QString ring::eid() const
{
    return Eid;
}

void ring::setEid(QString eid)
{
    Eid = eid;
}
