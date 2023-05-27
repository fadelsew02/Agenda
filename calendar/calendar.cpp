#include "calendar.h"
#include "ui_calendar.h"

Calendar::Calendar(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calendar),  dialog(new Dialog)
{

    ui->setupUi(this);
    mSystemTrayIcon = new QSystemTrayIcon(this);
    mSystemTrayIcon->setIcon(QIcon(":/myappico.png"));
    mSystemTrayIcon->setVisible(true);
    connect(ui->actionAjouter_un_evenement,&QAction::triggered,this,&Calendar::addEvent);
    connect(ui->actionSupprimer_un_evenement,&QAction::triggered,this,&Calendar::del_event);
    connect(ui->actionVoir_les_evenements_enregistres,&QAction::triggered,this,&Calendar::saveEvent);
    connect(dialog, &Dialog::finished,this,&Calendar::colorDate);

    QDir currentdir = QDir::current();
    QString folderName = "MusicDirectory";

    if(!currentdir.exists(folderName)){
        currentdir.mkdir(folderName);
        qDebug() << "Le dossier a été crée avec succès";
    }


    db1.setHostName("localhost");
    db1.setDatabaseName("agenda");
    db1.setUserName("fadelsew");
    db1.setPassword("azerty");

    if (!db1.open()) {
        qFatal("Erreur de connexion à la base de données : %s",
               qPrintable(db1.lastError().text()));
        return;
    }

    removeOldEvents();
    colorDate();
}


void Calendar::addEvent(){
    dialog->open();
}

void Calendar::colorDate(){
    // Étape 1 : Récupérer les dates et les couleurs associées depuis la base de données
    QSqlQuery query;
    query.prepare("SELECT date, color FROM event");
    if(query.exec()) {
        while(query.next()) {
            QDate date = query.value(0).toDate(); // Récupérer la date dans la première colonne
            QString colorName = query.value(1).toString(); // Récupérer le nom de la couleur dans la deuxième colonne
            QColor color(colorName); // Convertir le nom de la couleur en QColor

            // Étape 2 : Utiliser la date et la couleur pour colorier la date correspondante dans votre widget de calendrier
            QTextCharFormat format;
            format.setBackground(QBrush(color, Qt::SolidPattern)); // Définir la couleur de fond
            ui->calendarWidget->setDateTextFormat(date, format);
        }
    }
}

void Calendar::montrerMessage()
{
    mSystemTrayIcon->showMessage(tr("Notification"), tr("Un event s'est déclenché"));
}

void Calendar::removeOldEvents()
{
    QSqlQuery query;
    QDate currentDate = QDate::currentDate();

    query.prepare("DELETE FROM event WHERE date < :dateLimit");
    query.bindValue(":dateLimit", currentDate.toString("yyyy-MM-dd"));

    if(!query.exec()) {
        qFatal("Erreur lors de la suppression: %s",qPrintable(query.lastError().text()));
        return;
    }
    qDebug() << query.size();
}

void Calendar::del_event(){
    delete_event = new delete_events;
    delete_event->open();
}

void Calendar::saveEvent(){
    event_save = new events_save;
    event_save->open();
}

Calendar::~Calendar()
{
    QDir curDir = QDir::current();

    QString toRemove = "QSystemTrayIcon";
    if(curDir.exists(toRemove)){
        curDir.remove(toRemove);
    }
    db1.close();
    delete ui;
}
