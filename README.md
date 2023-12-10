# C8T6_U8G2
a menu by stm32f103c8t6 and u8g2
# v1.0 加入所有菜单的自适应菜单项宽度
# v1.1 修改Bug(在Update处右键时会是整体程序卡死)，解决方案：在controlKey函数的左键逻辑中加入InitParameters()修复，可以正常进入Update子菜单，当产生故障的原因还未查明。
# v1.2 加入了UI启动动画，通过确认键进入菜单循环
