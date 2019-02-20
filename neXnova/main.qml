import QtQuick 2.9
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.2
import QtQuick.Scene3D 2.0
import FpsWidget 1.0
import VideoWidget 1.0

Item{
    height: 768
    width: 1024
    id:mainroot

    FpsWidget{
        id:tested
        z:99
        x:0
        y:0
        width: 50
        height: 50
        visible: true
        fpsCalculator: fpscalc

    }

    /*VideoWidget{
                id:videodisplay
                framesource: VChat
                x:50
                y:50
                width: 200
                height: 200
                z:12
                visible: true
            }*/


    StackView {
        id: stackview
        anchors.fill: parent
        z:1

        Acountcreate{
            id:acountcreator
        }

        Gamescreen{
            visible: false
            id:id_gamescreen
        }
        Razeselector{
            id:id_razeselector
            onRazeselected: {
                stackview.pop()
                stackview.pop()
            }
        }

        initialItem:Authenticator{
            id:id_authenticator
            onAccountcreate: {
                stackview.push(acountcreator)
            }
            onSingin:{
                stackview.push(id_gamescreen)
                id_gamescreen.visible = true
            }

        }
        Connections{
            target: acountcreator
            onGoBack:{stackview.replace(id_authenticator)}
        }
        Connections{
            target: CppEngine
            onAccountCreatedSuccessfully:{
                id_razeselector.user = acountcreator.username
                stackview.push(id_razeselector)
            }
        }
        Connections{
            target: id_razeselector
            onRazeselected:{
                stackview.replace(id_authenticator)
            }
        }

        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 200
            }
        }
        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 200
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 200
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 200
            }
        }
    }

}
