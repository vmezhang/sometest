// =====================================================================================
// 
//       Filename:  main.cpp
//
//    Description:  程序运行主函数
//
//        Version:  1.0
//        Created:  2014年12月11日 19时33分13秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  ZhangRui, v.me-zr@live.cn
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================

#include <QtGui>

#include "FrameWindow.h"
#include "filesystem/init.h"

int main(int argc, char *argv[])
{
    init_file_layout();
    init_memory_layout();
    // 处理中文乱码问题
    QTextCodec *codec = QTextCodec::codecForName("system");

    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);

    QApplication a(argc, argv);

    MainWindow w;
    // 窗口图标
    w.setWindowIcon(QIcon(":/res/images/3.png"));
    // 窗口大小
    w.resize(800, 550);
    // 窗口名称
    w.setWindowTitle("OSEP");
    // 窗体在屏幕中间
    w.move((QApplication::desktop()->width() - w.width()) >> 1,
                (QApplication::desktop()->height() - w.height()) >> 1);
    w.show();
    w.setFocus();

    return a.exec();
}

