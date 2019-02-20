#include "star.h"

star::star(Qt3DCore::QEntity *root) : Qt3DCore::QEntity(root)
{
    Scale = 1.0;
    MrPickles = new Qt3DRender::QObjectPicker(this);
    MrPickles->setDragEnabled(false);
    MrPickles->setHoverEnabled(false);
    MrPickles->setEnabled(true);

    connect(MrPickles,&Qt3DRender::QObjectPicker::clicked,
            this,&star::clickedMsg);   
    addComponent(MrPickles);

    Rotation = nullptr;

}

star *star::generateStarFromJson(QJsonObject data, Qt3DCore::QEntity *parent)
{
    qDebug() <<"Creating sun "<<data<<endl;

    star * sun = new star(parent);

    sun->setEid(data["id"].toString());
    sun->setStarname(data["name"].toString());

    Qt3DExtras::QSphereMesh*mesh = new Qt3DExtras::QSphereMesh(sun);
    //! TODO grapics settings
    mesh->setGenerateTangents(true);
    mesh->setRings(64);
    mesh->setSlices(64);
    mesh->setRadius(data["radius"].toDouble());
    sun->setMesh(mesh);


    Qt3DCore::QTransform*transform = new Qt3DCore::QTransform(sun);
    transform->setTranslation(QVector3D(data["x"].toDouble(),
                                        data["y"].toDouble(),
                                        data["z"].toDouble()));
    transform->setRotationX(data["picth"].toDouble());
    transform->setRotationY(data["jaw"].toDouble());
    transform->setRotationZ(data["roll"].toDouble());

    sun->setTransform(transform);


    Qt3DRender::QPointLight*starfire = new Qt3DRender::QPointLight(sun);
    starfire->setColor(QColor(  data["colorr"].toInt(),
                                data["colorg"].toInt(),
                                data["colorb"].toInt()));
    starfire->setIntensity(data["intensity"].toDouble());

    sun->setStarfire(starfire);

    //!TODO add materials

    Qt3DExtras::QDiffuseSpecularMaterial * starmaterial =
            new Qt3DExtras::QDiffuseSpecularMaterial(sun);

    starmaterial->setAmbient(QColor(245,240,245));
    starmaterial->setSpecular(QColor(125.5,125.5,125.5));
    starmaterial->setShininess(150.0);

    Qt3DRender::QTexture2D * diffuseMapText = new Qt3DRender::QTexture2D;
    diffuseMapText->setMinificationFilter(Qt3DRender::QTexture2D::LinearMipMapLinear);
    diffuseMapText->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
    diffuseMapText->setMaximumAnisotropy(16.0);//graphics comming
    diffuseMapText->setGenerateMipMaps(true);

    Qt3DRender::QTextureWrapMode diffuseWrapMode;
    diffuseWrapMode.setX(Qt3DRender::QTextureWrapMode::Repeat);
    diffuseWrapMode.setY(Qt3DRender::QTextureWrapMode::Repeat);
    diffuseWrapMode.setZ(Qt3DRender::QTextureWrapMode::Repeat);
    diffuseMapText->setWrapMode(diffuseWrapMode);

    Qt3DRender::QTextureImage * sundiffuseMapTextureImage = new Qt3DRender::QTextureImage;
    sundiffuseMapTextureImage->setSource(QUrl(data["diffuseTexture"].toString()));
    diffuseMapText->addTextureImage(sundiffuseMapTextureImage);

    starmaterial->setDiffuse(QVariant::fromValue(diffuseMapText));

    //! EFFECT

    Qt3DRender::QEffect * effect = starmaterial->effect();

    Qt3DRender::QTechnique *gl3Technique = new Qt3DRender::QTechnique(effect);
    Qt3DRender::QRenderPass *gl3Pass = new Qt3DRender::QRenderPass(effect);

    Qt3DRender::QFilterKey *filterKey = new Qt3DRender::QFilterKey(effect);
    filterKey->setName("pass");
    filterKey->setValue("forward");

    Qt3DRender::QShaderProgram *glShader = new Qt3DRender::QShaderProgram(effect);

    sun->analizeShader(glShader);

    glShader->setVertexShaderCode(Qt3DRender::QShaderProgram::
                                  loadSource(QUrl("qrc:/assets/shaders/gl3/sunx.vert")));

    glShader->setFragmentShaderCode(Qt3DRender::QShaderProgram::
                                    loadSource(QUrl("qrc:/assets/shaders/gl3/sun.frag")));

    gl3Pass->setShaderProgram(glShader);
    gl3Pass->addFilterKey(filterKey);

    gl3Technique->addRenderPass(gl3Pass);

    gl3Technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    gl3Technique->graphicsApiFilter()->setMajorVersion(3);
    gl3Technique->graphicsApiFilter()->setMinorVersion(2);
    gl3Technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);

    effect->addTechnique(gl3Technique);

    sun->setMaterial(starmaterial);


    //! Generate Planets
    //!

    RotationParameters * rotational = new RotationParameters(sun);
    rotational->jsonIni(data);
    sun->setRotational(rotational);

    QJsonArray planetsdata = data["planets"].toArray();
    Q_FOREACH(QJsonValue planetdata,planetsdata){
        sun->setPlanet(planet::generateFromJson(planetdata.toObject(),
                                                sun));
    }
    /*
    QJsonArray spacestationsdata = data["spacestations"].toArray();
    Q_FOREACH(QJsonValue spacestationdata,spacestationsdata){
        sun->setSpaceStation(spacestation::generateFromJson(spacestationdata.toObject(),
                                                            sun));
    }
*/

    return sun;
}

