import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Particles 2.0



Item {
    id: item1
    Rectangle {
        id: rectt
        width: 175
        radius: 30
        border.width: 3
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 75
        anchors.top: parent.top
        anchors.topMargin: 75
        anchors.left: parent.left
        anchors.leftMargin: 75
        color: Qt.rgba(0,0,0,0)

        Connections{
            target: ChatModel
            onChangedd: console.log("changedd")
        }

        ListView {
            id: listView
            focus: true
            z:5
            anchors.fill: parent
            anchors.bottomMargin: 20
            anchors.topMargin: 20
            anchors.leftMargin: 20
            model: ChatModel                                  
            delegate: Label{
                        id: wrapper
                        text: name
                        color: listView.currentIndex == index ? "red":"white"
                        MouseArea { anchors.fill: parent;
                                    onClicked: listView.currentIndex = index; }
                    states: State {
                        name: "Current"
                        when: wrapper.ListView.isCurrentItem
                        PropertyChanges { target: wrapper; x: 20 }
                    }
                    transitions: Transition {
                        NumberAnimation { properties: "x"; duration: 150 }
                    }

                }


            // Set the highlight delegate. Note we must also set highlightFollowsCurrentItem
            // to false so the highlight delegate can control how the highlight is moved.
            highlightFollowsCurrentItem: false
            highlight:  Rectangle {
                            z: 0
                            width: rectt.width - 15;
                            height: listView.currentItem == null ? 0 : listView.currentItem.height;
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: "#99FF99" }
                                GradientStop { position: 1.0; color: "#88FF88" }
                                }
                            y: listView.currentItem == null ? 0 : listView.currentItem.y;
                            Behavior on y { SpringAnimation { spring: 2; damping: 0.2 } }
                            ImageParticle {
                                anchors.fill: parent
                                system: particles
                                source: "qrc:/assets/blueStar.png"
                                color: "red"
                                clip: true
                                alpha: 1.0
                                }
                }



            ParticleSystem { id: particles }

            Emitter {
                system: particles
                anchors.fill: parent
                emitRate: 0
                lifeSpan: 10000
                size: 24
                sizeVariation: 8
                velocity: AngleDirection { angleVariation: 360; magnitude: 3 }
                maximumEmitted: 10
                startTime: 5000
                Timer { running: true; interval: 10; onTriggered: parent.emitRate = 1; }
            }

            //! [0]
            ImageParticle {
                anchors.fill: parent
                system: particles
                source: "qrc:/assets/greenStar.png"
                alpha: 0.1
                color: "white"
                rotationVariation: 180
                z: -1
            }
            //! [0]
        }

    }


    Switch {
        id: audioswitch
        x: 271
        y: 79
        text: qsTr("Audio")
        onToggled:{ console.log("Audio activated")
                    CppEngine.iniAudioChatTo(listView.currentItem.text)}
    }
    Switch {
        id: videoswitch
        x: 404
        y: 79
        text: qsTr("Video")
        onToggled:{ console.log("Video activated")
                    CppEngine.iniVideoChat(listView.currentItem.text)
        }
    }

    TextEdit {
        id: textEdit
        x: 386
        y: 186
        width: 335
        height: 283
        color: "#ffffff"
        text: qsTr("")
        font.pixelSize: 12
    }

    Button {
        id: button
        x: 470
        text: qsTr("Send")
        anchors.horizontalCenter: textEdit.horizontalCenter
        anchors.top: textEdit.bottom
        anchors.topMargin: 0
        onClicked: {
            CppEngine.sendChatText(listView.currentItem.text,textEdit.text)
            textEdit.clear()
        }
    }



}
