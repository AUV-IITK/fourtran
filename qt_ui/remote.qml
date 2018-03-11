import QtQuick 2.0
import QtQuick.Controls 1.4

Rectangle{
    id:page;
    width:900;
    height:600;

    Rectangle{
        id:frame;
        anchors.horizontalCenter: page.horizontalCenter;
        anchors.verticalCenter: page.verticalCenter;
        border.color:" black";
        border.width: 5;
        width:860;
        height:550;
        Grid{

            columnSpacing:40;
            columns:2;
            rows:2;
            anchors.horizontalCenter: frame.horizontalCenter;
            anchors.verticalCenter: frame.verticalCenter;
            Rectangle{
                color:"black";
                id:sway;
                height:50;
                width:350;
                Text {

                    text: qsTr("SWAY");
                    font.bold: true;
                    font.pointSize: 15;
                    color:"white";
                    anchors.horizontalCenter: sway.horizontalCenter;
                    anchors.verticalCenter: sway.verticalCenter;

                }
            }
            Rectangle{
                color:"black";
                id:forward;
                height:50;
                width:350;
                Text {

                    text: qsTr("FORWARD");
                    font.bold: true;
                    font.pointSize: 15;
                    color:"white";
                    anchors.horizontalCenter: forward.horizontalCenter;
                    anchors.verticalCenter: forward.verticalCenter;

                }
            }

            Rectangle{
                height:420;
                border.color:"black";
                border.width: 2;
                width:350;
                Loader{
                    source:"remote/remote_sway.qml";
                }

            }
            Rectangle{
                height:420;
                border.color:"black";
                border.width: 2;
                width:350;
                Loader{
                    source:"remote/remote_forward.qml";
                }

            }


            //no use of rectangle...but dont remove
            /*Rectangle{
                height: 30;
                width:250;
            }

            //no use of rectangle...but dont remove

            Rectangle{
                height: 30;
                width:250;
            }
*/

            //keyboard inputs


    }
}
}
