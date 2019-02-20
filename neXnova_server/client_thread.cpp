#include "client_thread.h"

client_thread::client_thread(qintptr descriptor, server *serverptr) : QThread(),copy(descriptor)
  ,serverptr(serverptr)
{
    name = "";
    singin = false;
}

client_thread::client_thread(const client_thread &copy): QThread(),copy(copy.getDescriptor())
  ,serverptr(copy.getServerPtr())
{
    qDebug() << "Client_thread copy constructor "<<endl;

    name = copy.getName();
    singin = copy.getState();
    avatar = copy.getAvatar();
    sql_id = copy.getSqlId();
    handler = copy.getHandler();
}

client_thread::client_thread():QThread()
{
    qCritical() << "Invalid costructor called "<<endl;
}

client_thread::~client_thread()
{
    qDebug() << "Destroy everythink "<<endl;
}

QString client_thread::getName() const
{
    return name;
}

QString client_thread::getAvatar() const
{
    qInfo() << "Sup avatar of "<<avatar.length()<<" bytes "<<endl;
    return avatar;
}

QTcpSocket *client_thread::getHandler() const
{
    return handler;
}

int client_thread::getSqlId() const
{
    return sql_id;
}

bool client_thread::getState() const
{
    return singin;
}

server *client_thread::getServerPtr() const
{
    return serverptr;
}

qintptr client_thread::getDescriptor() const
{
    return copy;
}

void client_thread::run()
{
    qDebug() << "started client thread "<<QThread::currentThreadId()<<endl;
    handler = new QTcpSocket();
    handler->setSocketDescriptor(copy);

    QSslConfiguration sslConfiguration;
    QFile certFile(QStringLiteral("/home/hoax/SSL/cacert.pem"));
    QFile keyFile(QStringLiteral("/home/hoax/SSL/privkey.pem"));
    certFile.open(QIODevice::ReadOnly);
    keyFile.open(QIODevice::ReadOnly);
    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    certFile.close();
    keyFile.close();
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(sslKey);
    sslConfiguration.setProtocol(QSsl::TlsV1SslV3);

    connect(handler,&QTcpSocket::disconnected,this,&client_thread::disconnected,Qt::DirectConnection);
    connect(handler,&QTcpSocket::readyRead,this,&client_thread::serve,Qt::DirectConnection);

    connect(serverptr,&server::serverMsg,this,&client_thread::serverMsgSender);

    //handler->setSslConfiguration(sslConfiguration);
    //handler->startServerEncryption();

    qInfo() << "Incomming connection from " << handler->peerAddress()
            << " "<<handler->peerName() << endl;

    exec();
}

bool client_thread::authenticate(const QString user, const QString pass)
{

    QSqlQuery query;
    if(!query.exec(QString("SELECT password FROM users WHERE username = '%1';").arg(user))){
        qCritical() <<"Auth failed: "<< query.lastError().text() << endl;
        singin = false;
        return false;
    }

    while (query.next()) {
        if(query.value(0).toString() == pass){
        singin = true;
        name = user;
        qInfo() << "User "<<user<<"authenticated "<<endl;


        QSqlQuery avatarquery(QString("SELECT avatar,id FROM users WHERE username = '%1'").arg(user));
        if(!avatarquery.exec())
            qWarning() << "Unable to cach avatar for "<<user<<" Cause: "<<avatarquery.lastError().text()
                       << endl;
        while (avatarquery.next()) {
            avatar = avatarquery.value(0).toString();
            sql_id = avatarquery.value(1).toInt();
            qCritical() <<"string "<<avatar.length()<<endl;
        }
        Q_FOREACH(uint ss,listUserSolarSystems(user)){
            loadSolarSystem(ss);
        }
        return true;
        }
        else
            qCritical()<<"Pass"<<query.value(0).toString() <<endl<<"Expected "<<pass<<endl;
    }
    qDebug() << "Auth failed for " << user <<endl;
    singin = false;
    return false;


}

