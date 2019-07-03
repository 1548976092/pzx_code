sudo apt-get install -y avahi-utils

echo 123456 | sudo -S /etc/init.d/dbus start
echo 123456 | sudo -S /etc/init.d/avahi-daemon start    
. machinekit/scripts/rip-environment



sudo apt-get install ros-*-joint-state-controller
sudo apt-get install ros-indigo-effort-controllers
sudo apt-get install ros-*-position-controllers 
sudo apt-get install libactionlib-dev

sudo apt-get install ros-kinetic-joint-trajectory-controlle


sudo apt-get install ros-melodic-rosparam-shortcuts libgflags-dev

sudo apt-get install ros-*-robot-state-publisher
sudo apt-get install ros-*-moveit-visual-tools
sudo apt-get install ros-*-moveit-config
sudo apt-get install ros-*c-moveit-visual-tools
