import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Universal 2.1


Item {


    ListModel {
        id:techmodel
        ListElement {
            name: "Computing"
            ticon: "qrc:/gui/techicons/108.png"
            tdependencies: ["laser","ionic","energy"]
        }
        ListElement {
            name: "Spy"
            ticon: "qrc:/gui/techicons/106.png"
            tdependencies: ["laser","ionic","energy"]
        }
        ListElement {
            name: "Militar"
            ticon: "qrc:/gui/techicons/109.png"
            tdependencies: ["laser","ionic","energy"]
        }
        ListElement {
            name: "Shild"
            ticon: "qrc:/gui/techicons/110.png"
            tdependencies: ["laser","ionic","energy"]
        }
        ListElement {
            name: "Armor"
            ticon: "qrc:/gui/techicons/111.png"
            tdependencies: ["laser","ionic","energy"]
        }
        ListElement {
            name: "Energy"
            ticon: "qrc:/gui/techicons/113.png"
            tdependencies: ["laser","ionic","energy"]
        }
        ListElement {
            name: "Hyperspace"
            ticon: "qrc:/gui/techicons/114.png"
            tdependencies: ["laser","ionic","energy"]
        }
        ListElement {
            name: "Combustion motor"
            ticon: "qrc:/gui/techicons/115.png"
            tdependencies: ["laser","ionic","energy"]
        }
        ListElement {
            name: "Impulse motor"
            ticon: "qrc:/gui/techicons/117.png"
        }
        ListElement {
            name: "Hyperspace motor"
            ticon: "qrc:/gui/techicons/118.png"
        }
        ListElement {
            name: "Laser"
            ticon: "qrc:/gui/techicons/120.png"
        }
        ListElement {
            name: "Ionic"
            ticon: "qrc:/gui/techicons/121.png"
        }
        ListElement {
            name: "Plasma"
            ticon: "qrc:/gui/techicons/122.png"
        }
        ListElement {
            name: "Intergalactic Network"
            ticon: "qrc:/gui/techicons/123.png"
        }
        ListElement {
            name: "Expetition"
            ticon: "qrc:/gui/techicons/124.png"
        }
        ListElement {
            name: "Metal"
            ticon: "qrc:/gui/techicons/131.png"
        }
        ListElement {
            name: "Cristal"
            ticon: "qrc:/gui/techicons/132.png"
        }
        ListElement {
            name: "Deuterium"
            ticon: "qrc:/gui/techicons/133.png"
        }
        ListElement {
            name: "Mater Manipulation"
            ticon: "qrc:/gui/techicons/134.png"
        }
        ListElement {
            name: "Norio"
            ticon: "qrc:/gui/techicons/135.png"
        }
        ListElement {
            name: "Gravition"
            ticon: "qrc:/gui/techicons/199.png"
        }

    }



    PathView {
            anchors.fill: parent
            model: techmodel
            delegate: TechtreeNode{
                techname: name
                icon: ticon
                width: 100
                height: 100
                dependencies: tdependencies
                Component.onCompleted:{

                }
            }
            path: Path {
                id: pathmaker
                startX: 120; startY: 100
                PathQuad { x: 120; y: 25; controlX: 260; controlY: 75 }
                PathQuad { x: 120; y: 100; controlX: -20; controlY: 75 }
            }
        }



/*TechtreeNode{
    id:plasma
    techname:"plasma"
    anchors.top: parent.top
    anchors.left:parent.left
    height: 100
    width: 100
    icon: "qrc:/gui/techicons/122.png"
    dependencies: [laser,ionic,energy]
    developing: true
    techlevel: 10
}

TechtreeNode{
    id:laser
    techname:"laser"
    anchors.top: parent.top
    anchors.right: parent.right
    height: 100
    width: 100
    icon: "qrc:/gui/techicons/120.png"
    dependencies: [plasma,ionic,energy]
    developing: true
    techlevel: 20
}
TechtreeNode{
    id:ionic
    techname:"ionic"
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    height: 100
    width: 100
    icon: "qrc:/gui/techicons/121.png"
    dependencies: [laser,energy,ionic]
    developing: false
    techlevel: 0
}
TechtreeNode{
    id:energy
    techname: "energy"
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    height: 100
    width: 100
    icon: "qrc:/gui/techicons/113.png"
    dependencies: [laser,plasma,ionic]
    developing: true
    techlevel: 0
}
*/

Canvas {
    id:depPaths
    antialiasing: true
    z:-1
    anchors.fill: parent
    onPaint: {
        var ctx = getContext("2d")
        var gradientAct = ctx.createConicalGradient(techtree.width / 2,techtree.height / 2,
                                                 2*Math.PI)
        gradientAct.addColorStop(0.0, "#02d5ff");
        gradientAct.addColorStop(1.0, "#59ff00");
        ctx.lineWidth = parent.width / 30

        for(var j = 0;j<dependencies.length;j++){

            if(dependenciesLevels[j]>dependencies[j].techlevel){
                ctx.strokeStyle = "black"}
            else{
                ctx.strokeStyle = gradientAct}

            console.log("Drawing path to "+dependencies[j].techname)
            ctx.beginPath()
            ctx.moveTo(drawcenterx,
                       drawcentery)
            ctx.lineTo(dependencies[j].drawcenterx,
                       dependencies[j].drawcentery)
            console.log("Line from: "+drawcenterx+';'+drawcentery
                        +" to: "+dependencies[j].drawcenterx
                        +';'+dependencies[j].drawcentery)
            ctx.stroke()
        }

    }

}

    Component.onCompleted:{
        plasma.init()
        laser.init()
        ionic.init()
        energy.init()



    }
}