void client_thread::createUser(QJsonObject jsobj)
{
    //!TODO anadir baneo despues
    QSqlQuery users("select username,email from users;");
    if(!users.exec())
        qCritical() << "Sql query error "<<users.lastError().text() << endl;
    while (users.next()) {
        if(users.value(0).toString() == jsobj["user"].toString()){
            handler->write("ACCOUNT ERROR CODE 2 ");
            handler->write(FOOTER);
            return;
        }
        if(users.value(1).toString() == jsobj["email"].toString()){
            handler->write("ACCOUNT ERROR CODE 3 ");
            handler->write(FOOTER);
            return;
        }
    }
    //!TODO solar sistem picker funcion and email confirmation
    //!
    //!
    qDebug() <<"Info "<<jsobj<<endl;

    QSqlQuery createuser;
    if(!createuser.exec(QString("INSERT INTO users(id,username,password,email,authlevel,solarsistem,avatar)"
                                " VALUES (0,'%1','%2','%3',1,1,'%4');")
                        .arg(jsobj["user"].toString(),jsobj["pass"].toString(),
                             jsobj["email"].toString(),
                             QString::fromLatin1(jsobj["avatar"].toString().toLatin1().toBase64()))))
        qCritical() << "Sql query error "<<createuser.lastError().text() << endl;


    qDebug()<<"Registred password "<<jsobj["pass"].toString()<<endl;
    qInfo() << "Avatar size "<<jsobj["avatar"].toString().length()<<" length "
            <<jsobj["avatar"].toString().toLatin1().size()<<" Bytes"<<endl;

    QSqlQuery idquery(QString("SELECT id FROM users WHERE username = '%1'")
                      .arg(jsobj["user"].toString()));

    if(!idquery.exec())
        qWarning() << "Unable to find userid " << endl;
    while (idquery.next()) {
        sql_id = idquery.value(0).toInt();
    }

    QSqlQuery mainPlanetCreate;
    if(!mainPlanetCreate.exec(QString("INSERT INTO usersdata(id,name,Raze,Mainplanet)"
                                  " VALUES (%1,'%2','unselected','%3');")
                          .arg(QString::number(sql_id),jsobj["user"].toString(),
                               jsobj["mainplanetname"].toString())))
        qCritical() << "Sql query error "<<mainPlanetCreate.lastError().text() << endl;
    createMainPlanet(jsobj["user"].toString(),jsobj["mainplanetname"].toString());

    handler->write("ACCOUNT CREATED");
    handler->write(FOOTER);
}

void client_thread::defineRaze(QString user, QString raze)
{
    QSqlQuery defineUserRaze(QString("UPDATE usersdata SET Raze = '%1' WHERE name = '%2';")
                             .arg(raze,user));
    if(!defineUserRaze.exec())
        qCritical() << "Sql error "<<defineUserRaze.lastError().text() << endl;
}

QVector<uint> client_thread::listUserSolarSystems(QString user)
{
    QVector<uint> solarsystems;
    QSqlQuery query;
    if(!query.exec(QString("SELECT solarsystem FROM planets WHERE user = '%1'").arg(user))){
        QString errorstr("Error searching planets for user ");
        errorstr+=user;
        errorstr+=" BECAUSE ";
        errorstr+=query.lastError().text();
        errorstr+='\n';
        qFatal(errorstr.toLatin1());
        return solarsystems;
    }
    while (query.next()) {
       solarsystems.append(query.value("solarsystem").toUInt());
    }
    qDebug() << solarsystems.length() <<" solarsystems founded "<<endl;
    return solarsystems;
}

