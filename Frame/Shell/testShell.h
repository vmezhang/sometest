// =====================================================================================
// 
//       Filename:  testShell.h
//
//    Description:  一些自定义的测试命令
//
//        Version:  1.0
//        Created:  2015年05月15日 13时40分23秒
//       Revision:  none
//       Compiler:  g++
//
//         Author:  ZhangRui, v.me-zr@live.cn
//        Company:  Class 1107 of Computer Science and Technology
// 
// =====================================================================================


#ifndef TESTSHELL_H
#define TESTSHELL_H

#include "shell.h"
#include <QFile>
#include <QDebug>

QString readShell(QString cmd);
// cmd解析函数
// QString getCmdResult(QString cmd);

QString readShell(QString cmd)
{
    // 输出的结果
    QString outMessage;
    // 读取结果的文件名
    QString fileName;
    // 传递程序原文件
    QString pcfile;

    // 解析cmd
    // outMessage = getCmdResult(cmd);
    if (cmd.left(3) != "pl ") {
        return "error!";
    }
    // 保存源文件名之后传给编译运行程序的脚本之中
    pcfile = cmd.mid(3);
    qDebug(pcfile.toLatin1().data());
    // system("sh xxx.sh");

    // 程序执行的结果以及编译信息均保存在/tmp/result.txt中
    // 每次都去完毕我会将其结果清空保证文件
    // 这样做为了保证每次运行过程中的反馈信息以追加模式写入

    fileName = "/tmp/result.txt";
    QFile outputFile(fileName);
    outputFile.open(QFile::ReadWrite);
    outMessage = outputFile.readAll();
    // 运行结束后将文件内容清空
    outputFile.resize(0);

    return outMessage;
}

#endif  // TESTSHELL_H
