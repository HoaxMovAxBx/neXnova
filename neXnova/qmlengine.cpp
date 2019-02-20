#include "qmlengine.h"


void qmlengine::check_passwork(QString user,QString pass)
{
    QMetaObject::invokeMethod(conn,"sendAuthMsg",Q_ARG(QString,user),Q_ARG(QString,pass));
}

void qmlengine::accountCreate(QString user, QString pass, QString email, QString mainPlanetName, QString avatar)
{
    QJsonObject data;

    QCryptographicHash hash(QCryptographicHash::Sha3_512);
    hash.addData(pass.toLatin1());
    data["user"]=user;
    data["pass"]= QString::fromLatin1(hash.result().toBase64());
    data["email"]=email;
    data["mainplanetname"]=mainPlanetName;
    QUrl url(avatar);
    QPixmap map;
    if(!map.load(url.toLocalFile()))
        qCritical() << "Error loading file "<<endl;
    QByteArray*mapdata = new QByteArray;

    QBuffer*buffer = new QBuffer(mapdata);
    buffer->open(QBuffer::WriteOnly);

    map = map.scaled(128,128,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    if(!map.save(buffer,"PNG"))
        qCritical()<<"Error serializing "<<endl;

    qInfo() << "Map data size "<<mapdata->size() << endl;

    data["avatar"]=QString::fromLatin1(mapdata->data(),mapdata->size());

    qInfo() <<"String size "<<data["avatar"].toString().length()<<endl;

    qDebug() << "Pass "<<pass<<" encrypted as "<< data["pass"].toString()<<endl;

    QMetaObject::invokeMethod(conn,"accountCreate",Q_ARG(QJsonObject,data));

}

void qmlengine::defineRaze(QString user, QString raze)
{
    QMetaObject::invokeMethod(conn,"defineRaze",
                              Q_ARG(QString,user),
                              Q_ARG(QString,raze));
}

void qmlengine::test_slot()
{
    qDebug()<<"Test slot summoned by "<<QThread::currentThreadId() << endl;
}

void qmlengine::sendChatText(QString dest, QString msg)
{
    QJsonObject jsonmsg;
    jsonmsg["type"]="txt";
    jsonmsg["dest"]=dest;
    jsonmsg["msg"]=msg;
    jsonmsg["avatar"]="invalidterm";
    jsonmsg["sender"]="whoknows";

    QMetaObject::invokeMethod(conn,"sendMsg",
                              Qt::QueuedConnection,
                              Q_ARG(QJsonObject,jsonmsg));

}

void qmlengine::chatMsgClasifier(QJsonObject chatobj)
{

    if(chatobj["type"].toString()=="txt"){
        qDebug() << "text msg recived "<<endl;      
        emit chatTxtMsg(chatobj["sender"].toString(),chatobj["msg"].toString());

    }
    //!TODO audio and video chat

}

void qmlengine::iniAudioChatTo(QString target)
{
    QMetaObject::invokeMethod(sfxengine,"iniaudiochat",
                              Qt::QueuedConnection,Q_ARG(QString,target));
}

void qmlengine::iniVideoChat(QString target)
{
    QMetaObject::invokeMethod(conn,"requestVideoStreamToPeer",
                              Qt::QueuedConnection,
                              Q_ARG(QString,target));
}

void qmlengine::openVideoChannel(QJsonObject info)
{
    if(vchat == nullptr)
        return;

    vchat->reciver(info["peerip"].toString());
    emit videochannelOpen();
}

void qmlengine::sfx(QString name)
{
    QMetaObject::invokeMethod(sfxengine,"effect",
                              Qt::QueuedConnection,
                              Q_ARG(QString,name));
}

void qmlengine::setSurfaceFormat(){
    QSurfaceFormat format;

#ifdef QT_OPENGL_ES_2
    format.setRenderableType(QSurfaceFormat::OpenGLES);
#else
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        format.setVersion(3, 0);
        format.setProfile(QSurfaceFormat::CoreProfile);
    }
#endif
    format.setDepthBufferSize(24);
    format.setSamples(4);
    format.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(format);
}


