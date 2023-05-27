#include "events_save.h"
#include "ui_events_save.h"

events_save::events_save(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Events Saved");
    setFixedSize(400,450);

    db.setHostName("localhost");
    db.setDatabaseName("agenda");
    db.setUserName("fadelsew");
    db.setPassword("azerty");

    if (!db.open()) {
        qFatal("Erreur de connexion à la base de données : %s",
               qPrintable(db.lastError().text()));
        return;
    }

    seven = 0;
    three = 0;
    fourteen = 0;
    thirtyOne = 0;

    layoutPrincipal = new QGridLayout(this);

    QLabel *label = new QLabel("Events Saved", this);
    label->adjustSize();
    label->setEnabled(false);
    label->setFont(QFont("Cursive",15));

    combo = new QComboBox(this);
    combo->addItem("all");
    combo->addItem("3");
    combo->addItem("7");
    combo->addItem("14");
    combo->addItem("31");

    QPushButton *cancelButton = new QPushButton("Cancel",this);

    layoutPrincipal->addWidget(label,0,0,5,1);
    layoutPrincipal->addWidget(combo,5,0,1,1);
    layoutPrincipal->addWidget(cancelButton,5,1,1,5);
    
    periodChange();


    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(combo, SIGNAL(currentTextChanged(QString)), this, SLOT(periodChange()));
}

events_save::~events_save(){


}

void events_save::delete_table()
{
    QSqlQuery queryDropThree;
    QString dropTableQueryThree = "DROP TABLE IF EXISTS forThree";

    if (queryDropThree.exec(dropTableQueryThree)) {
        qDebug() << "Table 'forThree' supprimée avec succès.";
    } else {
        qDebug() << "Erreur lors de la suppression de la table 'forThree':"
                 << queryDropThree.lastError().text();
    }

    QSqlQuery queryDropSeven;
    QString dropTableQuerySeven = "DROP TABLE IF EXISTS forSeven";

    if (queryDropSeven.exec(dropTableQuerySeven)) {
        qDebug() << "Table 'forSeven' supprimée avec succès.";
    } else {
        qDebug() << "Erreur lors de la suppression de la table 'forSeven':"
                 << queryDropSeven.lastError().text();
    }

    QSqlQuery queryDropFourteen;
    QString dropTableQuery14 = "DROP TABLE IF EXISTS forFourteen";

    if (queryDropFourteen.exec(dropTableQuery14)) {
        qDebug() << "Table 'forFourteen' supprimée avec succès.";
    } else {
        qDebug() << "Erreur lors de la suppression de la table 'forFourteen':"
                 << queryDropFourteen.lastError().text();
    }

    QSqlQuery queryDropThirtyOne;
    QString dropTableQuery31 = "DROP TABLE IF EXISTS forThirtyOne";

    if (queryDropThirtyOne.exec(dropTableQuery31)) {
        qDebug() << "Table 'forThirtyOne' supprimée avec succès.";
    } else {
        qDebug() << "Erreur lors de la suppression de la table 'forThirtyOne':"
                 << queryDropThirtyOne.lastError().text();
    }
}

