import QtQuick 2.9
import QtQuick.Particles 2.0

Rectangle {
    id: root
    anchors.fill: parent
    color: "black"
    Image {
        anchors.fill: parent
        source: "file:///media/hoax/h0ax/Work/neXnova/neXnova/assets/tlo.jpg"
    }
    Rectangle{
        id : center
        height: 50
        width: 50
        anchors.centerIn: parent
        color: Qt.rgba(1,0,0,0.5)
    }
    ParticleSystem {
        id: particles
        anchors.fill: parent

        Age{
            z:1
            id:killafect
            system: parent
            lifeLeft: 0
            shape: EllipseShape{
                fill: center
            }
        }

        ImageParticle {
            groups: ["center"]
            anchors.fill: parent
            source: "file:///media/hoax/h0ax/Work/neXnova/neXnova/assets/star.png"
            colorVariation: 0.2
            color: "#009999FF"


        }

        Emitter {
            anchors.fill: parent
            group: "center"
            emitRate: 100
            lifeSpan: 4000
            size: 15
            sizeVariation: 2
            endSize: 0
            //! [0]
            //shape: EllipseShape {fill: false}
            velocity: TargetDirection {
                id:speed1
                targetX: root.width/2
                targetY: root.height/2
                proportionalMagnitude: true
                magnitude: 1.0
                Behavior on magnitude{
                    PropertyAnimation{  easing.type: Easing.InQuad;
                                        duration: 25000
                                        target: magnitude
                                      }
                }
            }

            //! [0]
        }
    }
    Component.onCompleted:{ speed1.magnitude = -1.0
                            }
}
