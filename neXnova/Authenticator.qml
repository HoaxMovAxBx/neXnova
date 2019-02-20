import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Templates 2.2
import QtQml 2.2
import QtQuick.Particles 2.0
import FpsWidget 1.0

Item{
    id: item1
    signal accountcreate
    signal singin

    Image {
        id: image
        z: 0
        anchors.fill: parent
        antialiasing: true
        source: "assets/background2.jpg"
    }

    Connections {
        target: CppEngine
        onAuthFailed:{  warningImage.opacity = 1.0 }
        onAuthSuccess:{ console.log("Authenticated")
                        CppEngine.sfx("sound2")
                        singin()}
    }
    
    Text {
        id: text1
        x: 175
        y: 91
        width: 291
        height: 84
        color: "#fbdf80"
        text: qsTr("<strong>xNovaOne</strong> is a space strategy game with hundreds of players simultaneously try to be the best. All you need to play is a standard web browser (FireFox is recommended)")
        anchors.verticalCenterOffset: -200
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideMiddle
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        horizontalAlignment: Text.AlignHCenter
        textFormat: Text.RichText
        font.pixelSize: 12
    }

    TextInput {
        id: id_userText
        x: 281
        width: 80
        height: 20
        color: "#060202"
        text: "User"
        anchors.top: text1.bottom
        anchors.topMargin: 250
        anchors.horizontalCenter: parent.horizontalCenter
        selectionColor: "#a6a4f7"
        antialiasing: true
        inputMask: ""
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
        onFocusChanged: {if(id_userText.text == "User")
                            id_userText.text = ""}
    }

    TextInput {
        id: id_passText
        x: 281
        width: 80
        height: 20
        text: qsTr("Pass")
        anchors.top: id_userText.bottom
        anchors.topMargin: 35
        anchors.horizontalCenter: parent.horizontalCenter
        echoMode: TextInput.Normal
        inputMask: ""
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
        onFocusChanged: {id_passText.echoMode = TextInput.Password;
                         id_passText.text = "" }

        Image {
            id: warningImage
            anchors.left: parent.right
            height: 20
            anchors.verticalCenter: parent.verticalCenter
            width: 20
            antialiasing: true
            visible: true
            opacity: 0.0
            z: 1
            source: "qrc:/gui/accountcreate/error.svg"
            Behavior on opacity {
                NumberAnimation { duration: 1000 }
            }
        }

        onAccepted: {
            CppEngine.check_passwork(id_userText.text,id_passText.text)
        }

    }

    ParticleSystem {
        id: id_particlesSys
        anchors.fill: parent

        Emitter {

            group: "stars"
            emitRate: 100
            lifeSpan: 100000
            lifeSpanVariation: 90000
            enabled: true
            maximumEmitted: 60
            size: 5
            sizeVariation: 20
            height: parent.height
            width: parent.width
        }

        ImageParticle {
            id: id_starsf
            autoRotation: true
            groups: ["stars"]
            anchors.fill: parent
            source: "qrc:/assets/star.png"

            alpha: 0.3
            alphaVariation: 0.3
            colorVariation: 1

            SequentialAnimation on opacity {
                loops: Animation.Infinite

                NumberAnimation{
                    easing.type: Easing.InQuint
                    from: 0; to: 1; duration: 10000 ;
                    }
                NumberAnimation{
                    from: 1; to: 0; duration: 10000 ;
                }
            }

            Age{
                id:id_ageAffector
                lifeLeft: Math.random() * 10000
            }

        }

    }
    Image {
        id:id_buttonac
        anchors.verticalCenter: label.verticalCenter
        anchors.left: label.right
        anchors.leftMargin: 5
        antialiasing: true
        scale: 0.9
        z: 0
        source: "qrc:/assets/Icons/icon-go.png"
        MouseArea {
            anchors.fill: parent
            z: 1
             onClicked:{ accountcreate();
             console.log("triggered")}
        }



    }
    Label {
       id: label
       width: 108
       height: 28
       color: "#ffffff"
       text: qsTr("Create Acount")
       anchors.verticalCenterOffset: -40
       anchors.right: parent.right
       anchors.rightMargin: 100
       anchors.verticalCenter: text1.verticalCenter
       font.pointSize: 11
    }



}

