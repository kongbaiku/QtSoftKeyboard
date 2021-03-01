# QtSoftKeyboard

基于QT的软键盘小组件

## 说明

以前做的一个项目参考的软键盘程序，原始代码参考[SHJ211/QtSoftKeyboard](https://github.com/SHJ211/QtSoftKeyboard)

修改了屏幕键盘界面，符合现代软键盘的布局逻辑。

修改了输入界面，并添加了实体键盘支持。

修改了取词界面，使之符合逻辑。

【空格】取首位词组，【确认】取原始字母。

杂七杂八改了一堆，记不清了，现在版本的QT自带软键盘示例，这个的存在意义更多是参考与学习，所以……

**注释管饱**

## 使用说明

引入三个文件（frminput.h，frminput.cpp，frminput.ui），main函数中加入下面一行代码即可，支持所有输入框。

frmInput::Instance()->Init("control", "black", 10); 

字库采用sqlite数据库存储，可自由添加自定义的词组，将源码下的py.db数据库文件复制到编译后的可执行文件同一目录即可使用。

## 程序截图

![3.PNG](https://github.com/kongbaiku/QtSoftKeyboard/blob/main/README/3.png)

![2.PNG](https://github.com/kongbaiku/QtSoftKeyboard/blob/main/README/2.png))