void client_thread::loadSolarSystem(uint id)
{
    QJsonObject sun = QJsonObject();

    QJsonArray moons,planets,rings;

    QSqlQuery sunquery,planetsquery,moonsquery,ringsquery,spacestationsquery;
    //!Sun
    //!
    if(!sunquery.exec(QString("SELECT name,radius,x,y,z,pitch,jaw,"
                           "roll,colorr,colorg,colorb,"
                           "intensity,diffuseTexture"
                           " FROM suns WHERE id = %1;").arg(id)))
        qCritical() << "SQL error "<<sunquery.lastError().text() << endl;

    while (sunquery.next()) {
        sun["id"]=QString("%1+0+0+0").arg(id);
        sun["name"]=sunquery.value("name").toString();
        sun["radius"]=sunquery.value("radius").toFloat();

        sun["x"]=sunquery.value("x").toFloat();
        sun["y"]=sunquery.value("y").toFloat();
        sun["z"]=sunquery.value("z").toFloat();

        sun["pitch"]=sunquery.value("pitch").toFloat();
        sun["jaw"]=sunquery.value("jaw").toFloat();
        sun["roll"]=sunquery.value("roll").toFloat();

        sun["colorr"]=sunquery.value("colorr").toInt();
        sun["colorg"]=sunquery.value("colorg").toInt();
        sun["colorb"]=sunquery.value("colorb").toInt();

        sun["intensity"]=sunquery.value("intensity").toFloat();
        sun["diffuseTexture"]=sunquery.value("diffuseTexture").toString();

        sun["rotx"] = double( (qrand() % 1000)/20000.0);
        sun["roty"] = double( (qrand() % 1000)/20000.0);
        sun["rotz"] = double( (qrand() % 1000)/20000.0);
    }
    //!planet
    //! TODO add planetary structures
    if(!planetsquery.exec(QString("SELECT id,user,name,type,campus,temperature,"
                                  "radius,x,y,z,ambientr,ambientg,ambientb,diffuser,"
                                  "diffuseg,diffuseb,specularr,specularg,specularb,"
                                  "shininess,diffuseText,specularTexture,normalTexture,"
                                  "N1,N2,I1,I2,W1,W2,A1,A2,E1,E2,M1,M2"
                                  " FROM planets WHERE solarsystem = %1;").arg(QString::number(id))))
        qCritical() << "SQL error "<<planetsquery.lastError().text() << endl;

    while (planetsquery.next()) {

        QJsonObject planet = QJsonObject();
        uint planetid = planetsquery.value("id").toUInt();
        planet["id"]=QString("%1+%2+0+0").arg(QString::number(id),
                                              QString::number(planetid));
        planet["name"]=planetsquery.value("name").toString();
        planet["user"]=planetsquery.value("user").toString();
        planet["type"]=planetsquery.value("type").toInt();
        planet["campus"]=planetsquery.value("campus").toInt();
        planet["temperature"]=planetsquery.value("temperature").toFloat();
        planet["radius"]=planetsquery.value("radius").toFloat();

        planet["x"]=planetsquery.value("x").toFloat();
        planet["y"]=planetsquery.value("y").toFloat();
        planet["z"]=planetsquery.value("z").toFloat();

        planet["ambientr"]=planetsquery.value("ambientr").toInt();
        planet["ambientg"]=planetsquery.value("ambientg").toInt();
        planet["ambientb"]=planetsquery.value("ambientb").toInt();

        planet["diffuser"]=planetsquery.value("diffuser").toInt();
        planet["diffuseg"]=planetsquery.value("diffuseg").toInt();
        planet["diffuseb"]=planetsquery.value("diffuseb").toInt();

        planet["specularr"]=planetsquery.value("specularr").toInt();
        planet["specularg"]=planetsquery.value("specularg").toInt();
        planet["specularb"]=planetsquery.value("specularb").toInt();

        planet["shininess"]=planetsquery.value("shininess").toFloat();

        planet["normalTexture"]=planetsquery.value("normalTexture").toString();
        planet["specularTexture"]=planetsquery.value("specularTexture").toString();
        planet["diffuseText"]=planetsquery.value("diffuseText").toString();

        planet["N1"] = planetsquery.value("N1").toFloat();
        planet["N2"] = planetsquery.value("N2").toFloat();

        planet["I1"] = planetsquery.value("I1").toFloat();
        planet["I2"] = planetsquery.value("I2").toFloat();

        planet["W1"] = planetsquery.value("W1").toFloat();
        planet["W2"] = planetsquery.value("W2").toFloat();

        planet["A1"] = planetsquery.value("A1").toFloat();
        planet["A2"] = planetsquery.value("A2").toFloat();

        planet["E1"] = planetsquery.value("E1").toFloat();
        planet["E2"] = planetsquery.value("E2").toFloat();

        planet["M1"] = planetsquery.value("M1").toFloat();
        planet["M2"] = planetsquery.value("M2").toFloat();

        planet["rotx"] = 0.0;//double( (qrand() % 1000)/20000.0);
        planet["roty"] = 0.0;//double( (qrand() % 1000)/20000.0);
        planet["rotz"] = 0.0;//double( (qrand() % 1000)/20000.0);

        //!moons

        moons = QJsonArray();

        qDebug() << "executing moons query"<<endl;

        if(!moonsquery.exec(QString("SELECT id,name,description,type,radius,"
                                    "ambientr,ambientg,ambientb,specularr,specularg,"
                                    "specularb,shininess,diffuseTexture,normalTexture,"
                                    "N1,N2,I1,I2,W1,W2,A1,A2,E1,E2"
                                    " FROM moons WHERE planetid = %1;")
                            .arg(QString::number(planetid))))
            qCritical() << "SQL error "<<moonsquery.lastError().text() << endl;

        qDebug() << "Done ... "<<moonsquery.size()<<" results"<<endl;

        while (moonsquery.next()) {
            QJsonObject moon;

            QString strcompose = QString("%1+%2+%3+0").arg(id).arg(planetid)
                                                      .arg(moonsquery.value("id").toInt());
            qDebug() << "moon id "<<strcompose<<endl;
            moon["id"] = strcompose;
            moon["name"]=moonsquery.value("name").toString();
            moon["description"]=moonsquery.value("description").toString();
            moon["type"]=moonsquery.value("type").toInt();
            moon["radius"]=moonsquery.value("radius").toFloat();

            moon["ambientr"]=moonsquery.value("ambientr").toInt();
            moon["ambientg"]=moonsquery.value("ambientg").toInt();
            moon["ambientb"]=moonsquery.value("ambientb").toInt();

            moon["specularr"]=moonsquery.value("specularr").toInt();
            moon["specularg"]=moonsquery.value("specularg").toInt();
            moon["specularb"]=moonsquery.value("specularb").toInt();

            moon["shininess"]=moonsquery.value("shininess").toFloat();

            moon["normalTexture"]=moonsquery.value("normalTexture").toString();
            moon["diffuseTexture"]=moonsquery.value("diffuseTexture").toString();

            moon["N1"] = moonsquery.value("N1").toFloat();
            moon["N2"] = moonsquery.value("N2").toFloat();

            moon["I1"] = moonsquery.value("I1").toFloat();
            moon["I2"] = moonsquery.value("I2").toFloat();

            moon["W1"] = moonsquery.value("W1").toFloat();
            moon["W2"] = moonsquery.value("W2").toFloat();

            moon["A1"] = moonsquery.value("A1").toFloat();
            moon["A2"] = moonsquery.value("A2").toFloat();

            moon["E1"] = moonsquery.value("E1").toFloat();
            moon["E2"] = moonsquery.value("E2").toFloat();

            moons.append(moon);

            qDebug() <<"adding moon "<<moon<<endl;
        }

        planet["moons"] = moons;
        //!Rings
        /*
        rings = QJsonArray();
        if(!ringsquery.exec(QString("SELECT id,type,name,scale,opacity"
                                    " FROM rings WHERE planetid = %1;")
                                    .arg(planet["id"].toInt())))
            qCritical() << "SQL error "<<ringsquery.lastError().text() << endl;

        while (ringsquery.next()) {
            QJsonObject ring;

            ring["id"]=QString("%1+%2+%3+0").arg(id,planetsquery.value("id").toInt(),
                                       ringsquery.value("id").toInt());

            ring["type"]=ringsquery.value("type").toInt();
            ring["name"]=ringsquery.value("name").toString();
            ring["scale"]=ringsquery.value("scale").toFloat();
            ring["opacity"]=ringsquery.value("opacity").toFloat();

            rings.append(ring);
        }
        */
        planet["rings"] = QJsonObject();

        qDebug() <<"Planet "<<planet["name"].toString()<<" composed"<<endl;

        planets.append(planet);
    }

    sun["planets"]=planets;

    QJsonObject solarsystemdata;
    QJsonArray suns;
    suns.append(sun);
    solarsystemdata["stars"]=suns;

    //!TODO json cache layer
    //!

    qDebug() <<solarsystemdata << endl;

    QJsonDocument doc(solarsystemdata);

    handler->write("UNIVERSE DATA ");
    handler->write(doc.toBinaryData());
    handler->write(FOOTER);

    qDebug() << "Solar system "<<id<<" sended "<<endl;

}

