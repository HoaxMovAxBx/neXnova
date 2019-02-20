import QtQuick 2.9
import QtQuick.Controls 2.2
import Qt.labs.settings 1.0

Item {
    id: page
    signal razeselected
    property string user:"BAD"

    Image {
        id: backgroung
        antialiasing: true
        anchors.fill: parent
        source: "qrc:/assets/tlo.jpg"
        mipmap: true
        cache: true
    }

    StackView {
        id: stackView
        y: 90
        height: 321
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: delayButton.top
        anchors.bottomMargin: 20
        anchors.left: listView.right
        anchors.leftMargin: 5
      }

    Image {
        id: logoimage
        visible: true
        opacity: 0.0
        anchors.right: parent.right
        anchors.rightMargin: 240
        anchors.left: listView.right
        anchors.leftMargin: 220
        anchors.bottom: stackView.top
        anchors.bottomMargin: 4
        anchors.top: parent.top
        anchors.topMargin: 25
        source: ""
        Behavior on opacity{
            NumberAnimation { duration: 600 }
            }
        }

    DelayButton {
        id: delayButton
        y: 417
        width: 100
        height: 31
        delay: 1500
        checked: false
        visible: false
        checkable: false
        enabled: true
        anchors.left: listView.right
        anchors.leftMargin: 200
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20

        onActivated:{
            console.log(text)
            CppEngine.defineRaze(user,text)
            razeselected()
        }

        }

    ListView {
    id: listView
    width: 120
    anchors.left: parent.left
    anchors.leftMargin: 5
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 12
    anchors.top: parent.top
    anchors.topMargin: 12
    currentIndex: -1
    highlightFollowsCurrentItem: false
    highlight: Rectangle {
                width: 120;
                height: 40
                color: "lightsteelblue";
                radius: 5
                y: listView.currentItem.y
                Behavior on y {
                    SpringAnimation {
                        spring: 3
                        damping: 0.2
                        }
                    }
               }


    delegate:ItemDelegate{
        text: name
        highlighted: ListView.isCurrentItem
        onClicked: {
            listView.currentIndex = index
            stackView.push(qmlfile)
            logoimage.source = logo
            delayButton.text = "Go " + name
            logoimage.opacity = 1.0
            if(delayButton.visible == false){
                delayButton.visible = true
                    }
                }
            }

    model: RazeModel
    }
}



