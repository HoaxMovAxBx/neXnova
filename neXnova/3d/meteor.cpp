#include "meteor.h"

meteor::meteor(Qt3DCore::QEntity *parent) : Qt3DCore::QEntity(parent)
{
    MrPickles = new Qt3DRender::QObjectPicker(this);
    MrPickles->setDragEnabled(false);
    MrPickles->setHoverEnabled(false);
    connect(MrPickles,&Qt3DRender::QObjectPicker::clicked,this,&meteor::clicked);
    addComponent(MrPickles);
}

void meteor::animate(float day){
    Q_UNUSED(day)
    //! TODO implement
    qFatal("Unimplemented");
}

meteor *meteor::generateFromJson(QJsonObject data, Qt3DCore::QEntity *parent)
{
    meteor * meteora = new meteor(parent);

    meteora->setEid(data["id"].toString());
    meteora->setName(data["name"].toString());

    Qt3DRender::QMesh* mesh = new Qt3DRender::QMesh(meteora);
    mesh->setSource(QUrl(data["mesh"].toString()));

    meteora->setMesh(mesh);


    Qt3DCore::QTransform* transform = new Qt3DCore::QTransform(meteora);
    transform->setTranslation(QVector3D(data["x"].toDouble(),
                                        data["y"].toDouble(),
                                        data["z"].toDouble()));

    transform->setRotationX(data["pitch"].toDouble());
    transform->setRotationY(data["jaw"].toDouble());
    transform->setRotationZ(data["roll"].toDouble());

    meteora->setTransform(transform);

    Qt3DExtras::QDiffuseSpecularMaterial  * meteormaterial = new Qt3DExtras::
             QDiffuseSpecularMaterial(meteora);

    Qt3DRender::QParameter * ambient = new Qt3DRender::QParameter;
    ambient->setName("ambient");
    ambient->setValue(QColor(   data["ambientr"].toInt(),
                                data["ambientg"].toInt(),
                                data["ambientb"].toInt()));

    Qt3DRender::QParameter * diffuse = new Qt3DRender::QParameter;
    diffuse->setName("diffuse");
    diffuse->setValue(QColor(   data["diffuser"].toInt(),
                                data["diffuseg"].toInt(),
                                data["diffuseb"].toInt()));

    Qt3DRender::QParameter * specular = new Qt3DRender::QParameter;
    specular->setName("specular");
    specular->setValue(QColor(  data["secularr"].toInt(),
                                data["secularg"].toInt(),
                                data["secularb"].toInt()));

    Qt3DRender::QParameter * shininess = new Qt3DRender::QParameter;
    shininess->setName("shininess");
    shininess->setValue(data["shininess"].toDouble());


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
    diffuseMapTextureImage->setSource(QUrl(data["diffuseTexture"].toString()));
    diffuseMapText->addTextureImage(diffuseMapTextureImage);

    Qt3DRender::QParameter * diffuseMap = new Qt3DRender::QParameter("diffuseTexture",
                                                                     diffuseMapText);

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
    specularMapTextureImage->setSource(QUrl(data["specularTexture"].toString()));
    specularMapText->addTextureImage(specularMapTextureImage);

    Qt3DRender::QParameter * specularMap = new Qt3DRender::QParameter("specularTexture",
                                                                      specularMapText);


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

    Qt3DRender::QParameter * normalMap = new Qt3DRender::QParameter("normalTexture",
                                                                    normalMapText);

    meteormaterial->addParameter(ambient);
    meteormaterial->addParameter(diffuse);
    meteormaterial->addParameter(specular);
    meteormaterial->addParameter(shininess);
    meteormaterial->addParameter(diffuseMap);
    meteormaterial->addParameter(specularMap);
    meteormaterial->addParameter(normalMap);

    //! TODO add effects
    //!
    //!
    meteora->setMaterial(meteormaterial);

    RotationParameters * rotation = new RotationParameters(meteora);
    rotation->jsonIni(data);

    meteora->setRotational(rotation);

    Qt3DRender::QPointLight * light = new Qt3DRender::QPointLight(meteora);
    light->setColor(QColor( data["lightr"].toInt(),
                            data["lightg"].toInt(),
                            data["lightb"].toInt()));

    meteora->setLight(light);

    return meteora;

}

void meteor::setMesh(Qt3DRender::QMesh *mesh)
{
    if(Mesh!=mesh){
        Mesh = mesh;
        addComponent(Mesh);
        emit meshChanged();
    }
}

Qt3DRender::QMesh *meteor::mesh() const
{
    return Mesh;
}

void meteor::setTransform(Qt3DCore::QTransform *transform)
{
    if(Transform != transform){
        Transform = transform;
        addComponent(Transform);
        emit transformChanged();
    }
}

Qt3DCore::QTransform *meteor::transform() const
{
    return Transform;
}

void meteor::setMaterial(Qt3DRender::QMaterial *material)
{
    if(Material!=material){
        Material = material;
        addComponent(Material);
        emit materialChanged();
    }
}

Qt3DRender::QMaterial *meteor::material() const
{
    return Material;
}

void meteor::setRotational(RotationParameters *rotational)
{
    if(Rotational != rotational){
        Rotational = rotational;
        emit rotationalChanged();
    }
}

RotationParameters *meteor::rotational() const
{
    return Rotational;
}

void meteor::setLight(Qt3DRender::QPointLight *light)
{
    if(Light != light){
        Light = light;
        addComponent(Light);
        emit lightChanged();
    }
}

Qt3DRender::QPointLight *meteor::light() const
{
    return Light;
}

void meteor::setName(QString name)
{
    Name = name;
}

QString meteor::name() const
{
    return Name;
}

void meteor::setPosition(QVector3D position)
{
    if(!qFuzzyCompare(position,Transform->translation())){
        Transform->setTranslation(position);
        emit positionChanged();
    }
}

QVector3D meteor::position() const
{
    return Transform->translation();
}

QString meteor::eid() const
{
    return Eid;
}

void meteor::setEid(QString eid)
{
    Eid = eid;
}

