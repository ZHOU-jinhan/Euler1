# 初代机

#### 介绍
初代机相关代码

#### 软件架构
基于树莓派的库函数开发

#### 安装教程
1.  installation git
sudo apt-get install git
2.  Clone engineering
git clone https://gitee.com/jikexianfeng/wb_Early_generation_machine.git
3.  Enter the directory
cd wb_Early_generation_machine
4.  run
sudo ./bind.sh
#### 使用说明
1.  Pin usage(pin：Features )

gpio   0:motor_device2_A
gpio   1:motor_device2_B
gpio   2:motor_device2_D
gpio   3:motor_device2_C
gpio   4:motor_device2_EN
gpio   5:Steering_gear_90_gyro_bottom
gpio   6:motor_device1_EN
gpio   7:Steering_gear_90_gyro_top
gpio   8:Steering_gear_90_ultasonic_bottom
gpio   9:(引脚电平不可控，未用)
gpio 10:Ultrasonic_TrigPin
gpio 11:Ultrasonic_EchoPin
gpio 12:(未用)
gpio 13:(未用)
gpio 14:(未用)
gpio 15:gyro_TxD(ttyAMA0)
gpio 16:gyro_RxD(ttyAMA0)
gpio 17:(无)
gpio 18:(无)
gpio 19:(无)
gpio 20:(无)
gpio 21:motor_device0_D
gpio 22:motor_device0_C
gpio 23:motor_device0_EN
gpio 24:motor_device0_B
gpio 25:motor_device0_A
gpio 26:motor_device1_B
gpio 27:motor_device1_A
gpio 28:motor_device1_D
gpio 29:motor_device1_C
gpio 30:（未用）
gpio 31:（未用）
2.  文件介绍

3.  配置详情


#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request
