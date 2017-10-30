
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/qtcpsocket.h>
#include <qhostaddress.h>
#include <QCryptographicHash>
#include <stringtohex.h>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpSocket=new QTcpSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui;
//    QTcpSocket *tcpSocket;
//
}

void MainWindow::on_pushButton_clicked()
{
//    char *c;
    QString s;
    QByteArray c2;

    //QByteArray array;

    QDataStream stream(&c2, QIODevice::WriteOnly);
    //stream << m_ack;

    c2.resize(4);
    //memset(c2,0,4);
    s=ui->textEdit->document()->toRawText();
    quint64 m_ack = s.size();
    stream << m_ack ;

    QByteArray ba;
    ba.resize(s.size());
    ba= s.toLatin1();
    ui->label->setText(StringToHex(QCryptographicHash::hash(ba,QCryptographicHash::Md5)));
    tcpSocket->connectToHost(QHostAddress("192.168.0.5"),22222);
    tcpSocket->write(c2);
    tcpSocket->write(ba);
    tcpSocket->close();

}

void MainWindow::on_toolButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.jpg *.png)"));
    if(path.length() == 0) {
            QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    } else {
            QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
            QByteArray c2;
            QDataStream stream(&c2, QIODevice::WriteOnly);
            QFile *f=new QFile(path);
            f->open(QIODevice::ReadOnly);
            c2.resize(8);
            //s=ui->textEdit->document()->toRawText();
            quint64 m_ack = f->size();
            stream << m_ack ;
            QByteArray ba=f->readAll();
            f->close();
            //ba.resize(s.size());
            //ba= s.toLatin1();
            ui->label->setText(StringToHex(QCryptographicHash::hash(ba,QCryptographicHash::Md5)));
            tcpSocket->connectToHost(QHostAddress("192.168.0.5"),22222);
            tcpSocket->write(c2);
            tcpSocket->write(ba);
            tcpSocket->close();
    }
}
