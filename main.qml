import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 1200
    height: 800

    property int margin: 10
    property int _currentRow: -1
    property int _currentCol: -1
    property var _curValue

    TextField {
        id: tf
        x: margin
        y: margin
        width: 200
        height: 40
        placeholderText: "Type name here"
        font.pixelSize: 16
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Button
    {
        y: tf.y
        x: tf.x + tf.width + margin

        width: 60
        height: 40
        text: "Save"
        onClicked: {
            if (_currentRow === -1 || _currentCol === -1) {
                return;
            }

            // index of node to be edited
            let editIndex = treeView.index(_currentRow, _currentCol)

            if (typeof _curValue === "boolean") {
                treeModel.setData(editIndex, tf.text.toLowerCase() === 'true' ? true : false, 1)
                return;
            }

            if (typeof _curValue === "number") {
                // checking for floating point or interger type
                // NOTE - in QML, we use modulus operator to differentiate between number and float
                if (_curValue % 1 !== 0) { // its float
                    treeModel.setData(editIndex, parseFloat(tf.text), 1)
                } else { // its number
                    treeModel.setData(editIndex, parseInt(tf.text), 1)
                }
                return;
            }

            // we are defaulting to string type, we will handle more types later, if needed
            treeModel.setData(editIndex, tf.text, 1)
        }
    }

    TreeView {
        id: treeView
        y: tf.y + tf.height + margin
        x: margin
        width: parent.width
        height: parent.height - tf.height - tf.y - margin
        anchors.margins: 10
        clip: true

        selectionModel: ItemSelectionModel {}

        model: treeModel

        delegate: Item {
            implicitWidth: padding + label.x + label.implicitWidth + padding
            implicitHeight: label.implicitHeight * 1.5

            readonly property real indentation: 20
            readonly property real padding: 5

            // Assigned to by TreeView:
            required property TreeView treeView
            required property bool isTreeNode
            required property bool expanded
            required property bool hasChildren
            required property int depth
            required property int row
            required property int column
            required property bool current

            // Rotate indicator when expanded by the user
            // (requires TreeView to have a selectionModel)
            property Animation indicatorAnimation: NumberAnimation {
                target: indicator
                property: "rotation"
                from: expanded ? 0 : 90
                to: expanded ? 90 : 0
                duration: 100
                easing.type: Easing.OutQuart
            }
            TableView.onPooled: indicatorAnimation.complete()
            TableView.onReused: if (current) indicatorAnimation.start()
            onExpandedChanged: indicator.rotation = expanded ? 90 : 0

            Rectangle {
                id: background
                anchors.fill: parent
                color: row == treeView.currentRow ? palette.highlight : "black"
                opacity: (treeView.alternatingRows && row % 2 !== 0) ? 0.3 : 0.1
            }

            Label {
                id: indicator
                x: padding + (depth * indentation)
                anchors.verticalCenter: parent.verticalCenter
                visible: isTreeNode && hasChildren
                text: "▶"

                TapHandler {
                    onSingleTapped: {
                        let index = treeView.index(row, column)
                        treeView.selectionModel.setCurrentIndex(index, ItemSelectionModel.NoUpdate)
                        treeView.toggleExpanded(row)
                    }
                }
            }

            Label {
                id: label
                x: padding + (isTreeNode ? (depth + 1) * indentation : 0)
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width - padding - x
                clip: true
                text: model.name

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        // we will only set the text for leaf nodes for editing
                        if(!hasChildren) {
                            tf.text = model.value
                            _currentRow = row
                            _currentCol = column
                            _curValue = model.value
                        }
                    }
                }
            }
        }
    }
}