QString star::eid() const
{
    return Eid;
}

void star::setEid(QString eid)
{
    Eid = eid;
}

void star::setPosition(QVector3D &newpostion)
{
    if(!qFuzzyCompare(newpostion,Transform->translation())){
        Transform->setTranslation(newpostion);
        emit positionChanged();
    }
}

const QVector3D star::position() const
{
    return Transform->translation();
}

Qt3DExtras::QSphereMesh *star::mesh()
{
    return Mesh;
}

void star::setMesh(Qt3DExtras::QSphereMesh *mesh)
{
    if(Mesh!=mesh){
        Mesh = mesh;
        addComponent(Mesh);
        emit meshChanged();
    }
}

Qt3DRender::QMaterial *star::material()
{
    return Material;
}

void star::setMaterial(Qt3DRender::QMaterial *material)
{
    if(Material != material){
        Material = material;
        addComponent(Material);
        emit materialChanged();
    }
}

Qt3DCore::QTransform *star::transform()
{
    return Transform;
}

void star::setTransform(Qt3DCore::QTransform *transform)
{
    if(Transform!=transform){
        Transform = transform;
        addComponent(Transform);
        emit transformChanged();
    }
}

Qt3DRender::QPointLight *star::starfire()
{
    return Starfire;
}

void star::setStarfire(Qt3DRender::QPointLight *starfire)
{
    if(Starfire != starfire){
        Starfire = starfire;
        addComponent(Starfire);
        emit starfireChanged();
    }
}

RotationParameters *star::rotational()
{
    return Rotation;
}

void star::setRotational(RotationParameters *rotational)
{
    if(Rotation!=rotational){
        Rotation=rotational;
        emit rotationalChanged();
    }
}

float star::scale()
{
    return Scale;
}

void star::setScale(float newscale)
{
    if(!qFuzzyCompare(newscale,Scale)){
        Scale = newscale;
        Transform->setScale(Scale);
        emit scaleChanged();
    }
}

Qt3DRender::QObjectPicker *star::picker() const
{
    return MrPickles;
}

void star::setPicker(Qt3DRender::QObjectPicker *newpicker)
{
    if(newpicker!=MrPickles){
        disconnect(MrPickles,&Qt3DRender::QObjectPicker::clicked,
                this,&star::clickedMsg);
        MrPickles = newpicker;
        MrPickles->setEnabled(true);
        addComponent(MrPickles);
        connect(MrPickles,&Qt3DRender::QObjectPicker::clicked,
                this,&star::clickedMsg);
        emit pickerChanged();
    }
}

QString star::starname() const
{
    return Name;
}

void star::setStarname(QString starname)
{
    Name = starname;
    setObjectName(Name);
}


void star::setRoll(float roll)
{
    if(roll!=Transform->rotationY()){
        Transform->setRotationY(roll);
        emit rollChanged();
    }
}

float star::roll()
{
    return Transform->rotationY();
}

void star::shaderLog(QString log)
{
    qCritical()<<"Shader: "<<log<<endl;
}

void star::shaderStatus(Qt3DRender::QShaderProgram::Status status)
{
    qDebug() << "shader status changed "<<endl;
    switch (status) {
    case Qt3DRender::QShaderProgram::NotReady:
        qWarning()<<"Shader status not ready"<<endl;
        break;
    case Qt3DRender::QShaderProgram::Ready:
        qDebug()<<"Shader ready "<<endl;
        break;
    case Qt3DRender::QShaderProgram::Error:
        qFatal("Shader error");
        break;
    default:
        break;
    }

}

void star::analizeShader(Qt3DRender::QShaderProgram *  shader)
{
    connect(shader,&Qt3DRender::QShaderProgram::logChanged,this,&star::shaderLog);
    connect(shader,&Qt3DRender::QShaderProgram::statusChanged,this,&star::shaderStatus);
}

QList<planet *> star::getPlanets() const
{
    return planets;
}

void star::clickedMsg(Qt3DRender::QPickEvent *pick)
{
    //Q_UNUSED(pick)
    qDebug() << "Star "<<Name<<" clicked"<<endl;
    pick->setAccepted(true);
}

void star::setPlanet(planet *newplanet)
{
    planets.append(newplanet);
}

void star::removePlanet(planet *planetToRemove)
{
    planets.removeOne(planetToRemove);
    //TODO memory mag
}

void star::setSpaceStation(spacestation *newspacestation)
{
    spacestations.append(newspacestation);
}

void star::removeSpaceStation(spacestation *spacestationToRemove)
{
    spacestations.removeOne(spacestationToRemove);
    //! TODO memory delete
}

void star::animate(float day)
{
    qDebug() << "animating "<<day<<endl;
    if(Rotation==nullptr)
        return;
    Rotation->calculateTransform(Transform,day);
    Q_FOREACH(planet* planeto,planets){
        planeto->animate(day);
    }
}

