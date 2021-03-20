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

QString id;

void MainWindow::on_listView_clicked(const QModelIndex &index)
{   id=index.data().toString();
    file.setFileName(cache_dir.path()+"/"+index.data().toString());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString tmp=in.readAll(),lyric; int p=62;
    tmp.replace(QString("\\n"),QString("\n"));
    if (tmp[62]!='[') ui->textEdit->setText("无歌词");
    else{
    for (int i=62;i<tmp.length()-3;i++) if (tmp.mid(i,3)=="\",\""){
        lyric+=tmp.mid(p,i-62); p=i; break;
    }//读取原歌词
    for (int i=p;i<tmp.length()-3;i++) if (tmp.mid(i,7)=="romeLrc"){
        i+=10; p=i;
        for (;i<tmp.length()-3;i++) if (tmp.mid(i,3)=="\",\""){lyric+=tmp.mid(p,i-p); break;}
        break;
    }//读取罗马音标
    for (int i=p;i<tmp.length()-3;i++) if (tmp.mid(i,5)=="Lyric"){
        i+=11;
        for(;i<tmp.length()-3;i++) if (tmp[i]==']'){p=i; break;}
        for (;i<tmp.length()-3;i++) if (tmp.mid(i,3)=="\",\""){lyric+=tmp.mid(p,i-p); break;}
        break;
    }//读取翻译
    ui->textEdit->setText(lyric);}
    ui->label->setText(file.fileName());

    QHostInfo::lookupHost("music.163.com",this,SLOT(ifonline(QHostInfo)));

}

void MainWindow::ifonline(QHostInfo host){
    if (host.error() != QHostInfo::NoError) {ui->label_2->setText("无法连接至网易云服务器");ui->statusbar->showMessage("无法连接至网易云服务器");}
    else {
        QNetworkAccessManager *url=new QNetworkAccessManager(this);
        connect(url, SIGNAL(finished(QNetworkReply    *)), this, SLOT(getweb(QNetworkReply*)));
        url->get(QNetworkRequest(QUrl("http://music.163.com/api/song/detail/?id="+id+"&ids=["+id+"]")));

    }

}
