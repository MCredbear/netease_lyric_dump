#include "mainwindow.h"

#include <QApplication>
#include <QtAndroid>
bool checkPermission() {
    QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
    if(r == QtAndroid::PermissionResult::Denied) {
        QtAndroid::requestPermissionsSync( QStringList() << "android.permission.WRITE_EXTERNAL_STORAGE" );
        r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");
        if(r == QtAndroid::PermissionResult::Denied) {
             return false;
        }
   }
   return true;
}
bool checkPermission2() {
    QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission.READ_EXTERNAL_STORAGE");
    if(r == QtAndroid::PermissionResult::Denied) {
        QtAndroid::requestPermissionsSync( QStringList() << "android.permission.READ_EXTERNAL_STORAGE" );
        r = QtAndroid::checkPermission("android.permission.READ_EXTERNAL_STORAGE");
        if(r == QtAndroid::PermissionResult::Denied) {
             return false;
        }
   }
   return true;
}
int main(int argc, char *argv[])
{
    checkPermission();
    checkPermission2();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
