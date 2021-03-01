#include "frminput.h"
#include "ui_frminput.h"
#include "qdesktopwidget.h"
#include "iconhelper.h"

#ifdef Q_OS_WIN32//判断系统是否为win32
#include <windows.h>
#endif


frmInput *frmInput::_instance = 0;
frmInput::frmInput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmInput)
{
    ui->setupUi(this);
    this->InitProperty();//初始化属性
    this->InitForm();//初始化窗体数据
    this->ChangeStyle();//改变样式，变颜色
}

frmInput::~frmInput()
{
    delete ui;
}

void frmInput::Init(QString position, QString style, int btnFontSize, int labFontSize)//初始化面板状态,包括字体大小
{
    //如果需要更改输入法面板的显示位置,改变currentPosition这个变量即可
    //control--显示在对应输入框的正下方 bottom--填充显示在底部  center--窗体居中显示
    this->currentPosition = position;

    //如果需要更改输入法面板的样式,改变currentStyle这个变量即可
    //blue--淡蓝色  dev--dev风格  black--黑色  brown--灰黑色  lightgray--浅灰色  darkgray--深灰色  gray--灰色  silvery--银色
    this->currentStyle = style;

    //如果需要更改面板字体大小，btnFontSize为按钮、labFontSize为标签的字体大小,
    this->btnFontSize = btnFontSize;
    this->labFontSize = labFontSize;

    this->ChangeStyle();//改变样式，变颜色
    this->ChangeFont();//改变字体大小
}

void frmInput::mouseMoveEvent(QMouseEvent *e)//鼠标拖动事件
{
    if (mousePressed && (e->buttons() && Qt::LeftButton))//如果鼠标左键按下
    {
        this->move(e->globalPos() - mousePoint);//移动窗口
        e->accept();
    }
}

void frmInput::mousePressEvent(QMouseEvent *e)//鼠标按下事件
{
    if (e->button() == Qt::LeftButton)//鼠标左键按下
    {
        mousePressed = true;//鼠标按下标志
        mousePoint = e->globalPos() - this->pos();//鼠标拖动自定义标题栏时的坐标
        e->accept();
    }
}

void frmInput::mouseReleaseEvent(QMouseEvent *)//鼠标松开事件
{
    mousePressed = false;//鼠标按下标志取消
}

void frmInput::InitForm()//初始化窗体数据
{
    this->mousePressed = false;
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);//设为工具窗口、置顶、无边框

    QDesktopWidget w;

    deskWidth = w.frameGeometry().width();//确定桌面宽高
    deskHeight = w.frameGeometry().height();

    frmWidth = this->width();//窗体宽高
    frmHeight = this->height();

    QSqlDatabase DbConn;//数据库
    DbConn = QSqlDatabase::addDatabase("QSQLITE");//数据库连接
    DbConn.setDatabaseName(qApp->applicationDirPath() + "/py.db");//设置连接文件名
    DbConn.open();//打开数据库

    isFirst = true;//首次加载标志
    isPress = false;//长按退格标志
    timerPress = new QTimer(this);//退格键定时器
    connect(timerPress, SIGNAL(timeout()), this, SLOT(reClicked()));

    currentWidget = 0;//当前焦点的对象
    currentLineEdit = 0;//当前焦点的单行文本框
    currentTextEdit = 0;//当前焦点的多行文本框
    currentPlain = 0;//当前焦点的富文本框
    currentBrowser = 0;//当前焦点的文本浏览框
    currentEditType = "";//当前焦点控件的类型

    //初始化量，实际无用
    currentPosition = "bottom";//键盘位置
    currentStyle = "black";//键盘风格
    btnFontSize = 10;//按钮大小
    labFontSize = 10;//标签大小
    currentType = "chinese";//键盘首页
    currentCaps = "min";
    currentNum = "ABC";
    currentChar = "123";
    changeType(currentType);
    changeKey(currentNum);

    QList<QPushButton *> btn = this->findChildren<QPushButton *>();//遍历按钮，设置槽
    foreach (QPushButton * b, btn)
    {
        connect(b, SIGNAL(clicked()), this, SLOT(btn_clicked()));
    }

    //绑定全局改变焦点信号槽
    connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)),
            this, SLOT(focusChanged(QWidget *, QWidget *)));
    //绑定按键事件过滤器
    qApp->installEventFilter(this);
}

