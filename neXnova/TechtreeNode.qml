import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Universal 2.1
import CircleImage 1.0

Item {
    id:techtree
    property string icon: ""
    property string iconWB: ""
    property string techname: "Plasma"
    property string description: "Ble ble ble ble"
    property var dependencies : []
    property var dependenciesLevels: [20,20,7]
    property int techlevel: 0
    property int developtime: 100
    property int developProgress: 0
    property bool developing: false
    property real drawcenterx: backbround.x + backbround.width/2
    property real drawcentery: backbround.y + backbround.height/2

    signal developmentfinished
    signal developRequest

    function init(){
        techcrption.generateDepText();
    }


    Rectangle{
        z:0
        anchors.fill: parent
        radius: Math.min(height,width) / 2

        MouseArea{
        anchors.fill: parent
        z:3
        hoverEnabled: true
        onClicked: developRequest()
        onEntered: techcrption.visible = true
        onExited: techcrption.visible = false
        }

        CircleImage {
            id: backbround
            source: icon
            anchors.fill: parent
            antialiasing: true
            disabled: techlevel == 0 ? true : false

            Timer{
                id:timer
                interval: developtime
                running: developing
                repeat: developing
                onTriggered: {
                    developProgress++;
                    console.log(developProgress)
                    canvas.requestPaint();
                    if(developProgress === 1000){
                        developing = false;
                        techlevel++;
                        developmentfinished();
                        canvas.requestPaint();
                    }
                }

            }
        }
        Canvas {
            id: canvas
            anchors.fill: parent
            antialiasing: true
            z:1
            onPaint: {
            var ctx = getContext("2d")
            var gradient = ctx.createConicalGradient(techtree.width / 2,
                                                     techtree.height / 2,
                                                     2*Math.PI)
            gradient.addColorStop(0.0, "#02d5ff");
            gradient.addColorStop(1.0, "#59ff00");

            ctx.lineWidth = parent.width / 20
            ctx.strokeStyle = gradient

            if(developing){
                ctx.beginPath()
                var endAngle = developProgress * 2 * Math.PI/1000.0
                ctx.arc(parent.width / 2 , parent.height / 2,
                parent.width / 2 - ctx.lineWidth / 2,0, endAngle)
                ctx.clearRect(0, 0, width, height)
                ctx.stroke()
            }
            else{
                ctx.strokeStyle = "red"
                ctx.clearRect(0, 0, width, height)
                for (var i = 0; i < techlevel*2; i++){
                    if(i%2===1){
                        ctx.beginPath()
                        console.log("creating arc " + i)
                        ctx.arc(parent.width / 2 , parent.height / 2,
                        parent.width / 2 - ctx.lineWidth / 2
                        ,i*2*Math.PI/50,(i+1)*2*Math.PI/50)
                        ctx.stroke()
                 }

                        }

                    }

                    }
                }
            }
    Text{
        id:techcrption
        visible: false
        anchors.left: techtree.right
        anchors.top: techtree.bottom
        height: 200
        width: 200
        color: Qt.rgba(1.0,1.0,1.0,0.8)
        textFormat: Text.RichText
        function generateDepText(){
            techcrption.text = techname + "<br>" + description + "<br> <b> Req: </b>"
            for(var i=0;i<dependencies.length;i++){
                techcrption.text += "<br>"
                techcrption.text += "<img width = 20 height = 20 src = "+dependencies[i].icon + "/>"
                techcrption.text += dependencies[i].techname
            }


        }


    }

    Component.onCompleted: init()
}


    //!TODO canvas,lines and dialog over items with info


