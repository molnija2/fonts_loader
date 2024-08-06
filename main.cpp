#include "dialog_fontload.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog_FontLoad w;

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));

    w.show();
    return a.exec();
}
