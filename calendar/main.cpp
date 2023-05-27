#include "calendar.h"
#include "dialog.h"

#include <QApplication>
#include <QtSql>
#include <QDateTime>
#include <QBuffer>
#include <QSoundEffect>
#include <QtMultimedia/QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");

    Calendar w;
    w.show();

    QSqlDatabase dtb = QSqlDatabase::addDatabase("QPSQL","connectionThread");
    dtb.setHostName("localhost");
    dtb.setDatabaseName("agenda");
    dtb.setUserName("fadelsew");
    dtb.setPassword("azerty");

    if (!dtb.open()) {
        qFatal("Erreur de connexion à la base de données : %s",
               qPrintable(dtb.lastError().text()));
        return 0;
    }


    QTimer timer;
    timer.setInterval(1000); // 1 seconde

    QObject::connect(&timer, &QTimer::timeout, [&]() {

        QDateTime now = QDateTime::currentDateTime();
        QMediaPlayer *player;
        QAudioOutput *audioOutput;

        QSqlQuery quer;
        quer.prepare("SELECT music FROM event WHERE date = :date AND heure = :time");
        quer.bindValue(":date", now.date().toString("yyyy-MM-dd"));
        quer.bindValue(":time", now.time().toString("hh:mm"));
        if (!quer.exec()) {
            qFatal("Erreur lors de la requête dans le thread : %s", qPrintable(quer.lastError().text()));
            return;
        }
        if (quer.next()) {
            QString musicData = quer.value(0).toString();
            player = new QMediaPlayer;
            audioOutput = new QAudioOutput;
            player->setAudioOutput(audioOutput);
            QString destPath = QDir::currentPath();
            QString destFile;

            destFile = destPath + "/MusicDirectory/" + musicData;

            player->setSource(QUrl::fromLocalFile(destFile));
            audioOutput->setVolume(50);
            player->play();

            w.montrerMessage();
        }

        dtb.close();
    });
    timer.start();

    return a.exec();
}