void client_thread::generateRandomSolarSystem(uint id)
{
    qDebug() << "Generating solar system "<<id;

    uint planetsNo = 1 + qrand() % 10;
    qDebug() << "Planets number "<<planetsNo<<endl;

    QVector<float> sspos = generateSolarSysPos();
    float sunrad = generateSunRadius();

    QSqlQuery createSun;

    if(!createSun.exec(QString("INSERT INTO suns(id,name,radius,x,y,z,pitch,jaw,"
                               "roll,colorr,colorg,colorb,intensity,diffuseTexture)"
                               " VALUES (%1,'%2',%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,'%14');")
                   .arg(QString::number(id),
                        generateSunName(),
                        QString::number(sunrad),
                        QString::number(sspos[0]),
                        QString::number(sspos[1]),
                        QString::number(sspos[2]),
                        QString::number(0.0),
                        QString::number(0.0),
                        QString::number(0.0))
                   .arg(QString::number(135 + qrand() % 121 ),
                        QString::number(200 + qrand() % 56 ),
                        QString::number(100 + qrand() % 156 ),
                        QString::number((200 + qrand() % 1000)/500.0),
                        QString("qrc:/assets/images/solarsystemscope/sunmap.jpg")))){
        qCritical() << "Error creating sun "<<createSun.lastError().text() << endl;
        return;
    }

    QSqlQuery insertPlanet;


    for(int i = 0;i<planetsNo;i++){

         QVector<float> ppos = generatePlanetPos(sspos,sunrad);

        if(!insertPlanet.exec(QString(  "INSERT INTO planets(id,solarsystem,user,name,type,campus,"
                                        "temperature,radius,x,y,z,ambientr,ambientg,ambientb,diffuser,diffuseg,"
                                        "diffuseb,specularr,specularg,specularb,shininess,diffuseText,"
                                        "specularTexture,normalTexture,N1,N2,I1,I2,W1,W2,A1,A2,E1,E2,M1,M2)"
                                        " VALUES (0,%1,'%2','%3',%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,"
                                        "%15,%16,%17,%18,%19,%20,'%21','%22','%23',%24,%25,%26,%27,%28,%29,%30,"
                                        "%31,%32,%33);")
                          .arg(QString::number(id),
                               QString(""),
                               generatePlanetName(),
                               QString::number(qrand() % 18),
                               QString::number(50 + qrand() % 500),
                               QString::number(qrand() % 100),
                               QString::number(generatePlanetRadius()),
                               QString::number(ppos[0]),
                               QString::number(ppos[1]))
                          .arg(QString::number(ppos[2]),
                               QString::number(qrand() % 256),
                               QString::number(qrand() % 256),
                               QString::number(qrand() % 256),
                               QString::number(qrand() % 256),
                               QString::number(qrand() % 256),
                               QString::number(qrand() % 256))
                          .arg(QString::number(qrand() % 256),
                               QString::number(qrand() % 256),
                               QString::number(qrand() % 256),
                               QString::number(qrand() % 8),
                               QString("qrc:/assets/images/solarsystemscope/earthmap2k.jpg"),
                               QString("qrc:/assets/images/solarsystemscope/earthspec2k.jpg"),
                               QString("qrc:/assets/images/solarsystemscope/earthnormal2k.jpg"))
                          .arg(QString::number(0.0),
                               QString::number(0.0),
                               QString::number(0.0),
                               QString::number(0.0),
                               QString::number(0.0),
                               QString::number(0.0),
                               QString::number(0.0),
                               QString::number(0.0))
                          .arg(QString::number(0.0),
                               QString::number(0.0),
                               QString::number(0.0),
                               QString::number(0.0))))
            qCritical() << "Error inserting planet "<<insertPlanet.lastError().text();

        uint moonsno = qrand() % 9;
        if(moonsno > 6)
            moonsno -= 5;
        else
            moonsno = 0;
        QSqlQuery moonsquery;

        int planetid = 0;
        QSqlQuery counter(QString("SELECT id FROM plantes WHERE solarsystem = %1;").arg(id));
        if(!counter.exec())
            qCritical() << "Error counting "<<counter.lastError().text();

        while (counter.next()) {
           planetid = int(planetid > counter.value("id").toInt() ? planetid : counter.value("id").toInt());
        }

        for(int i=0;i<moonsno;i++){
            QJsonObject rotParam = generateRootParam();

            moonsquery.exec(QString("INSERT INTO moons(planetid,id,name,description,type,"
                                    "radius,N1,N2,I1,I2,W1,W2,A1,A2,E1,E2,ambientr,ambientg,ambientb,"
                                    "specularr,specularg,specularb,shininess,diffuseTexture,"
                                    "normalTexture,metadata) "
                                    "VALUES (%1,0,'%2','%3',%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,"
                                    "%15,%16,%17,%18,%19,%20,%21,%22,'%23','%24','%25');")
                            .arg(QString::number(planetid),
                                 generateMoonName(),
                                 QString("description"),
                                 QString::number(qrand() % 5),
                                 QString::number(generateMoonRadius()))
                            .arg(   QString::number(rotParam["N1"].toDouble()),
                                    QString::number(rotParam["N2"].toDouble()),
                                    QString::number(rotParam["I1"].toDouble()),
                                    QString::number(rotParam["I2"].toDouble()),
                                    QString::number(rotParam["W1"].toDouble()),
                                    QString::number(rotParam["W2"].toDouble()),
                                    QString::number(rotParam["A1"].toDouble()),
                                    QString::number(rotParam["A2"].toDouble()),
                                    QString::number(rotParam["E1"].toDouble()))
                            .arg(   QString::number(rotParam["E2"].toDouble()),

                                    QString::number(qrand() % 256),
                                    QString::number(qrand() % 256),
                                    QString::number(qrand() % 256),
                                    QString::number(qrand() % 256),
                                    QString::number(qrand() % 256),
                                    QString::number(qrand() % 256))
                            .arg(QString::number((200 + qrand() % 1000)/500.0))
                            .arg("qrc:/assets/images/solarsystemscope/moonmap2k.jpg")
                            .arg("qrc:/assets/images/solarsystemscope/moonnormal2k.jpg")
                            .arg("Metamethod"));
        }

    }


}