qmlengine::qmlengine()
{
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
    {
        format.setVersion(3, 0);
        format.setProfile(QSurfaceFormat::CoreProfile);
    }

    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(4);

    settings = new QSettings("/media/hoax/h0ax/Work/neXnova/neXnova/style.conf",QSettings::NativeFormat);

    conn = new connection;
    cache = new JsonCacheLayer(conn);
    cache->start();

    barmodel = new planetbarmodel;


    verso = new Universe;

    connect(cache,&JsonCacheLayer::dataToRender,verso,&Universe::setRenderData);
    connect(cache,&JsonCacheLayer::changedItem,verso,&Universe::entityChanged);
    connect(cache,&JsonCacheLayer::planetBarData,barmodel,&planetbarmodel::insertData);

    sfxengine = new audioengine(conn);
    sfxengine->start();

    razeModels = new RazeSelector;
    razeModels->razeGenerator();

    chatmodel = new ChatClientsModel(conn);

    imgprovider = new AsyncImageProvider;

    window = new QQuickView;
    //window->setFormat(format);

    fpscalc = new fps(this);
    connect(window,&QQuickView::afterRendering,fpscalc,&fps::newFrame);

    engine = window->engine();
    engine->addImageProvider("prov",imgprovider);

    vchat = new videochat;


    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //setSurfaceFormat();

    QStringList styles =  QQuickStyle::availableStyles();
    Q_FOREACH(QString style,styles)
        qDebug() << "Avalible style "<<style;

    QQuickStyle::setStyle("Universal");

    connect(conn,&connection::authresultOk,this,&qmlengine::authSuccess);
    connect(conn,&connection::authresultBad,this,&qmlengine::authFailed);

    connect(conn,&connection::accountCreated,this,&qmlengine::accountCreatedSuccessfully);
    connect(conn,&connection::accountCreationError,this,&qmlengine::accountCreationError);
    connect(conn,&connection::accountEmailVerification,this,&qmlengine::accountWaitingForEmailVerification);

    connect(conn,&connection::pingcompleted,this,&qmlengine::pong);

    connect(conn,&connection::chatMsg,this,&qmlengine::chatMsgClasifier);
    connect(conn,&connection::connectedClientsInfo,chatmodel,&ChatClientsModel::jsonClientsData);

    connect(conn,&connection::chatMsg,imgprovider,&AsyncImageProvider::avatarLoadFromMsg);
    connect(conn,&connection::connectedClientsInfo,imgprovider,&AsyncImageProvider::avatarLoadFromUsersInfo);

    connect(conn,&connection::videochannelrequest,this,&qmlengine::openVideoChannel);

    connect(this,&qmlengine::authSuccess,&qmlengine::test_slot);
    connect(this,&qmlengine::authFailed,&qmlengine::test_slot);


    QMetaObject::invokeMethod(conn,"testConnection",Q_ARG(QString,"localhost"));

    rootContext = engine->rootContext();
    rootContext->setContextProperty("AudioSfx",sfxengine);
    rootContext->setContextProperty("RazeModel",razeModels);
    rootContext->setContextProperty("ChatModel",chatmodel);
    rootContext->setContextProperty("BarModel",barmodel);
    rootContext->setContextProperty("CppEngine",this);
    rootContext->setContextProperty("verso",verso);
    rootContext->setContextProperty("VChat",vchat);
    rootContext->setContextProperty("fpscalc",fpscalc);

    qmlRegisterType<skybox>("Skybox",1,0,"Skybox");
    qmlRegisterType<star>("Star",1,0,"Star");
    qmlRegisterType<planet>("Planet",1,0,"Planet");
    qmlRegisterType<meteor>("Meteor",1,0,"Meteor");
    qmlRegisterType<CircleImage>("CircleImage",1,0,"CircleImage");
    qmlRegisterType<Universe>("Universe",1,0,"Universe");
    qmlRegisterType<fps>("fps",1,0,"fps");
    qmlRegisterType<fpswidget>("FpsWidget",1,0,"FpsWidget");
    qmlRegisterType<videowidget>("VideoWidget",1,0,"VideoWidget");

    window->setSource(QUrl("qrc:/main.qml"));
    window->show();

    qDebug() << "loaded "<<endl;

}
