import QtQuick 2.0
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Templates 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2

Item {
    signal goBack
    property string username
    Image {
        id: image
        anchors.fill: parent
        source: "assets/tlo.jpg"

        Connections{
            target: CppEngine
            onAccountCreationError:{
            switch(errorcode){
            case 2:
                warningImageUser.opacity = 1.0
                break
            case 3:
                warningImageMail.opacity = 1.0
                break
            }
            }

        }

        TextInput {
            id: id_CreateUser
            x: 280
            y: 105
            width: 80
            height: 20
            color: "#ffffff"
            text: qsTr("User")
            anchors.verticalCenterOffset: -160
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12

            Image {
                id: warningImageUser
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

        }

        TextInput {
            id: id_CreateMail
            x: 280
            y: 150
            width: 80
            height: 20
            color: "#ffffff"
            text: qsTr("Mail")
            anchors.verticalCenterOffset: -96
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12

            Image {
                id: warningImageMail
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
        }

        TextInput {
            id: id_CreatePass
            x: 280
            y: 211
            width: 80
            height: 20
            color: "#ffffff"
            text: qsTr("Pass")
            anchors.verticalCenterOffset: -32
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
            onTextChanged: {id_PassWordStrength.value = text.length/12.0}

            Image {
                id: warningImagePass
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

        }

        TextInput {
            id: id_CreatePassConfirm
            x: 280
            y: 257
            width: 80
            height: 20
            color: "#ffffff"
            text: qsTr("Confirm Pass")
            anchors.verticalCenterOffset: 32
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
        }

        TextInput {
            id: id_CreateMainPlanet
            x: 280
            y: 315
            width: 80
            height: 20
            color: "#ffffff"
            text: qsTr("Main Planet")
            anchors.verticalCenterOffset: 116
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
        }

        CircularGauge {
            id: id_PassWordStrength
            y: 226
            width: 67
            height: 66
            anchors.verticalCenter: id_CreatePass.verticalCenter
            anchors.left: id_CreatePass.right
            anchors.leftMargin: 40
            antialiasing: true
            maximumValue: 1.0
            minimumValue: 0.0
            Rectangle {
                id: rectangle
                anchors.fill: parent
                z:-1
                color: "#000000"
                radius: Math.min(parent.width,parent.height)/2
            }
            Behavior on value {
                NumberAnimation {
                    duration: 1000
                }
            }
        }

        DelayButton {
            id: id_CreateAccept
            x: 280
            y: 371
            delay: 3000
            text: qsTr("Accept")
            anchors.verticalCenterOffset: 200
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            onActivated:{
                if(id_CreatePass.text !== id_CreatePassConfirm.text){
                    console.log("wrong password ")
                    warningImagePass.opacity = 1.0
                }
                else{
                    warningImagePass.opacity = 0.0
                    warningImageMail.opacity = 0.0
                    warningImageUser.opacity = 0.0
                    username = id_CreateUser.text
                    CppEngine.accountCreate(id_CreateUser.text,id_CreatePass.text,
                                            id_CreateMail.text,id_CreateMainPlanet.text,
                                            avatar.source)

                }

            }
        }

        Image {
            id: avatar
            x: 910
            y: 186
            width: 90
            height: 77
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: id_CreateUser.top
            anchors.bottomMargin: 40
            source: "qrc:/gui/accountcreate/kittens.png"

            MouseArea {
                id: mouseArea
                anchors.fill: parent

                FileDialog {
                    id: fileDialog
                    modality: Qt.NonModal
                    title: "Pick your avatar "
                    nameFilters: [ "Image files (*.png *.jpg)", "All files (*)" ]

                    onAccepted: {
                        console.log("Selected avatar : " + fileUrl)
                        avatar.source = fileUrl
                    }
                    onRejected: { console.log("Rejected") }
                }

                onClicked: {
                    fileDialog.open()
                }
            }
        }

        Label {
            id: label
            x: 910
            y: 147
            width: 90
            height: 18
            color: "#52acff"
            text: qsTr("Avatar")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: avatar.top
            anchors.bottomMargin: 5
        }
    }

}
