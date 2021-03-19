#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QStandardItem>
#include <QStringListModel>
#include <QTextCodec>


QWidget *publicwidget;
QFile file;
QDir cache_dir("/sdcard/netease/cloudmusic/Cache/Lyric/"),download_dir("/sdcard/netease/cloudmusic/Download/Lyric/");
QStringListModel *filenamelistmodel;
QStringList filenamelist,filetimelist;

QFileInfo filetime;

        int max;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    publicwidget=ui->settings;
    ui->settings->hide();
    ui->listView->setEditTriggers(QAbstractItemView:: NoEditTriggers);
    if ((!cache_dir.exists()) & (!download_dir.exists())) ui->statusbar->showMessage("无法读取网易云歌词目录");
    else {
        cache_dir.setFilter(QDir::Files | QDir::NoSymLinks);
        download_dir.setFilter(QDir::Files | QDir::NoSymLinks);
        if (cache_dir.exists() & (cache_dir.count()!=0)) for (int i=0;i<cache_dir.count();i++)  {filenamelist.append(cache_dir[i]); filetime.setFile("/sdcard/netease/cloudmusic/Cache/Lyric/"+cache_dir[i]); filetimelist.append(filetime.lastModified().toString("yyyyMMddhhmmss")); }
        if (download_dir.exists() & (download_dir.count()!=0)) for (int i=0;i<download_dir.count();i++)  {filenamelist.append(download_dir[i]); filetime.setFile("/sdcard/netease/cloudmusic/Download/Lyric/"+cache_dir[i]); filetimelist.append(filetime.lastModified().toString("yyyyMMddhhmmss"));}
        ui->statusbar->showMessage("读取完成");
    }

    for (int i=0;i<filetimelist.length()-1;i++){
        max=i;
        for (int j=i+1;j<filetimelist.length();j++) if (filetimelist[max].toLongLong()<filetimelist[j].toLongLong()) max=j;
        std::swap(filenamelist[i],filenamelist[max]);   std::swap(filetimelist[i],filetimelist[max]);
    }

    filenamelistmodel=new QStringListModel(this);
    filenamelistmodel->setStringList(filenamelist);
    ui->listView->setModel(filenamelistmodel);

    connect(ui->actionsettings,SIGNAL(triggered()),this,SLOT(opensettings()));
    connect(ui->actionabout,SIGNAL(triggered()),this,SLOT(openabout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::opensettings(){publicwidget->show();}
void MainWindow::openabout(){publicwidget->move(100,100);}

void MainWindow::getweb(QNetworkReply *reply){
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString str = codec->toUnicode(reply->readAll()),song_name;
    for (int i=19;;i++){
        if (str[i]!='"') song_name+=str[i];
        else break;
    }
    if (song_name=="code") ui->statusbar->showMessage("访问过于频繁，被网易云ban了");
    ui->label_2->setText(song_name);
    reply->deleteLater();
}

void MainWindow::on_close_button_clicked()
{
    publicwidget->close();
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    file.setFileName(cache_dir.path()+"/"+index.data().toString());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString lyric=in.readAll();
    lyric.replace(QString("\\n"),QString("\n"));
    ui->textEdit->setText(lyric);
    ui->label->setText(file.fileName());
    QNetworkAccessManager *url=new QNetworkAccessManager(this);
    connect(url, SIGNAL(finished(QNetworkReply    *)), this, SLOT(getweb(QNetworkReply*)));
    url->get(QNetworkRequest(QUrl("http://music.163.com/api/song/detail/?id="+index.data().toString()+"&ids=["+index.data().toString()+"]")));

}