QString client_thread::generateSunName() const
{
    QStringList posib_sun_names;
    QFile posib_sun_names_file("posib_sun_names_file.csv");
    if(!posib_sun_names_file.open(QIODevice::ReadOnly))
        qCritical()<< "Error opening suns names "
                   << posib_sun_names_file.errorString()
                   << endl;
    QByteArray data = posib_sun_names_file.readAll();
    Q_FOREACH(QByteArray value,data.split(','))
        posib_sun_names << QString::fromLatin1(value);

    return posib_sun_names[qrand() % posib_sun_names.count()];
}

QString client_thread::generatePlanetName() const
{
    QStringList posib_planet_names;
    QFile posib_planet_names_file("posib_planet_names_file.csv");
    if(!posib_planet_names_file.open(QIODevice::ReadOnly))
        qCritical()<< "Error opening suns names "
                   << posib_planet_names_file.errorString()
                   << endl;
    QByteArray data = posib_planet_names_file.readAll();
    Q_FOREACH(QByteArray value,data.split(','))
        posib_planet_names << QString::fromLatin1(value);

    return posib_planet_names[qrand() % posib_planet_names.count()];
}

QString client_thread::generateMoonName() const
{
    return QString("Moon");
}

QJsonObject client_thread::generateRootParam() const
{
    return QJsonObject();
}

