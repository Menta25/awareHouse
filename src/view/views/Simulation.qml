import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Simulator 1.0
import SimulationMap 1.0

Item {
    id: root

    anchors.fill: parent

    Item {
        id: simulation

        readonly property alias inProgress: play.paused
        readonly property real  aspectRatio: 16/9

        anchors.centerIn: parent
        height: parent.height * 0.7
        width:  height * aspectRatio

        Material.background: Material.primary
        Material.elevation:  6

        onWidthChanged:  simulationGrid.forceLayout()
        onHeightChanged: simulationGrid.forceLayout()

        TableView {
            id: simulationGrid

            property real cellSize: Math.min(simulation.width  / simulation.wareHouseCols - spacing,
                                             simulation.height / simulation.wareHouseRows - spacing)

            columnWidthProvider: function(column) { return cellSize; }
            rowHeightProvider: function(row) { return cellSize; }
            anchors.fill: parent
            anchors.centerIn: parent

            rowSpacing: 1
            columnSpacing: 1

            ScrollBar.horizontal: ScrollBar{}
            ScrollBar.vertical: ScrollBar{}

            delegate: Rectangle {

                id: cell
                implicitWidth: simulationGrid.cellSize
                implicitHeight: simulationGrid.cellSize
                Image {
                    source: model.image
                    anchors.fill: parent
                }
            }

            model: MapTablePresenterModel {
                items: simpresenter.layout.map
            }


            contentX: (contentWidth - parent.width) / 2;
            contentY: (contentHeight - parent.height) / 2;
        }
    }

    Item {
        id: simulationControl

        anchors {
            top: simulation.bottom; bottom: parent.bottom
            horizontalCenter: simulation.horizontalCenter
        }
        width:  simulation.width  * 0.4

        RowLayout {
            id: controlLayout

            anchors.centerIn: parent
            height:           parent.height * 0.65

            MediaControlButton {
                id: play

                property bool paused: true

                Layout.fillWidth:  true
                Layout.fillHeight: true

                icon.source: paused ? "qrc:/play_white.png" : "qrc:/pause_white.png"
                //BUG: when button is at minimal size and clicked -> changed text appears
                //text:        paused ? qsTr("Pause")                : qsTr("Play")

                onClicked: {
                    if(paused)
                        simpresenter.simulationStart()
                    else
                        simpresenter.simulationStop()

                    paused = !paused
                    }
            }

            MediaControlButton {
                id: speed

                property real simulationSpeed: 1

                states: [
                    State {
                        name: "normal"
                        PropertyChanges { target: speed; text: "1×"; simulationSpeed: 1 }
                    },
                    State {
                        name: "double"
                        PropertyChanges { target: speed; text: "2×"; simulationSpeed: 2 }
                    },
                    State {
                        name: "half"
                        PropertyChanges { target: speed; text: "0.5×"; simulationSpeed: 0.5 }
                    }
                ]
                state: "normal"

                Layout.fillWidth:  true
                Layout.fillHeight: true

                font.bold: true
                font.letterSpacing: 0
                font.pixelSize: Math.min(parent.height * 0.35, 30)

                onClicked: {
                    switch (speed.state) {
                    case "normal": speed.state = "double"; simpresenter.setTickRate(TickRate.TWICE); break
                    case "double": speed.state = "half";   simpresenter.setTickRate(TickRate.HALF_SPEED); break
                    case "half":   speed.state = "normal"; simpresenter.setTickRate(TickRate.NORMAL); break
                    }
                }
            }

            MediaControlButton {
                id: reset

                Layout.fillWidth:  true
                Layout.fillHeight: true

                icon.source: "qrc:/replay_white.png"
                //text: "Reset"

                onClicked: simpresenter.reloadWarehouse();
            }
        }
    }
}
