#include "mainwindow.h"
#include "connectdlg.h"

#include <QApplication>
#include <QDialog>

int main(int argc, char *argv[])
{
    if(QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    ConnDlg dialog;
    if(dialog.exec() != QDialog::Accepted)
        return -1;
    dialog.show();

    // MainWindow w;
    // w.show();
    return a.exec();
}
