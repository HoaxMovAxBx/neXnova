import QtQuick 2.9

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtQuick.Window 2.2
import QtQuick.Scene3D 2.0

import Universe 1.0
import Planet 1.0
import Star 1.0
import Skybox 1.0
import Meteor 1.0

Scene3D {
    id: scene3d
    entity:verso
    focus: true
    aspects: ["input", "logic","render"]
    cameraAspectRatioMode: Scene3D.AutomaticAspectRatio
    Component.onCompleted:{
        scene3d.forceActiveFocus()
        verso.setEventSource(scene3d)
    }
}
