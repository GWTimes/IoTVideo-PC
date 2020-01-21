#include "widget.h"
#include <QApplication>
//#define DEBUG_TEST

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef DEBUG_TEST
    AllocConsole();  //用于控制台调试
    freopen("CONOUT$", "w", stdout);
#endif
    Widget w;
    w.show();

    return a.exec();
}
