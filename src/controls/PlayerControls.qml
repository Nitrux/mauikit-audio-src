import QtQuick

/**
 *  QtQuick.Item
 *  Placeholder surface reserved for reusable audio playback controls.
 *
 *  This type does not currently expose playback controls or bind to a
 * MediaPlayer. Applications should construct their controls around MediaPlayer
 * directly until this component is implemented.
 */
Item {
    id: root

    Rectangle {
        anchors.fill: root
        color: "green"
    }
}
