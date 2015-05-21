#ifndef MYQTFILE_H
#define MYQTFILE_H

#include <QString>

using namespace std;

class MyQtFile
{
public:
    MyQtFile(const QString &name);
    ~MyQtFile();

    int myfOpen();
    int myfRead(QString &buffersize);
    int myfwrite(QString &buffersize);
    int myfClose();

private:
    // 文件描述符
    int fdQfile;
    QString filePath;
};

#endif // MYQTFILE_H
