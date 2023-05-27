#include "delete_events.h"
#include "ui_delete_events.h"

delete_events::delete_events(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::delete_events)
{
    ui->setupUi(this);
    setFixedSize(475,200);
    connect(ui->pushButton, &QPushButton::clicked,this,&delete_events::read_and_delete_event);
    connect(ui->pushButton_2, &QPushButton::clicked,this,&delete_events::close);
}

void delete_events::read_and_delete_event(){
    event = ui->lineEdit->text();

    QMessageBox::question(this, "Question", "Etes-vous sûr de supprimer cet event");

    db.setHostName("localhost");
    db.setDatabaseName("agenda");
    db.setUserName("fadelsew");
    db.setPassword("azerty");


    if (!db.open()) {
        qFatal("Erreur de connexion à la base de données delete: %s",
               qPrintable(db.lastError().text()));
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM event WHERE nom = :nom");
    query.bindValue(":nom", event);
    if(!query.exec()){
        qFatal("Erreur lors de la suppression dans la base de données: %s", qPrintable(query.lastError().text()));
            return;
    }else{
        QMessageBox::information(this, "Information", "L'event  " + event + "  a bien été supprimé");
    }


    close();
}

delete_events::~delete_events()
{
    db.close();
    delete ui;
}
