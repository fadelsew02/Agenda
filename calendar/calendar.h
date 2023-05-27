#ifndef CALENDAR_H
#define CALENDAR_H

#include <QMainWindow>
#include <QGroupBox>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include <QSystemTrayIcon>


#include "dialog.h"
#include "delete_events.h"
#include "events_save.h"
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui { class Calendar; }
QT_END_NAMESPACE

class QSystemTrayIcon;

class Calendar : public QMainWindow
{
    Q_OBJECT

public:
    Calendar(QWidget *parent = nullptr);
    ~Calendar();
    void addEvent();
    void del_event();
    void saveEvent();
    void colorDate();
    void checkBddforPlayMusic();
    void montrerMessage();
    void removeOldEvents();

private:
    Ui::Calendar *ui;
    QSystemTrayIcon *mSystemTrayIcon;
    Dialog *dialog;
    delete_events *delete_event;
    events_save *event_save;
    QString color;
    QSqlDatabase db1 = QSqlDatabase::addDatabase("QPSQL");

};


#endif // CALENDAR_H