float client_thread::generateSunRadius() const
{
    int preradius = qrand() % 1000;
    preradius += 500;
    qDebug() << "Generated sun radius "<<preradius / 100.0<<endl;
    return preradius / 100.0;
}

float client_thread::generateMoonRadius(float planetRadius) const
{
    return 2.5;
}

float client_thread::generatePlanetRadius() const
{
    return 2.5 + (qrand() % 100)/10.1;
}

const QVector<float> client_thread::generateSolarSysPos() const
{
    float x,y,z;
    //! TODO upgrade
    x = qrand() % (UNIVERSE_RADIUS * 100);
    x = x / UNIVERSE_RADIUS;

    y = qrand() % (UNIVERSE_RADIUS * 100);
    y = y / UNIVERSE_RADIUS;

    z = qrand() % (UNIVERSE_RADIUS * 100);
    z = z / UNIVERSE_RADIUS;

    QVector<float>xyz;
    xyz.clear();
    xyz.append(x);
    xyz.append(y);
    xyz.append(z);

    return xyz;
}

const QVector<float> client_thread::generatePlanetPos(QVector<float> sunpos, float sunradius) const
{
    float planetx,planety,planetz;
    planetx = sunpos[0] + qrand() % (SS_RADIUS * 2) - SS_RADIUS;
    if(abs(planetx - sunpos[0]) < sunradius)
        planetx = generatePlanetPos(sunpos,sunradius)[0];

    planety = sunpos[1] + qrand() % (SS_RADIUS * 2) - SS_RADIUS;
    if(abs(planety - sunpos[1]) < sunradius)
        planety = generatePlanetPos(sunpos,sunradius)[1];

    planetz = sunpos[2] + qrand() % (SS_RADIUS * 2) - SS_RADIUS;
    if(abs(planetz - sunpos[2]) < sunradius)
        planetz = generatePlanetPos(sunpos,sunradius)[2];


    QVector<float>pp;
    pp.append(planetx);
    pp.append(planety);
    pp.append(planetz);
    qDebug() << "Generating pp "<<pp<<endl;

    return pp;

}

