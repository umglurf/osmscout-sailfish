/*
 OSMScout - a Qt backend for libosmscout and libosmscout-map
 Copyright (C) 2016  Lukas Karas

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

import QtQuick 2.0
import Sailfish.Silica 1.0
import QtPositioning 5.2

import harbour.osmscout.map 1.0

import "../custom"

Page {
    id: layersPage

    AppSettings{
        id:appSettings
    }
    Settings {
        id: settings
    }
    onStatusChanged: {
        if (status == PageStatus.Activating){
            map.view = appSettings.mapView;
        }
    }

    Drawer {
        id: drawer
        anchors.fill: parent

        dock: layersPage.isPortrait ? Dock.Top : Dock.Left
        backgroundSize: layersPage.isPortrait ? drawer.height * 0.6 : drawer.width * 0.6
        open: true

        background: Rectangle{
            id: wrapper
            anchors.fill: parent
            color: "transparent"

            OpacityRampEffect {
                enabled: (!onlineTileProviderComboBox._menuOpen &&
                          !stylesheetComboBox._menuOpen &&
                          !fontComboBox._menuOpen &&
                          !fontSizeComboBox._menuOpen)
                offset: 1 - 1 / slope
                slope: flickable.height / (Theme.paddingLarge * 4)
                direction: 2
                sourceItem: flickable
            }
            SilicaFlickable {
                id: flickable
                anchors.fill: parent
                //height: childrenRect.height
                contentHeight: content.height + 2*Theme.paddingLarge

                VerticalScrollDecorator {}

                Column {
                    id: content
                    width: parent.width

                    PageHeader { title: qsTr("Map settings") }

                    Slider{
                        id: mapDpiSlider
                        width: parent.width

                        value: settings.mapDPI
                        valueText: Math.round((settings.mapDPI / 96) * 100) + "%"
                        minimumValue: 96
                        maximumValue: Math.max(96 * 2, settings.physicalDPI * 1.5)
                        label: qsTr("Map magnification")

                        onValueChanged: {
                            settings.mapDPI = value;
                        }
                    }

                    SectionHeader{ text: qsTr("Online Maps") }

                    TextSwitch{
                        id: onlineTilesSwitch
                        width: parent.width

                        checked: settings.onlineTiles
                        text: qsTr("Enable online maps")
                        //description: qsTr("Enables online maps")

                        onCheckedChanged: {
                            settings.onlineTiles = checked;
                        }
                    }

                    ComboBox {
						id: onlineTileProviderComboBox
                        width: parent.width

                        property bool initialized: false

                        OnlineTileProviderModel{
                            id: providerModel
                        }

                        label: qsTr("Style")
                        menu: ContextMenu {
                            Repeater {
                                width: parent.width
                                model: providerModel
                                delegate: MenuItem {
                                    text: qsTranslate("resource", name)
                                }
                            }
                        }

                        onCurrentItemChanged: {
                            if (!initialized){
                                return;
                            }

                            settings.onlineTileProviderId = providerModel.getId(currentIndex)
                        }
                        Component.onCompleted: {
                            for (var i = 0; i < providerModel.count(); i++) {
                                if (providerModel.getId(i) == settings.onlineTileProviderId) {
                                    currentIndex = i
                                    break
                                }
                            }
                            initialized = true;
                        }
                    }

                    SectionHeader{ text: qsTr("Offline Maps") }

                    TextSwitch{
                        id: offlineMapSwitch
                        width: parent.width

                        checked: settings.offlineMap
                        text: qsTr("Enable offline map")
                        //description: qsTr("See can overlap online tiles!")

                        onCheckedChanged: {
                            settings.offlineMap = checked;
                        }
                    }
                    ComboBox {
                        id: stylesheetComboBox
                        width: parent.width

                        property bool initialized: false

                        label: qsTr("Style")
                        menu: ContextMenu {
                            Repeater {
                                model: mapStyle
                                MenuItem { text: qsTranslate("stylesheet",name) }
                            }
                        }
                        MapStyleModel{
                            id: mapStyle
                        }

                        onCurrentItemChanged: {
                            if (!initialized){
                                return;
                            }
                            var stylesheet=mapStyle.file(currentIndex)
                            mapStyle.style = stylesheet;
                        }
                        Component.onCompleted: {
                            var stylesheet = mapStyle.style;
                            currentIndex = mapStyle.indexOf(stylesheet);
                            initialized = true;
                        }
                    }
                    ComboBox {
                        id: fontComboBox
                        width: parent.width

                        property bool initialized: false

                        label: qsTr("Font")
                        // some standard fonts form `fc-list :lang=en`
                        menu: ContextMenu {
                            MenuItem { text: qsTr("DejaVu Sans") }
                            MenuItem { text: qsTr("Droid Serif") }
                            MenuItem { text: qsTr("Liberation Sans") }
                        }

                        onCurrentItemChanged: {
                            if (!initialized){
                                return;
                            }
                            if (currentIndex==0)
                                settings.fontName="DejaVu Sans"
                            if (currentIndex==1)
                                settings.fontName="Droid Serif"
                            if (currentIndex==2)
                                settings.fontName="Liberation Sans"
                        }
                        Component.onCompleted: {
                            console.log("use font: "+settings.fontName);
                            if (settings.fontName=="DejaVu Sans")
                                currentIndex = 0;
                            if (settings.fontName=="Droid Serif")
                                currentIndex = 1;
                            if (settings.fontName=="Liberation Sans")
                                currentIndex = 2;
                            initialized = true;
                        }
                    }
                    ComboBox {
                        id: fontSizeComboBox
                        width: parent.width

                        property bool initialized: false

                        label: qsTr("Font Size")
                        menu: ContextMenu {
                            MenuItem { text: qsTr("Normal") }
                            MenuItem { text: qsTr("Big") }
                            MenuItem { text: qsTr("Bigger") }
                            MenuItem { text: qsTr("Huge") }
                        }

                        onCurrentItemChanged: {
                            if (!initialized){
                                return;
                            }
                            if (currentIndex==0)
                                settings.fontSize=2.0;
                            if (currentIndex==1)
                                settings.fontSize=3.0;
                            if (currentIndex==2)
                                settings.fontSize=4.0;
                            if (currentIndex==3)
                                settings.fontSize=6.0;
                        }
                        Component.onCompleted: {
                            if (settings.fontSize<=2.0)
                                currentIndex = 0;
                            if (settings.fontSize>2.0 && settings.fontSize <= 3.0)
                                currentIndex = 1;
                            if (settings.fontSize>3.0 && settings.fontSize <= 4.0)
                                currentIndex = 2;
                            if (settings.fontSize>4.0)
                                currentIndex = 3;
                            initialized = true;
                        }
                    }

                    TextSwitch{
                        id: renderSeaSwitch
                        width: parent.width

                        checked: settings.renderSea
                        text: qsTr("Sea rendering")
                        description: qsTr("Sea can overlap online tiles!")

                        onCheckedChanged: {
                            settings.renderSea = checked;
                        }
                    }

                    SectionHeader{ text: qsTr("Style flags") }

                    ListView {
                        id: flagList
                        StyleFlagsModel{
                            id: mapFlags
                        }
                        height: contentHeight
                        width: parent.width
                        model:mapFlags
                        delegate: TextSwitch{
                            checked: value
                            text: qsTranslate("styleflag", key)
                            property bool initialized: false
                            onCheckedChanged: {
                                if (initialized){
                                    mapFlags.setFlag(key, !value);
                                }
                            }
                            Component.onCompleted: {
                                initialized=true;
                            }
                        }
                    }
                }
            }
        }

        MapComponent {
            id: map

            focus: true
            anchors.fill: parent

            showCurrentPosition: true
        }
    }
}
