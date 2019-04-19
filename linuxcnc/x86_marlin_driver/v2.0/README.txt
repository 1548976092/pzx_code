#1.编译驱动包 对接marlin 1.1版本
halcompile --install hal_skeleton_marlin.c 


#2.执行linuxcnc

linuxcnc stepper_mm_pwm.ini
