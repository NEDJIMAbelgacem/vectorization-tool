#include "mainwindow.h"

#include <QApplication>

#include "DarkStyle.h"
#include "framelesswindow/framelesswindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(new DarkStyle);

    // create frameless window (and set windowState or title)
      FramelessWindow framelessWindow;
      framelessWindow.setWindowState(Qt::WindowMaximized);
      framelessWindow.setWindowTitle("Vectorisation tool");
      //framelessWindow.setWindowIcon(a.style()->standardIcon(QStyle::SP_DesktopIcon));

    MainWindow w;
//    w.show();

    framelessWindow.setContent(&w);
    framelessWindow.show();
    MainWindow::frameless = &framelessWindow;

    return a.exec();
}
