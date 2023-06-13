# A-Security-System-Based-on-51-Single-Chip-Microcomputer
通过使用红外遥控器、单片机开发板、HC-SR04超声波传感器模块以及其他相关硬件，实现一个模拟多功能智能家居超声波安全防控系统。
本次课程设计旨在通过使用红外遥控器、单片机开发板、HC-SR04超声波传感器模块以及其他相关硬件，实现一个模拟多功能智能家居超声波安全防控系统。以下是具体的设计要求：
1. 系统基本功能
利用遥控器按下“1”键，布防超声波感应模块，在主人离开家之前开启布防。当有人从阳台和窗户入侵时，警报器会发出声音，同时 LCD1602 会显示相关信息。撤销布防可按下“2”键。
2. 日历功能
控制器在闲时（未布防，或者布防后未检测到入侵）可作为万年历使用。读出实时时钟芯片 DS1302 中的年月日、星期、时间，用 LCD1602 显示。
3. 远程控制功能
在撤销布防后可做家居多功能遥控器使用。按下“4”键，继电器吸合，实现风扇的打开；按下“5”键，继电器断开，实现风扇的关闭。另外，按下“7”键，将温度传感器采集的环境温度或室内温湿度显示在 LCD1602 上。
各模块结束：
main.c：主函数，由主函数调用以下各模块。
REDFEEL.c：红外线遥控器模块函数
HCSR04.c:超声波感应模块函数
DS18B20.c：温度读取模块函数
Motor.c：电机转动模块函数
LCD1602：时钟日历模块函数
