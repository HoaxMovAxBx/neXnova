import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import QtQuick.Controls.Universal 2.2
import QtQuick.Scene3D 2.0
import VideoWidget 1.0

Item{

    id: window
    Universal.theme: Universal.Dark
    focus: true

    Image {
        id: metalimage
        x: 234
        y: 23
        width: 38
        height: 37
        anchors.bottomMargin: 10
        source: "qrc:/assets/requisites/metal_marchand.png"
    }

    Image {
        id: cristalimage
        y: 23
        width: 39
        height: 37
        anchors.left: metallabel.right
        anchors.leftMargin: 20
        anchors.bottomMargin: 10
        source: "qrc:/assets/requisites/crystal_marchand.png"
    }

    Image {
        id: deuteriumimage
        y: 23
        width: 37
        height: 37
        anchors.left: cristallabel.right
        anchors.leftMargin: 20
        anchors.bottomMargin: 10
        source: "qrc:/assets/requisites/deuterium_marchand.png"
    }

    Image {
        id: energyimage
        y: 23
        width: 47
        height: 37
        anchors.left: deuteriumlabel.right
        anchors.leftMargin: 20
        anchors.bottomMargin: 10
        source: "qrc:/assets/requisites/energy_max.png"
    }

    Label {
        id: metallabel
        y: 23
        width: 70
        height: 42
        color: "#ffffff"
        text: qsTr("0")
        anchors.left: metalimage.right
        anchors.leftMargin: 20
        anchors.bottomMargin: 10
    }

    Label {
        id: cristallabel
        y: 23
        width: 63
        height: 42
        color: "#ffffff"
        text: qsTr("0")
        anchors.left: cristalimage.right
        anchors.leftMargin: 20
        anchors.bottomMargin: 10
    }

    Label {
        id: deuteriumlabel
        y: 23
        width: 103
        height: 42
        color: "#ffffff"
        text: qsTr("0")
        anchors.left: deuteriumimage.right
        anchors.leftMargin: 20
        anchors.bottomMargin: 10
    }

    Label {
        id: energylabel
        y: 23
        width: 56
        height: 37
        color: "#ffffff"
        text: qsTr("0")
        anchors.left: energyimage.right
        anchors.leftMargin: 20
        anchors.bottomMargin: 10
    }

    Image {
        id: id_background
        antialiasing: true
        z: -1
        anchors.fill: parent
        source: "assets/tlo.jpg"
    }

    StackView {
        id: stackView
        width: 800
        anchors.top: parent.top
        anchors.topMargin: 100
        anchors.right: chat_flickable.left
        anchors.rightMargin: 25
        anchors.bottom: planetsDelegate.top
        anchors.bottomMargin: 15
        anchors.left: itemDelegate.right
        anchors.leftMargin: 6
    }

        ListView {
            id: planetsDelegate
            y: 654
            height: 100
            spacing: 10
            contentHeight: 80
            contentWidth: 80
            opacity: 0.8
            anchors.right: chat_flickable.left
            anchors.rightMargin: 25
            anchors.left: itemDelegate.right
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 15

            model: BarModel
            orientation: Qt.Horizontal
            delegate: Item{
                Image {
                     source: planeticon
                     fillMode: Image.Stretch
                     height: 90
                     width: 90
                     MouseArea{
                         anchors.fill: parent
                         onClicked:{    console.log("Cliked "+name)
                                        verso.focusPlanet(name) }
                     }
                     Label{
                         text:name
                         font.pixelSize: 22
                         anchors.horizontalCenter: parent.horizontalCenter
                         anchors.verticalCenter: parent.verticalCenter
                     }
                }
            }

        }

        //initialItem: Universe{id:id_universe}

    ListModel {
        id: functionsModel
        ListElement {
            name: "Planets"
            xicon: "qrc:/gui/planets/wasserplanet08.jpg"
            qmlsrc: "qrc:/UniverseQml.qml"
        }
        ListElement {
            name: "Chat"
            xicon: "qrc:/gui/razes/voltra.jpg"
            qmlsrc: "qrc:/Chat.qml"
        }
        ListElement {
            name: "Techno"
            xicon: "qrc:/gui/strc_and_ships/31.png"
            qmlsrc: "qrc:/Techtree.qml"
        }
    }



    ListView {
        id: itemDelegate
        anchors.right: parent.right
        anchors.rightMargin: 900
        anchors.left: parent.left
        anchors.leftMargin: 35
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 125
        spacing : 5
        delegate:ItemDelegate {  
                      text: name
                      width: parent.width
                      height: width
                      onClicked: stackView.push(qmlsrc)
                      Image {
                          anchors.fill: parent
                          source: xicon
                      }
                  }

        model: functionsModel

    }

    Flickable {
        id: chat_flickable
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        interactive: true
        flickDeceleration: 15
        maximumFlickVelocity: 250
        flickableDirection: Flickable.HorizontalFlick
        contentHeight: 250
        z: 1

        Behavior on width{ NumberAnimation {
                duration: 1200
                easing.type: Easing.InOutQuad }

        }

        topMargin: 20
        bottomMargin: 20

        Image {
            id: avatar
            width: 100
            height: 100
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: id_lablename.bottom
            anchors.topMargin: 0
            source: ""
        }

        Label {
            id:id_lablename
            font.pixelSize: 20
            width: parent.width
            color: "#ffffff"
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("No text")
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
        }
        Label {
            id:id_labeltext
            color: "#ffffff"
            wrapMode: Label.WordWrap
            text: qsTr("No text ")
            visible: false
            }

        StackView{
        id:chatcontent
        width: parent.width
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: avatar.bottom
        anchors.topMargin: 0
        initialItem:VideoWidget{
            framesource:VChat
            visible:true
            width: 200
            height: 200
        }
        Component.onCompleted: {
            chatcontent.push(id_labeltext)
            id_labeltext.visible = true
        }
        }
        ScrollIndicator.vertical: ScrollIndicator { }
    }

    Connections{
        target: CppEngine
        onChatTxtMsg:{  chat_flickable.width = 200
                        id_lablename.text = sender
                        avatar.source = "image://prov/" + sender;
                        id_labeltext.text = msg} //(QString sender,QString msg)

       /* VideoWidget{
            id:videodisplay
            framesource: VChat
            visible: false
        }*/

        onVideochannelOpen:{
            chat_flickable.width = 400
            chatcontent.pop()
        }
    }



}
