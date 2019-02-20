import Planet 1.0
import Star 1.0
import Skybox 1.0
import Meteor 1.0

import QtQuick 2.2
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtQuick.Window 2.2
import QtQuick.Scene3D 2.0

Entity {
    id: root

    Camera {
        id: camera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 45
        aspectRatio: 16/9
        nearPlane : 0.1
        farPlane : 1000.0
        position: Qt.vector3d( 0.0, 0.0, -40.0 )
        upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
        viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
    }

    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                clearColor: "black"
                camera: camera
            }
        },
        InputSettings { }
    ]

    FirstPersonCameraController {camera: camera }
    Skybox{}
    //Star{}
    //Meteor{}
    //Planet{}

}

