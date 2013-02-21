#include "resizedialog.h"
#include <QApplication>
#include<QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/dip"));
    ResizeDialog w;
    w.setWindowTitle("ImageResizer");
    w.show();
    
    return a.exec();
}