void client_thread::createMainPlanet(QString user,QString Name)
{
    qsrand(QTime::currentTime().msec());

    if(qrand() < (std::numeric_limits<int>::max()/3))
        generateRandomSolarSystem();
    QSqlQuery planets("SELECT id,user FROM planets;");
    if(!planets.exec())
        qCritical() << "Sql Error "<<planets.lastError().text()<<endl;

    QList<uint>avalibles;

    while (planets.next()) {
       if(planets.value("user").toString().isEmpty())
           avalibles << planets.value("id").toInt();
    }
    if(avalibles.count() < 15){
        generateRandomSolarSystem();
        return createMainPlanet(user,Name);
    }
    uint selected = avalibles[qrand() % avalibles.count()];
    QSqlQuery creator(QString("UPDATE planets SET user = '%1',name = '%2' WHERE id = '%3';")
                      .arg(user,Name,QString::number(selected)));
    if(!creator.exec())
        qCritical() << "Sql Error "<<creator.lastError().text()<<endl;
}

uint client_thread::lastStarGeneratedPlusOne()
{
    QSqlQuery enumerator("SELECT id FROM suns;");
    if(!enumerator.exec())
        qCritical() << "Sql Error "<<enumerator.lastError().text()<<endl;
    qDebug() << enumerator.size() << " Suns founded "<<endl;
    int nextsun = enumerator.size() + 1;
    return nextsun;
}

QJsonObject client_thread::connectedUsersInfo()
{
    QJsonObject list = QJsonObject();
    const QString cname = getName();
    QMetaObject::invokeMethod(serverptr,"connectedClients",Qt::BlockingQueuedConnection,
                              Q_RETURN_ARG(QJsonObject,list),
                              Q_ARG(QString,cname));

    qDebug() << "Info "<<list<<endl;
    return list;
}

void client_thread::disconnected()
{
    singin = false;
    QMetaObject::invokeMethod(serverptr,"removeClientFromStack",
                              Qt::BlockingQueuedConnection,Q_ARG(client_thread*,this));
    connect(this,&client_thread::finished,&client_thread::deleteLater);
    qCritical() << "Connection closed !!! ..." << endl;    
}