void frmInput::InitProperty()//初始化属性
{
    //设置按钮属性值
    ui->btnSpace->setProperty("btnOther", true);
    ui->btnDelete->setProperty("btnOther", true);
    ui->btnCaps->setProperty("btnOther", true);
    ui->btnSwitch->setProperty("btnOther", true);
    ui->btnNum->setProperty("btnOther", true);
    ui->btnEnter->setProperty("btnOther", true);

    ui->btna->setProperty("btnLetter", true);
    ui->btnb->setProperty("btnLetter", true);
    ui->btnc->setProperty("btnLetter", true);
    ui->btnd->setProperty("btnLetter", true);
    ui->btne->setProperty("btnLetter", true);
    ui->btnf->setProperty("btnLetter", true);
    ui->btng->setProperty("btnLetter", true);
    ui->btnh->setProperty("btnLetter", true);
    ui->btni->setProperty("btnLetter", true);
    ui->btnj->setProperty("btnLetter", true);
    ui->btnk->setProperty("btnLetter", true);
    ui->btnl->setProperty("btnLetter", true);
    ui->btnm->setProperty("btnLetter", true);
    ui->btnn->setProperty("btnLetter", true);
    ui->btno->setProperty("btnLetter", true);
    ui->btnp->setProperty("btnLetter", true);
    ui->btnq->setProperty("btnLetter", true);
    ui->btnr->setProperty("btnLetter", true);
    ui->btns->setProperty("btnLetter", true);
    ui->btnt->setProperty("btnLetter", true);
    ui->btnu->setProperty("btnLetter", true);
    ui->btnv->setProperty("btnLetter", true);
    ui->btnw->setProperty("btnLetter", true);
    ui->btnx->setProperty("btnLetter", true);
    ui->btny->setProperty("btnLetter", true);
    ui->btnz->setProperty("btnLetter", true);

    //中文标签
    labCh.append(ui->labCh0);
    labCh.append(ui->labCh1);
    labCh.append(ui->labCh2);
    labCh.append(ui->labCh3);
    labCh.append(ui->labCh4);
    labCh.append(ui->labCh5);
    labCh.append(ui->labCh6);
    labCh.append(ui->labCh7);
    labCh.append(ui->labCh8);
    labCh.append(ui->labCh9);
    for (int i = 0; i < 10; i++)
    {
        labCh[i]->installEventFilter(this);
    }
}

void frmInput::ShowPanel()//显示输入法面板
{
    this->setVisible(true);//设置可见

    int width = ui->btnq->width();//读取btnq大小
    width = width > 40 ? width : 40;//真为width，假为40
    ui->btnPre->setMinimumWidth(width*0.75);//上一页大小
    ui->btnPre->setMaximumWidth(width*0.75);
    ui->btnNext->setMinimumWidth(width*0.75);//下一页大小
    ui->btnNext->setMaximumWidth(width*0.75);
    ui->btnClose->setMinimumWidth(width);//关闭大小
    ui->btnClose->setMaximumWidth(width);
    ui->btnNum->setMinimumWidth(width);//数字键大小
    ui->btnNum->setMaximumWidth(width);
    ui->btnSwitch->setMinimumWidth(width);//切换键大小
    ui->btnSwitch->setMaximumWidth(width);
    ui->btnEnter->setMinimumWidth(width*2);//回车键大小
    ui->btnEnter->setMaximumWidth(width*2);
    ui->btnClose->setMinimumWidth(width);//关闭键大小
    ui->btnClose->setMaximumWidth(width);

    IconHelper::Instance()->setIcon(ui->btnClose, 0xf11c, 15);
}

