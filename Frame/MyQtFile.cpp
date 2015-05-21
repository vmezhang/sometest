#include <QDebug>

#include "MyQtFile.h"
#include "filesystem/fileinterface.h"

MyQtFile::MyQtFile(const QString &name)
{
    filePath = name;
}

MyQtFile::~MyQtFile()
{

}

int MyQtFile::myfOpen()
{
    fdQfile = lfopen(filePath.toLatin1().data(), LF_RDWR|LF_CREAT);
    return fdQfile;
}

int MyQtFile::myfRead(QString &buffersize)
{
    int filesize;
    lfstat(fdQfile, &filesize);
    char buf[filesize + 1];
    memset(buf, '\0', filesize + 1);
    int i = lfread(fdQfile, buf, filesize);
    buffersize = QString(buf);
    return i;
}

int MyQtFile::myfwrite(QString &buffersize)
{
    return lfwrite(fdQfile, buffersize.toLatin1().data(), buffersize.size());
}

int MyQtFile::myfClose()
{
    return lfclose(fdQfile);
}
