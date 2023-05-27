#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtCore>
#include <QFileDialog>
#include <QtMultimedia/QMediaPlayer>
#include <QAudioOutput>
#include <QFile>
#include <QByteArray>
#include <QThread>



namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void lire_infos(void);
    void open_file();

private:
    Ui::Dialog *ui;
    QString colorName;
    QString name;
    QString description;
    QString date;
    QString time;
    QString musicPath;
//    QByteArray musicData;
    QSqlDatabase db2 = QSqlDatabase::addDatabase("QPSQL","connection2");

};

#endif // DIALOG_H