bool frmInput::eventFilter(QObject *obj, QEvent *event)//事件过滤器,用于识别鼠标单击汉字标签处获取对应汉字
{
    if (event->type() == QEvent::MouseButtonPress) //鼠标按下
    {
        if (obj == ui->labCh0) {
            for(int i=0;i<QSlabPY.length();i++){deleteValue();}setChinese(9);//清除字符，替换汉字**
        } else if (obj == ui->labCh1) {
            for(int i=0;i<QSlabPY.length();i++){deleteValue();}setChinese(0);//清除字符，替换汉字**
        } else if (obj == ui->labCh2) {
            for(int i=0;i<QSlabPY.length();i++){deleteValue();}setChinese(1);//清除字符，替换汉字**
        } else if (obj == ui->labCh3) {
            for(int i=0;i<QSlabPY.length();i++){deleteValue();}setChinese(2);//清除字符，替换汉字**
        } else if (obj == ui->labCh4) {
            for(int i=0;i<QSlabPY.length();i++){deleteValue();}setChinese(3);//清除字符，替换汉字**
        } else if (obj == ui->labCh5) {
            for(int i=0;i<QSlabPY.length();i++){deleteValue();}setChinese(4);//清除字符，替换汉字**
        } else if (obj == ui->labCh6) {
            for(int i=0;i<QSlabPY.length();i++){deleteValue();}setChinese(5);//清除字符，替换汉字**
        } else if (obj == ui->labCh7) {
            for(int i=0;i<QSlabPY.length();i++){deleteValue();}setChinese(6);//清除字符，替换汉字**
        } else if (obj == ui->labCh8) {
            for(int i=0;i<QSlabPY.length();i++){deleteValue();}setChinese(7);//清除字符，替换汉字**
        } else if (obj == ui->labCh9) {
            for(int i=0;i<QSlabPY.length();i++){deleteValue();}setChinese(8);//清除字符，替换汉字**
        } else if (currentEditType != "" && obj != ui->btnClose) {//焦点不空且未按关闭
            ShowPanel();//显示输入法面板
        }
        btnPress = (QPushButton *)obj;//退格检测
        if (checkPress())//如果长按
        {
            isPress = true;
            timerPress->start(500);
        }
        return false;
    }
    else if (event->type() == QEvent::MouseButtonRelease) //鼠标按压释放
    {
        btnPress = (QPushButton *)obj;//退格检测
        if (checkPress())//如果长按
        {
            isPress = false;
            timerPress->stop();
        }
        return false;
    }
    else if (event->type() == QEvent::KeyPress) //键盘按下
    {
        if (!isVisible())//如果输入法窗体不可见,则不需要处理
        {
            return QWidget::eventFilter(obj, event);
        }

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //键盘事件，Shift切换输入法模式,esc键关闭输入法面板,空格取第一个汉字,退格键删除

        if (keyEvent->key() == Qt::Key_Space)//中文模式下空格键获取首字,当没有拼音时输入空格
        {
            ui->btnSpace->click();
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) //大小键盘回车键获得拼音
        {
            ui->btnEnter->click();
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Shift) //Shift键，切换大小写
        {
            ui->btnSwitch->click();//切换大小写
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Escape) //Esc键，关闭键盘界面
        {
            ui->btnClose->click();//关闭
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Backspace) //退格键，删除输入内容
        {
            ui->btnDelete->click();//删除
            return true;
        }
        else if (keyEvent->key() == Qt::Key_CapsLock)//大写锁定键
        {
            ui->btnCaps->click();//切换大小写
            return true;
        }
        else if (keyEvent->text() == "+" || keyEvent->text() == "=") //按下键盘上的+或=
        {
            if (/*ui->labPY->text() != ""*/QSlabPY != "")//清除字符串**
            {
                ui->btnNext->click();//拼音下一页
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (keyEvent->text() == "-" || keyEvent->text() == "_")//按下键盘上的-或_
        {
            if (/*ui->labPY->text() != ""*/QSlabPY != "")//清除字符串**
            {
                ui->btnPre->click();//拼音上一页
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (currentEditType == "QWidget")//如果当前焦点为QWidget则无
            {
                return false;
            }

            QString key;
            if (currentCaps == "max")//如果大写则输入大，小写则输入小
            {
                key = keyEvent->text().toUpper();
            }
            else
            {
                key = keyEvent->text().toLower();
            }

            QList<QPushButton *> btn = this->findChildren<QPushButton *>();//遍历按钮
            foreach (QPushButton * b, btn)
            {
                if (b->text() == key) //点击和键盘输入对应的按钮
                {
                    b->click();
                    return true;
                }
            }
        }
        return false;
    }
    return QWidget::eventFilter(obj, event);
}

bool frmInput::checkPress()//校验当前长按的按钮
{
    //只有属于输入法键盘的合法按钮才继续处理
    bool num_ok = btnPress->property("btnNum").toBool();//返回是否
    bool other_ok = btnPress->property("btnOther").toBool();
    bool letter_ok = btnPress->property("btnLetter").toBool();
    if (num_ok || other_ok || letter_ok) //如果属于虚拟键盘的按键
    {
        return true;
    }
    return false;
}

void frmInput::reClicked()//重复点击
{
    if (isPress)
    {
        timerPress->setInterval(30);//设置超时间隔
        btnPress->click();//点击长按按钮
    }
}

void frmInput::focusChanged(QWidget *oldWidget, QWidget *nowWidget)//焦点改变
{
//    qDebug() << "oldWidget:" << oldWidget << " nowWidget:" << nowWidget;
    if (nowWidget != 0 && !this->isAncestorOf(nowWidget))
    {
        //在Qt5和linux系统中(嵌入式linux除外),当输入法面板关闭时,焦点会变成无,然后焦点会再次移到焦点控件处
        //这样导致输入法面板的关闭按钮不起作用,关闭后马上有控件获取焦点又显示.
        //为此,增加判断,当焦点是从有对象转为无对象再转为有对象时不要显示.
        //这里又要多一个判断,万一首个窗体的第一个焦点就是落在可输入的对象中,则要过滤掉
#ifndef __arm__
        if (oldWidget == 0x0 && !isFirst)//如果无焦点且不是第一次启动则不显示键盘
        {
            return;
        }
#endif

        isFirst = false;//不是第一次启动
        if (nowWidget->inherits("QLineEdit"))//如果是输入框
        {
            if(QSlabPY != ""){for(int i=0;i<QSlabPY.length();i++){deleteValue();}QSlabPY = "";clearChinese();}//变焦全清**
            currentLineEdit = (QLineEdit *)nowWidget;//记录焦点
            if(currentLineEdit->validator() != 0){currentText = currentLineEdit->text();}
            currentEditType = "QLineEdit";
            ShowPanel();//显示输入法面板
        }
        else if (nowWidget->inherits("QTextEdit"))//如果是文字编辑框
        {
            if(QSlabPY != ""){for(int i=0;i<QSlabPY.length();i++){deleteValue();}QSlabPY = "";clearChinese();}//变焦全清**
            currentTextEdit = (QTextEdit *)nowWidget;
            currentEditType = "QTextEdit";
            ShowPanel();
        }
        else if (nowWidget->inherits("QPlainTextEdit"))//如果是纯文本编辑框
        {
            if(QSlabPY != ""){for(int i=0;i<QSlabPY.length();i++){deleteValue();}QSlabPY = "";clearChinese();}//变焦全清**
            currentPlain = (QPlainTextEdit *)nowWidget;
            currentEditType = "QPlainTextEdit";
            ShowPanel();
        }
        else if (nowWidget->inherits("QTextBrowser"))//如果是文本浏览器
        {
            if(QSlabPY != ""){for(int i=0;i<QSlabPY.length();i++){deleteValue();}QSlabPY = "";clearChinese();}//变焦全清**
            currentBrowser = (QTextBrowser *)nowWidget;
            currentEditType = "QTextBrowser";
            ShowPanel();
        }
//        else if (nowWidget->inherits("QComboBox"))//下拉栏
//        {
//            QComboBox *cbox = (QComboBox *)nowWidget;
//            //只有当下拉选择框处于编辑模式才可以输入
//            if (cbox->isEditable()) {
//                currentLineEdit = cbox->lineEdit() ;
//                currentEditType = "QLineEdit";
//                ShowPanel();
//            }
//        }
//        else if (nowWidget->inherits("QSpinBox")||nowWidget->inherits("QDoubleSpinBox")||nowWidget->inherits("QDateEdit")
//                 ||nowWidget->inherits("QTimeEdit")||nowWidget->inherits("QDateTimeEdit"))//纺锤箱（上下箭头）
//        {
//            currentWidget = nowWidget;
//            currentEditType = "QWidget";
//            ShowPanel();
//        }
        else
        {
            //需要将输入法切换到最初的原始状态--chinese,同时将之前的对象指针置为零
            currentWidget = 0;
            currentLineEdit = 0;
            currentTextEdit = 0;
            currentPlain = 0;
            currentBrowser = 0;
            currentEditType = "";
            currentType = "chinese";//设置输入法起始页
            currentCaps = "min";
            currentNum = "ABC";
            currentChar = "123";

#ifdef Q_OS_WIN32//判断系统是否为win32
            if (!GetKeyState(VK_CAPITAL) & 1 )
            {currentCaps = "min";}
            else
            {currentCaps = "max";}
#endif
            changeKey(currentNum);
            changeType(currentType);

            this->setVisible(false);
        }

        //根据用户选择的输入法位置设置-居中显示-底部填充-显示在输入框正下方
        if (currentPosition == "center") //输入法位置-居中
        {
            QPoint pos = QPoint(deskWidth / 2 - frmWidth / 2, deskHeight / 2 - frmHeight / 2);
            this->setGeometry(pos.x(), pos.y(), frmWidth, frmHeight);
        }
        else if (currentPosition == "bottom") //输入法位置-底部
        {
            this->setGeometry(deskWidth/2 - frmWidth*0.75, deskHeight - frmHeight - 60, frmWidth*1.5, frmHeight*1.3);
        }
        else if (currentPosition == "control") //输入法位置-控件下方
        {
            QRect rect = nowWidget->rect();//获取焦点位置
            QPoint pos = QPoint(rect.left(), rect.bottom() + 2);//获取控件左下位置点
            pos = nowWidget->mapToGlobal(pos);//将小部件坐标位置转换为全局屏幕坐标
            this->setGeometry(pos.x(), pos.y(), frmWidth, frmHeight);
        }
    }
}

void frmInput::changeType(QString type)//改变输入法类型
{
    if (type == "english") //英文键盘布局
    {
        if (currentCaps == "max")
        {
            changeLetter(true);
            ui->btnSwitch->setText("英");
//            ui->labInfo->setText("英文");//Info小写**
            ui->btnCaps->setText("大");
        }
        else
        {
            changeLetter(false);
            ui->btnSwitch->setText("英");
//            ui->labInfo->setText("英文");//Info小写**
            ui->btnCaps->setText("小");
        }
    }
    else //中文键盘布局
    {
        if (currentCaps == "max")
        {
            changeLetter(true);
            ui->btnSwitch->setText("中");
//            ui->labInfo->setText("中文");//Info中文**
            ui->btnCaps->setText("大");
        }
        else
        {
            changeLetter(false);
            ui->btnSwitch->setText("中");
//            ui->labInfo->setText("中文");//Info中文**
            ui->btnCaps->setText("小");
        }
    }
    //每次切换到模式,都要执行清空之前中文模式下的信息
    clearChinese();
    /*ui->labPY->setText("");*/for(int i=0;i<QSlabPY.length();i++){deleteValue();}QSlabPY = "";//全清**
}

void frmInput::changeKey(QString type)//改变输入法类型
{
    if (type == "123") //英文键盘布局
    {
        ui->btnq->setText("1");
        ui->btnw->setText("2");
        ui->btne->setText("3");
        ui->btnr->setText("4");
        ui->btnt->setText("5");
        ui->btny->setText("6");
        ui->btnu->setText("7");
        ui->btni->setText("8");
        ui->btno->setText("9");
        ui->btnp->setText("0");
        ui->btna->setText("-");
        ui->btns->setText("/");
        ui->btnd->setText(":");
        ui->btnf->setText(";");
        ui->btng->setText("(");
        ui->btnh->setText(")");
        ui->btnj->setText("@");
        ui->btnk->setText("“");
        ui->btnl->setText("”");
        ui->btnz->setText("。");
        ui->btnx->setText("，");
        ui->btnc->setText("、");
        ui->btnv->setText("?");
        ui->btnb->setText("!");
        ui->btnn->setText(".");
        ui->btnm->setText("￥");

        ui->btnNum->setText("ABC");
        ui->btnCaps->setText("#+=");
//        ui->labInfo->setText("字符");//Info字符**
    }
    else if (type == "ABC")
    {
        ui->btnq->setText("q");
        ui->btnw->setText("w");
        ui->btne->setText("e");
        ui->btnr->setText("r");
        ui->btnt->setText("t");
        ui->btny->setText("y");
        ui->btnu->setText("u");
        ui->btni->setText("i");
        ui->btno->setText("o");
        ui->btnp->setText("p");
        ui->btna->setText("a");
        ui->btns->setText("s");
        ui->btnd->setText("d");
        ui->btnf->setText("f");
        ui->btng->setText("g");
        ui->btnh->setText("h");
        ui->btnj->setText("j");
        ui->btnk->setText("k");
        ui->btnl->setText("l");
        ui->btnz->setText("z");
        ui->btnx->setText("x");
        ui->btnc->setText("c");
        ui->btnv->setText("v");
        ui->btnb->setText("b");
        ui->btnn->setText("n");
        ui->btnm->setText("m");

        ui->btnNum->setText("123");
        ui->btnCaps->setText("小");
//        ui->labInfo->setText("中文");//Info中文**
    }
    //每次切换到模式,都要执行清空之前中文模式下的信息
    clearChinese();
    /*ui->labPY->setText("");*/for(int i=0;i<QSlabPY.length();i++){deleteValue();}QSlabPY = "";//全清**
}

void frmInput::changeKey2(QString type)//改变输入法类型
{
    if (type == "123") //英文键盘布局
    {
        ui->btnq->setText("1");
        ui->btnw->setText("2");
        ui->btne->setText("3");
        ui->btnr->setText("4");
        ui->btnt->setText("5");
        ui->btny->setText("6");
        ui->btnu->setText("7");
        ui->btni->setText("8");
        ui->btno->setText("9");
        ui->btnp->setText("0");
        ui->btna->setText("-");
        ui->btns->setText("/");
        ui->btnd->setText(":");
        ui->btnf->setText(";");
        ui->btng->setText("(");
        ui->btnh->setText(")");
        ui->btnj->setText("@");
        ui->btnk->setText("“");
        ui->btnl->setText("”");
        ui->btnz->setText("。");
        ui->btnx->setText("，");
        ui->btnc->setText("、");
        ui->btnv->setText("?");
        ui->btnb->setText("!");
        ui->btnn->setText(".");
        ui->btnm->setText("￥");

        ui->btnCaps->setText("#+=");
    }
    else if (type == "#+=")
    {
        ui->btnq->setText("【");
        ui->btnw->setText("】");
        ui->btne->setText("{");
        ui->btnr->setText("}");
        ui->btnt->setText("#");
        ui->btny->setText("%");
        ui->btnu->setText("^");
        ui->btni->setText("*");
        ui->btno->setText("+");
        ui->btnp->setText("=");
        ui->btna->setText("_");
        ui->btns->setText("——");
        ui->btnd->setText("\\");
        ui->btnf->setText("|");
        ui->btng->setText("~");
        ui->btnh->setText("《");
        ui->btnj->setText("》");
        ui->btnk->setText("$");
        ui->btnl->setText("&&");
        ui->btnz->setText("·");
        ui->btnx->setText("…");
        ui->btnc->setText("，");
        ui->btnv->setText(":-)");
        ui->btnb->setText("？");
        ui->btnn->setText("！");
        ui->btnm->setText("`");

        ui->btnCaps->setText("123");
    }
    //每次切换到模式,都要执行清空之前中文模式下的信息
    clearChinese();
    /*ui->labPY->setText("");*/for(int i=0;i<QSlabPY.length();i++){deleteValue();}QSlabPY = "";//全清**
}

void frmInput::changeLetter(bool isUpper)//改变字母大小写
{
    QList<QPushButton *> btn = this->findChildren<QPushButton *>();//遍历按钮
    foreach (QPushButton * b, btn)
    {
        if (b->property("btnLetter").toBool())
        {
            if (isUpper) //更换按钮上的大小写文本
            {
                b->setText(b->text().toUpper());
            }
            else
            {
                b->setText(b->text().toLower());
            }
        }
    }
}

void frmInput::selectChinese()//查询汉字
{
    clearChinese();//清空当前汉字信息
    QSqlQuery query;
    /*QString currentPY = ui->labPY->text();*/QString currentPY = QSlabPY;//提取字符串**
    QString sql = "select [word] from [pinyin] where [pinyin]='" + currentPY + "';";//选择单词，从拼音到拼音
    query.exec(sql);
    //逐个将查询到的字词加入汉字队列
    while(query.next()) //直到下一次检索
    {
        QString result = query.value(0).toString();//返回索引值
        QStringList text = result.split(" ");
        foreach (QString txt, text) //遍历txt
        {
            if (txt.length() > 0)
            {
                allPY.append(txt);
                currentPY_count++;
            }
        }
    }
    showChinese();//显示查询到的汉字
}

void frmInput::showChinese()//显示查询到的汉字
{
    //每个版面最多显示10个汉字
    int count = 1;
    currentPY.clear();//清理py表
    for (int i = 0; i < 10; i++)
    {
        labCh[i]->setText("");
    }

    for (int i = currentPY_index; i < currentPY_count; i++)
    {
        if (count == 11)
        {
            break;
        }
        if (count == 10)//0号放置末尾
        {
            QString txt = QString("%1.%2").arg(0).arg(allPY[currentPY_index]);//标号+文字 例：1.喵喵
            currentPY.append(allPY[currentPY_index]);//添加到链表
            labCh[0]->setText(txt);//设置显示栏
        }
        else//索引从1开始到9结束
        {
            QString txt = QString("%1.%2").arg(count).arg(allPY[currentPY_index]);
            currentPY.append(allPY[currentPY_index]);
            labCh[count]->setText(txt);
        }
        count++;
        currentPY_index++;
    }
    //qDebug() << "currentPY_index:" << currentPY_index << "currentPY_count:" << currentPY_count;
}

void frmInput::btn_clicked()//按钮点击
{
    if (currentEditType == "")//如果当前焦点控件类型为空,则返回不需要继续处理
    {
        return;
    }

    QPushButton *btn = (QPushButton *)sender();
    QString objectName = btn->objectName();
    if (objectName == "btnSwitch") //切换键盘类型
    {
        if (currentType == "english")
        {
            currentType = "chinese";
            currentCaps = "min";
            currentNum = "ABC";
            currentChar = "123";
        }
        else if (currentType == "chinese")
        {
            currentType = "english";
            currentCaps = "min";
            currentNum = "ABC";
            currentChar = "123";
        }
        changeKey(currentNum);
        changeType(currentType);
    }
    else if (objectName == "btnNum") //切换数字键盘
    {
        if (currentNum == "123")
        {
            currentNum = "ABC";
            currentCaps = "min";
            currentChar = "123";
        }
        else if (currentNum == "ABC")
        {
            currentNum = "123";
            currentCaps = "min";
            currentChar = "123";
        }
        changeKey(currentNum);
    }
    else if (objectName == "btnCaps") //切换大小写
    {
        if (currentNum == "123")//当为数字页面时，变为符号切换键
        {
            if (currentChar == "123")
            {
                currentChar = "#+=";
            }
            else if (currentChar == "#+=")
            {
                currentChar = "123";
            }
            changeKey2(currentChar);
        }
        else if (currentNum == "ABC")//当为字母页面时，变为大小写切换键
        {
            if (currentCaps == "max")
            {
                currentCaps = "min";
            }
            else if (currentCaps == "min")
            {
                currentCaps = "max";
            }
            changeType(currentType);
        }
    }
    else if (objectName == "btnDelete") //如果当前是中文模式,则删除对应拼音,删除完拼音之后再删除对应文本输入框的内容
    {
        if (currentType == "chinese")
        {
            /*QString txt = ui->labPY->text();*/QString txt = QSlabPY;//提取字符串**
            int len = txt.length();
            if (len > 0)//还存有拼音，则先删拼音
            {
                /*ui->labPY->setText(txt.left(len - 1));*/deleteValue();QSlabPY.chop(1);//删除字符且字符串末尾-1**
                selectChinese();
            }
            else//直接删除控件字符
            {
                deleteValue();//删除当前焦点控件的一个字符
            }
        }
        else//不是中文就直接删除
        {
            deleteValue();
        }
    }
    else if (objectName == "btnEnter") //回车键
    {
        /*insertValue(ui->labPY->text());*/insertValue(QSlabPY);//插入字符**
        /*ui->labPY->setText("");*/for(int i=0;i<QSlabPY.length();i++){deleteValue();}QSlabPY = "";//清空字符串**
        selectChinese();//输入字母
    }
    else if (objectName == "btnPre") //上一页
    {
        if (currentPY_index >= 20)//每次最多显示10个汉字,所以每次向前的时候索引要减20
        {
            if (currentPY_index % 10 == 0) //是否能除余
            {
                currentPY_index -= 20;
            }
            else
            {
                currentPY_index = currentPY_count - (currentPY_count % 10) - 10;
            }
        }
        else
        {
            currentPY_index = 0;
        }
        showChinese();
    }
    else if (objectName == "btnNext") //下一页
    {
        if (currentPY_index < currentPY_count - 1)
        {
            showChinese();
        }
    }
    else if (objectName == "btnClose")//关闭
    {
        if(QSlabPY != ""){for(int i=0;i<QSlabPY.length();i++){deleteValue();}QSlabPY = "";clearChinese();}//全清**
        this->setVisible(false);//键盘不可见
    }
    else if (objectName == "btnSpace")//空格
    {
        if (/*ui->labPY->text() != ""*/QSlabPY != "")//如果字符串为空**
        {
            for(int i=0;i<QSlabPY.length();i++){deleteValue();}setChinese(0);//清空字符串后，取1号字**
        }
        else
        {
            insertValue(" ");//插入值到当前焦点控件
        }
    }
    else
    {
        QString value = btn->text();
        //如果是&按钮，因为对应&被过滤,所以真实的text为去除前面一个&字符
        if (objectName == "btnl" && currentChar == "#+=")
        {
            value = value.right(1);
        }
        //当前不是中文模式,则单击按钮对应text为传递参数
        if (currentType != "chinese")
        {
            insertValue(value);
        }
        else
        {
            //中文模式下,允许输入特殊字符
            //单击对应数字按键取得当前索引的汉字(这个没了)
            //由于0号字放置在末尾，对应索引也做了相应修改
            if (btn->property("btnOther").toBool() || currentNum == "123" || currentChar == "#+=" || currentCaps == "max")
            {
                if (/*ui->labPY->text().length() == 0*/QSlabPY.length() == 0)//如果没py，特殊字符直接输入**
                {
                    insertValue(value);
                }
            }
            else if (btn->property("btnNum").toBool()) {//数字键则为py选择键
                if (/*ui->labPY->text().length() == 0*/QSlabPY.length() == 0) {//如果没py，数字直接输入**
                    insertValue(value);
                } else if (objectName == "btn0") {
                    setChinese(9);
                } else if (objectName == "btn1") {
                    setChinese(0);
                } else if (objectName == "btn2") {
                    setChinese(1);
                } else if (objectName == "btn3") {
                    setChinese(2);
                } else if (objectName == "btn4") {
                    setChinese(3);
                } else if (objectName == "btn5") {
                    setChinese(4);
                } else if (objectName == "btn6") {
                    setChinese(5);
                } else if (objectName == "btn7") {
                    setChinese(6);
                } else if (objectName == "btn8") {
                    setChinese(7);
                } else if (objectName == "btn9") {
                    setChinese(8);
                }
            }
            else if (btn->property("btnLetter").toBool())//输入字母
            {
                /*ui->labPY->setText(ui->labPY->text() + value);*/insertValue(value);QSlabPY.append(value);//插入字符且字符串+1**
                selectChinese();
            }
        }
    }
}

void frmInput::insertValue(QString value)//插入值到当前焦点控件
{
    if (currentEditType == "QLineEdit")
    {
        currentLineEdit->insert(value);
    }
    else if (currentEditType == "QTextEdit")
    {
        currentTextEdit->insertPlainText(value);
    }
    else if (currentEditType == "QPlainTextEdit")
    {
        currentPlain->insertPlainText(value);
    }
    else if (currentEditType == "QTextBrowser")
    {
        currentBrowser->insertPlainText(value);
    }
    else if (currentEditType == "QWidget")
    {
        QKeyEvent keyPress(QEvent::KeyPress, 0, Qt::NoModifier, QString(value));
        QApplication::sendEvent(currentWidget, &keyPress);
    }
}

void frmInput::deleteValue()//删除当前焦点控件的一个字符
{
    if (currentEditType == "QLineEdit")
    {
        if(currentText != currentLineEdit->text())
        {
            currentLineEdit->backspace();
        }
    }
    else if (currentEditType == "QTextEdit")
    {
        //获取当前QTextEdit光标,如果光标有选中,则移除选中字符,否则删除光标前一个字符
        QTextCursor cursor = currentTextEdit->textCursor();//返回可见光标
        if (cursor.hasSelection()) //如果光标选中
        {
            cursor.removeSelectedText();//移除选中内容
        }
        else
        {
            cursor.deletePreviousChar();//删除光标前一位
        }
    }
    else if (currentEditType == "QPlainTextEdit")
    {
        //获取当前QTextEdit光标,如果光标有选中,则移除选中字符,否则删除光标前一个字符
        QTextCursor cursor = currentPlain->textCursor();
        if (cursor.hasSelection())
        {
            cursor.removeSelectedText();
        }
        else
        {
            cursor.deletePreviousChar();
        }
    }
    else if (currentEditType == "QTextBrowser")
    {
        //获取当前QTextEdit光标,如果光标有选中,则移除选中字符,否则删除光标前一个字符
        QTextCursor cursor = currentBrowser->textCursor();
        if (cursor.hasSelection())
        {
            cursor.removeSelectedText();
        }
        else
        {
            cursor.deletePreviousChar();
        }
    }
    else if (currentEditType == "QWidget")
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier, QString());
        QApplication::sendEvent(currentWidget, &keyPress);
    }
}

void frmInput::setChinese(int index)//设置当前汉字
{
    int count = currentPY.count();
    if (count > index)
    {
        //添加完一个汉字后,清空当前汉字信息,等待重新输入
        insertValue(currentPY[index]);
        clearChinese();
        /*ui->labPY->setText("");*/QSlabPY = "";//清空字符串**
    }
}

void frmInput::clearChinese() //清空当前汉字信息
{
    //清空汉字,重置索引
    for (int i = 0; i < 10; i++)
    {
        labCh[i]->setText("");
    }
    allPY.clear();
    currentPY.clear();
    currentPY_index = 0;
    currentPY_count = 0;
}

void frmInput::ChangeStyle()//改变样式，变颜色
{
    if (currentStyle == "blue") {
        changeStyle("#DEF0FE", "#C0DEF6", "#C0DCF2", "#386487");
    } else if (currentStyle == "dev") {
        changeStyle("#C0D3EB", "#BCCFE7", "#B4C2D7", "#324C6C");
    } else if (currentStyle == "gray") {
        changeStyle("#E4E4E4", "#A2A2A2", "#A9A9A9", "#000000");
    } else if (currentStyle == "lightgray") {
        changeStyle("#EEEEEE", "#E5E5E5", "#D4D0C8", "#6F6F6F");
    } else if (currentStyle == "darkgray") {
        changeStyle("#D8D9DE", "#C8C8D0", "#A9ACB5", "#5D5C6C");
    } else if (currentStyle == "black") {
        changeStyle("#4D4D4D", "#292929", "#D9D9D9", "#CACAD0");
    } else if (currentStyle == "brown") {
        changeStyle("#667481", "#566373", "#C2CCD8", "#E7ECF0");
    } else if (currentStyle == "silvery") {
        changeStyle("#E1E4E6", "#CCD3D9", "#B2B6B9", "#000000");
    }
}

void frmInput::ChangeFont()//改变字体大小
{
    QFont btnFont(this->font().family(), btnFontSize);
//    QFont labFont(this->font().family(), labFontSize);//感觉设置有问题，统一文字大小，不用两个了

    QList<QPushButton *> btns = ui->widgetMain->findChildren<QPushButton *>();//遍历按钮修改字体
    foreach (QPushButton * btn, btns)
    {
        btn->setFont(btnFont);
    }

    QList<QLabel *> labs = ui->widgetTop->findChildren<QLabel *>();//遍历标签修改字体
    foreach (QLabel * lab, labs)
    {
        lab->setFont(btnFont);
    }
    ui->btnPre->setFont(btnFont);//设置字体
    ui->btnNext->setFont(btnFont);//这里有点问题
    ui->btnClose->setFont(btnFont);//虽然上面设置了
//    ui->labInfo->setFont(btnFont);//但还是重设一遍**
//    ui->labPY->setFont(btnFont);//反正没差**
}

void frmInput::changeStyle(QString topColor, QString bottomColor, QString borderColor, QString textColor)//改变样式
{
    QStringList qss;
    qss.append(QString("QWidget#frmInput{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 %1,stop:1 %2);}")
               .arg(topColor).arg(bottomColor));
    qss.append("QPushButton{padding:5px;border-radius:3px;}");
    qss.append(QString("QPushButton:hover{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 %1,stop:1 %2);}")
               .arg(topColor).arg(bottomColor));
    qss.append(QString("QLabel,QPushButton{color:%1;}").arg(textColor));
    qss.append(QString("QPushButton#btnPre,QPushButton#btnNext,QPushButton#btnClose{padding:5px;}"));
    qss.append(QString("QPushButton{border:1px solid %1;}")
               .arg(borderColor));
    qss.append(QString("QLineEdit{border:1px solid %1;border-radius:5px;padding:2px;background:none;selection-background-color:%2;selection-color:%3;}")
               .arg(borderColor).arg(bottomColor).arg(topColor));

    qss.append(QString("QPushButton:pressed{background-color:#4882d6;}"));//按钮按下颜色

    this->setStyleSheet(qss.join(""));
}