void client_thread::serve()
{
    buffer.append(handler->readAll());
    while (buffer.contains(FOOTER)) {
        qDebug() << "serving client thread "<<QThread::currentThreadId()<<endl;

        //qDebug() << "Buffer Content "<<buffer << endl;

        int pos = buffer.indexOf(FOOTER);
        QByteArray command = buffer.left(pos);
        buffer.remove(0,pos+9);
        //qDebug() << "Processing command "<<command << endl;

        if(command.contains("MAKELOGIN")){
           command.remove(0,9);
           QJsonDocument doc = QJsonDocument::fromBinaryData(command);
           QJsonObject authdata = doc.object();
            if(authenticate(authdata["user"].toString(),authdata["password"].toString()))
                handler->write(QByteArray("LOGIN OK"));
            else
                handler->write("LOGIN BAD");
            handler->write(FOOTER);
        }
        else if(command.contains("PING")){
            QByteArray pong;
            pong.append("PONG ");
            pong.insert(pong.size(),18 - pong.size(),'F');
            pong.append(FOOTER);
            handler->write(pong);
        }
        else if(command.contains("ACOUNTCREATE ")){
            command.remove(0,13);

            QJsonDocument doc = QJsonDocument::fromBinaryData(command);
            QJsonObject jsobj = doc.object();
            createUser(jsobj);

        }
        else if(command.contains("CHAT ")){
            command.remove(0,5);
            QJsonDocument doc = QJsonDocument::fromBinaryData(command);
            //! Add some extra info
            QJsonObject obj = doc.object();
            obj["sender"]=name;
            obj["avatar"]=getAvatar();

            qCritical() << "Chat command "<<endl;

            QJsonDocument doctop(obj);

            QByteArray newmsg;
            newmsg.append("CHAT ");
            newmsg.append(doctop.toBinaryData());
            newmsg.append(FOOTER);

            QMetaObject::invokeMethod(serverptr,"sendMsg",Qt::BlockingQueuedConnection
                                      ,Q_ARG(QString,obj["dest"].toString())
                                      ,Q_ARG(QByteArray,newmsg));

        }
        else if(command.contains("CLIENTS LIST ")){
            QJsonObject obj = connectedUsersInfo();

            qDebug() << "Sending clients info "<<obj<<endl;

            QJsonDocument doc(obj);
            QByteArray resp;
            resp.append("CLIENTS LIST RESP ");
            resp.append(doc.toBinaryData());
            resp.append(FOOTER);
            handler->write(resp);
        }
        else if(command.contains("AUDIOCHANNEL ")){
            qDebug() << "Openning audio channel "<<endl;
            command.remove(0,13);
            QJsonDocument doctop = QJsonDocument::fromBinaryData(command);
            QJsonObject dataobj = doctop.object();
            dataobj["sender"] = name;
            dataobj["senderaddress"] = QHostAddress(handler->peerAddress().toIPv4Address())
                    .toString();
            QMetaObject::invokeMethod(serverptr,"audioChannelRele",
                                      Qt::QueuedConnection,
                                      Q_ARG(QJsonObject,dataobj));
        }
        else if(command.contains("DEFINERAZE ")){
            command.remove(0,11);
            qDebug() << "Def raze recived "<<command << endl;
            QList<QByteArray> parts = command.split(' ');
            if(parts.count() < 2)
                qFatal("Error defining raze\n");
            defineRaze( QString::fromLatin1(parts[0]),
                        QString::fromLatin1(parts[2]));
        }
        else if(command.contains("VIDEOCHANNEL TO ")){
            handler->write("VIDEO CHANNEL REQUEST ");
            QJsonObject ip;
            ip["peerip"]="192.168.0.30";
            QJsonDocument doc(ip);
            handler->write(doc.toBinaryData());
            handler->write(FOOTER);
        }
        else
            qFatal(QString("unrecognized %1 \n").arg(QString::fromLatin1(command)).toLatin1().data());


    }
}

void client_thread::serverMsgSender(QByteArray msg)
{
    qDebug() << "writing server msg to "<<name<<" MSG: "<<msg<<endl;
    handler->write(msg);
    handler->waitForBytesWritten();
}

