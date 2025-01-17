/*
    SPDX-FileCopyrightText: 2013 Sebastian Kügler <sebas@kde.org>
    SPDX-FileCopyrightText: 2014 Martin Gräßlin <mgraesslin@kde.org>
    SPDX-FileCopyrightText: 2016 Kai Uwe Broulik <kde@privat.broulik.de>
    SPDX-FileCopyrightText: 2017 Roman Gilg <subdiff@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

import QtQuick 2.15
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import QtQml.Models 2.2

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 3.0 as PlasmaComponents3

Loader {
    id: toolTipDelegate

    property Item parentTask
    property var rootIndex
    // hasRootIndex is needed to avoid unnecessary reevaluation of model property in DelegateModel,
    // because !!toolTipDelegate.rootIndex will not work as expected.
    readonly property bool hasRootIndex: !!rootIndex

    property string appName
    property int pidParent
    property bool isGroup

    property var windows
    readonly property bool isWin: windows !== undefined

    property variant icon
    property url launcherUrl
    property bool isLauncher
    property bool isMinimizedParent

    // Needed for generateSubtext()
    property string displayParent
    property string genericName
    property var virtualDesktopParent
    property bool isOnAllVirtualDesktopsParent
    property var activitiesParent
    //
    property bool smartLauncherCountVisible
    property int smartLauncherCount

    readonly property bool isVerticalPanel: plasmoid.formFactor === PlasmaCore.Types.Vertical

    // These properties are required to make tooltip interactive when there is a player but no window is present.
    readonly property string mprisSourceName: mpris2Source.sourceNameForLauncherUrl(launcherUrl, pidParent)
    readonly property var playerData: mprisSourceName != "" ? mpris2Source.data[mprisSourceName] : 0
    readonly property bool hasPlayer: !!mprisSourceName && !!playerData

    Binding on Layout.minimumWidth {
        value: implicitWidth
        delayed: true // Prevent early hide of tooltip (BUG439522)
    }
    Layout.maximumWidth: Layout.minimumWidth

    Binding on Layout.minimumHeight {
        value: implicitHeight
        delayed: true // Prevent early hide of tooltip (BUG439522)
    }
    Layout.maximumHeight: Layout.minimumHeight

    LayoutMirroring.enabled: Qt.application.layoutDirection === Qt.RightToLeft
    LayoutMirroring.childrenInherit: true

    property int textWidth: PlasmaCore.Theme.mSize(PlasmaCore.Theme.defaultFont).width * 20

    active: rootIndex !== undefined
    asynchronous: true

    sourceComponent: isGroup ? groupToolTip : singleTooltip

    Component {
        id: singleTooltip

        ToolTipInstance {
            submodelIndex: toolTipDelegate.rootIndex
        }
    }

    Component {
        id: groupToolTip

        PlasmaComponents3.ScrollView {
            // 2 * PlasmaCore.Units.smallSpacing is for the margin of tooltipDialog
            implicitWidth: leftPadding + rightPadding + Math.min(Screen.desktopAvailableWidth - 2 * PlasmaCore.Units.smallSpacing, contentItem.contentItem.childrenRect.width)
            implicitHeight: bottomPadding + Math.min(Screen.desktopAvailableHeight - 2 * PlasmaCore.Units.smallSpacing, contentItem.contentItem.childrenRect.height)

            ListView {
                id: groupToolTipListView

                model: DelegateModel {
                    model: toolTipDelegate.hasRootIndex ? tasksModel : null

                    rootIndex: toolTipDelegate.rootIndex
                    onRootIndexChanged: groupToolTipListView.positionViewAtBeginning() // Fix a visual glitch (when the mouse moves from a tooltip with a moved scrollbar to another tooltip without a scrollbar)

                    delegate: ToolTipInstance {
                        submodelIndex: tasksModel.makeModelIndex(toolTipDelegate.rootIndex.row, index)
                    }
                }

                orientation: isVerticalPanel ? ListView.Vertical : ListView.Horizontal
                reuseItems: true
                spacing: PlasmaCore.Units.largeSpacing
            }
        }
    }
}