void events_save::periodChange( )
{
    QString item = combo->currentText();
    QSqlQuery query;
    
    view = new QTableView(this);
    modal = new QSqlQueryModel;

    if( item == "all"){
        modal->setQuery("SELECT * FROM event");
        view->setModel(modal);
        view->hideColumn(0);
        view->show();
        layoutPrincipal->addWidget(view,0, 1, 5,5);
    }else{   
        int days = item.toInt();
        QDate startDate = QDate::currentDate();
        QDate endDate = startDate.addDays(days);

        query.prepare("SELECT nom, description, color, date, heure, music FROM event WHERE date BETWEEN :startDate AND :endDate");
        query.bindValue(":startDate", startDate.toString("yyyy-MM-dd"));
        query.bindValue(":endDate", endDate.toString("yyyy-MM-dd"));

        if (!query.exec()) {
            qFatal("Erreur d'exécution de la requête : %s",
                   qPrintable(query.lastError().text()));
            return;
        }

        if(days == 3){
            delete_table();
            QSqlQuery queryForCreate;
            QString createTableQuery = "CREATE TABLE forThree ("
                                       "id INTEGER PRIMARY KEY, "
                                       "nom character varying(50) NOT NULL, "
                                       "description character varying(250) NOT NULL,"
                                       "color character varying(50) NOT NULL,"
                                       "date date,"
                                       "heure time,"
                                       "music character varying(250) NOT NULL)";

            if (queryForCreate.exec(createTableQuery)) {
                qDebug() << "Table 'forThree' créée avec succès.";
            } else {
                qDebug() << "Erreur lors de la création de la table 'forThree':"
                         << queryForCreate.lastError().text();
            }

            QSqlQuery queryForPrivilege;
            QString tableName = "forThree";
            QString username = "fadelsew";
            QString grantQuery = QString("GRANT ALL PRIVILEGES ON %1 TO %2").arg(tableName).arg(username);

            if (queryForPrivilege.exec(grantQuery)) {
                qDebug() << "Privilèges GRANT accordés avec succès à l'utilisateur" << username << "pour la table" << tableName;
            } else {
                qDebug() << "Erreur lors de l'accord des privilèges GRANT :" << queryForPrivilege.lastError().text();
            }

            while (query.next()) {
                three++;
                queryForCreate.prepare("INSERT INTO forThree (id, nom, description, color, date, heure, music) VALUES (:id, :nom, :description, :color, :date, :heure, :music)");
                QString nom = query.value(0).toString();
                QString description = query.value(1).toString();
                QString couleur = query.value(2).toString();
                QString date = query.value(3).toString();
                QString heure = query.value(4).toString();
                QString musique = query.value(5).toString();
                qDebug() << nom << description << couleur << date << heure << musique;
                queryForCreate.bindValue(":id", three);
                queryForCreate.bindValue(":nom", nom);
                queryForCreate.bindValue(":description", description);
                queryForCreate.bindValue(":color", couleur);
                queryForCreate.bindValue(":date", date);
                queryForCreate.bindValue(":heure", heure);
                queryForCreate.bindValue(":music", musique);
            }

            if (queryForCreate.exec()) {
                qDebug() << "Données insérées avec succès dans la table 'forThree'.";
            } else {
                qDebug() << "Erreur lors de l'insertion de données dans la table 'forThree':"
                         << queryForCreate.lastError().text();
            }

            modal->setQuery("SELECT * FROM forThree");
            view->setModel(modal);
            view->hideColumn(0);
            view->show();
            layoutPrincipal->addWidget(view,0, 1, 5,5);

        }else if(days == 7){
            delete_table();
            QSqlQuery queryForCreate;
            QString createTableQuery = "CREATE TABLE forSeven ("
                                       "id INTEGER PRIMARY KEY, "
                                       "nom character varying(50) NOT NULL, "
                                       "description character varying(250) NOT NULL,"
                                       "color character varying(50) NOT NULL,"
                                       "date date,"
                                       "heure time,"
                                       "music character varying(250) NOT NULL)";

            if (queryForCreate.exec(createTableQuery)) {
                qDebug() << "Table 'forSeven' créée avec succès.";
            } else {
                qDebug() << "Erreur lors de la création de la table 'forSeven':"
                         << queryForCreate.lastError().text();
            }

            QSqlQuery queryForPrivilege;
            QString tableName = "forSeven";
            QString username = "fadelsew";
            QString grantQuery = QString("GRANT ALL PRIVILEGES ON %1 TO %2").arg(tableName).arg(username);

            if (queryForPrivilege.exec(grantQuery)) {
                qDebug() << "Privilèges GRANT accordés avec succès à l'utilisateur" << username << "pour la table" << tableName;
            } else {
                qDebug() << "Erreur lors de l'accord des privilèges GRANT :" << queryForPrivilege.lastError().text();
            }

            while (query.next()) {
                seven++;
                queryForCreate.prepare("INSERT INTO forSeven (id, nom, description, color, date, heure, music) VALUES (:id, :nom, :description, :color, :date, :heure, :music)");
                QString nom = query.value(0).toString();
                QString description = query.value(1).toString();
                QString couleur = query.value(2).toString();
                QString date = query.value(3).toString();
                QString heure = query.value(4).toString();
                QString musique = query.value(5).toString();
                qDebug() << nom << description << couleur << date << heure << musique;
                queryForCreate.bindValue(":id", seven);
                queryForCreate.bindValue(":nom", nom);
                queryForCreate.bindValue(":description", description);
                queryForCreate.bindValue(":color", couleur);
                queryForCreate.bindValue(":date", date);
                queryForCreate.bindValue(":heure", heure);
                queryForCreate.bindValue(":music", musique);
            }

            if (queryForCreate.exec()) {
                qDebug() << "Données insérées avec succès dans la table 'forSeven'.";
            } else {
                qDebug() << "Erreur lors de l'insertion de données dans la table 'forSeven':"
                         << queryForCreate.lastError().text();
            }

            modal->setQuery("SELECT * FROM forSeven");
            view->setModel(modal);
            view->hideColumn(0);
            view->show();
            layoutPrincipal->addWidget(view,0, 1, 5,5);
        }else if(days == 14){
            delete_table();
            QSqlQuery queryForCreate;
            QString createTableQuery = "CREATE TABLE forFourteen ("
                                       "id INTEGER PRIMARY KEY, "
                                       "nom character varying(50) NOT NULL, "
                                       "description character varying(250) NOT NULL,"
                                       "color character varying(50) NOT NULL,"
                                       "date date,"
                                       "heure time,"
                                       "music character varying(250) NOT NULL)";

            if (queryForCreate.exec(createTableQuery)) {
                qDebug() << "Table 'forFourteen' créée avec succès.";
            } else {
                qDebug() << "Erreur lors de la création de la table 'forFourteen':"
                         << queryForCreate.lastError().text();
            }

            QSqlQuery queryForPrivilege;
            QString tableName = "forFourteen";
            QString username = "fadelsew";
            QString grantQuery = QString("GRANT ALL PRIVILEGES ON %1 TO %2").arg(tableName).arg(username);

            if (queryForPrivilege.exec(grantQuery)) {
                qDebug() << "Privilèges GRANT accordés avec succès à l'utilisateur" << username << "pour la table" << tableName;
            } else {
                qDebug() << "Erreur lors de l'accord des privilèges GRANT :" << queryForPrivilege.lastError().text();
            }

            while (query.next()) {
                fourteen++;
                queryForCreate.prepare("INSERT INTO forFourteen (id, nom, description, color, date, heure, music) VALUES (:id, :nom, :description, :color, :date, :heure, :music)");
                QString nom = query.value(0).toString();
                QString description = query.value(1).toString();
                QString couleur = query.value(2).toString();
                QString date = query.value(3).toString();
                QString heure = query.value(4).toString();
                QString musique = query.value(5).toString();
                qDebug() << nom << description << couleur << date << heure << musique;
                queryForCreate.bindValue(":id", fourteen);
                queryForCreate.bindValue(":nom", nom);
                queryForCreate.bindValue(":description", description);
                queryForCreate.bindValue(":color", couleur);
                queryForCreate.bindValue(":date", date);
                queryForCreate.bindValue(":heure", heure);
                queryForCreate.bindValue(":music", musique);
            }

            if (queryForCreate.exec()) {
                qDebug() << "Données insérées avec succès dans la table 'forFourteen'.";
            } else {
                qDebug() << "Erreur lors de l'insertion de données dans la table 'forFourteen':"
                         << queryForCreate.lastError().text();
            }

            modal->setQuery("SELECT * FROM forFourteen");
            view->setModel(modal);
            view->hideColumn(0);
            view->show();
            layoutPrincipal->addWidget(view,0, 1, 5,5);
        }else if(days == 31){
            delete_table();
            QSqlQuery queryForCreate;
            QString createTableQuery = "CREATE TABLE forThirtyOne ("
                                       "id INTEGER PRIMARY KEY, "
                                       "nom character varying(50) NOT NULL, "
                                       "description character varying(250) NOT NULL,"
                                       "color character varying(50) NOT NULL,"
                                       "date date,"
                                       "heure time,"
                                       "music character varying(250) NOT NULL)";

            if (queryForCreate.exec(createTableQuery)) {
                qDebug() << "Table 'forThirtyOne' créée avec succès.";
            } else {
                qDebug() << "Erreur lors de la création de la table 'forThirtyOne':"
                         << queryForCreate.lastError().text();
            }

            QSqlQuery queryForPrivilege;
            QString tableName = "forThirtyOne";
            QString username = "fadelsew";
            QString grantQuery = QString("GRANT ALL PRIVILEGES ON %1 TO %2").arg(tableName).arg(username);

            if (queryForPrivilege.exec(grantQuery)) {
                qDebug() << "Privilèges GRANT accordés avec succès à l'utilisateur" << username << "pour la table" << tableName;
            } else {
                qDebug() << "Erreur lors de l'accord des privilèges GRANT :" << queryForPrivilege.lastError().text();
            }

            while (query.next()) {
                queryForCreate.prepare("INSERT INTO forThirtyOne (id, nom, description, color, date, heure, music) VALUES (:id,:nom, :description, :color, :date, :heure, :music)");
                thirtyOne++;
                QString nom = query.value(0).toString();
                QString description = query.value(1).toString();
                QString couleur = query.value(2).toString();
                QString date = query.value(3).toString();
                QString heure = query.value(4).toString();
                QString musique = query.value(5).toString();
                qDebug() << thirtyOne << nom << description << couleur << date << heure << musique;
                queryForCreate.bindValue(":id", thirtyOne);
                queryForCreate.bindValue(":nom", nom);
                queryForCreate.bindValue(":description", description);
                queryForCreate.bindValue(":color", couleur);
                queryForCreate.bindValue(":date", date);
                queryForCreate.bindValue(":heure", heure);
                queryForCreate.bindValue(":music", musique);

                if (queryForCreate.exec()) {
                    qDebug() << "Données insérées avec succès dans la table 'forThirtyOne'.";
                } else {
                    qDebug() << "Erreur lors de l'insertion de données dans la table 'forThirtyOne':"
                             << queryForCreate.lastError().text();
                }
            }

            modal->setQuery("SELECT * FROM forThirtyOne");
            view->setModel(modal);
            view->hideColumn(0);
            view->show();
            layoutPrincipal->addWidget(view,0, 1, 5,5);
        }
    }
}

