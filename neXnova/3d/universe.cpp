#include "universe.h"

Universe::Universe() : Qt3DCore::QEntity(nullptr)
{
    emit loading();

    setObjectName("Universe");

    camera = new ControledCamera(this);

    forwardrender = new Qt3DExtras::QForwardRenderer(this);
    forwardrender->setClearColor(QColor(0,0,0,0));
    forwardrender->setCamera(camera);

    rendersettings = new Qt3DRender::QRenderSettings(this);
    rendersettings->setActiveFrameGraph(forwardrender);
    addComponent(rendersettings);

    input = new Qt3DInput::QInputSettings(this);
    addComponent(input);

    controler = new Qt3DExtras::QFirstPersonCameraController(this);
    controler->setCamera(camera);

    syncanimator = new Qt3DLogic::QFrameAction(this);

    sky = new skybox(this);

}

void Universe::setRenderData(QJsonObject data)
{
    qDebug() << "Adding data to render "<<endl;
    sceneRoot = new SolarSistem(this);
    sceneRoot->generateFromJson(data);
    connect(syncanimator,&Qt3DLogic::QFrameAction::triggered,this,&Universe::animator);
    emit loaded();
}

void Universe::entityChanged(QString id, QJsonObject newdata)
{
    //!TODO implement here and in each entity
    qDebug() << "Entity "<<id<<" changed to "<<newdata<<endl;
}

void Universe::focusPlanet(QString planetName)
{
    qDebug() << "Focusing "<<planetName<<endl;
    camera->setFocusedItem(sceneRoot->getPlanetPos(planetName),
                           sceneRoot->getPlanetRadius(planetName));
}

void Universe::setInputset(Qt3DInput::QInputSettings *inputset)
{
    if(inputset!=input){
        input = inputset;
        addComponent(input);
        emit inputsetChanged();
    }
}

void Universe::setEventSource(QObject *eventSource)
{
    input->setEventSource(eventSource);
}

Qt3DInput::QInputSettings *Universe::inputset()
{
    return input;
}

void Universe::animator()
{
    day += 0.2;
    if(sceneRoot != nullptr)
        sceneRoot->animate(day);
}



