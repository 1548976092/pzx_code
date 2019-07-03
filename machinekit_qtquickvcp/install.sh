//先安装qt5
sudo apt-get install build-essential
sudo apt-get install qtcreator
sudo apt-get install qt5-default

sudo apt-get install qtdeclarative5-dev qml-module-qtquick-controls

//克隆库
//git clone https://github.com/machinekit/QtQuickVcp.git
//安装插件
sudo apt install qml-module-qttest  QtQuick.XmlListModel QtQuick.Dialogs QtQuick.Particles 
sudo apt install QtQuick.XmlListModel QtQuick.Dialogs QtQuick.Particles QtQuick.XmlListModel 
sudo apt install QtQuick.Dialogs QtQuick.Particles Qt.Labs.settings 

//配置qmake
//QMAKE_DEFAULT_INCDIRS=\\
//或
//QMAKE_CFLAGS_ISYSTEM=-I

//以下使用qml的designer很关键
sudo apt install Qt.labs.folderlistmodel
sudo apt install QtQuick.Extras
//desigenr 需要
sudo apt install qtdeclarative5-private-dev
sudo apt-get install qttools5-private-dev
sudo apt-get install qtbase5-private-dev

//在realese里面 make install,docs,docs install
