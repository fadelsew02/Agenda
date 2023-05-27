#include "dialog.h"
#include "ui_dialog.h"

#include <QtDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QDir>



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->comboBox->addItem("Red");
    ui->comboBox->addItem("Green");
    ui->comboBox->addItem("Blue");
    ui->comboBox->addItem("Yellow");
    ui->comboBox->addItem("Pink");
    ui->comboBox->addItem("Purple");
    ui->comboBox->addItem("Gray");
    ui->comboBox->setStyleSheet("QComboBox::item { background-color: red; }"
                                 "QComboBox::item:selected { background-color: yellow; }"
                                 "QComboBox::item:checked { background-color: green; }"
                                 "QComboBox QAbstractItemView { selection-background-color: blue; }");

    setFixedSize(400,350);

    connect(ui->pushButton, &QPushButton::clicked,this,&Dialog::lire_infos);
    connect(ui->pushButton_2, &QPushButton::clicked,this,&Dialog::close);
    connect(ui->pushButton_3, &QPushButton::clicked,this,&Dialog::open_file);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::open_file(){
    QString filename = QFileDialog::getOpenFileName(this,tr("Select Audio File"), "", tr("MP3 Files (*.MP3);;MPEG-4 Audio Files (*.m4a)"));

    QFileInfo fil(filename);
    QDateTime current = current.currentDateTime();
    QString destFile;
    QString dest;
    QString destPath = QDir::currentPath();
    qDebug() << filename;

    QFile srcFile(filename);
    QString url = fil.fileName();

    destFile = destPath + "/MusicDirectory/" + current.toString() + "__" + url;
    dest = current.toString() + "__" + url;

    if(srcFile.copy(destFile)){
        qDebug() << "Le fichier a été bien copié";
    }else{
        qDebug() << "Erreur lors de la copie du fichier";
    }
    ui->musicInput->setText(fil.fileName());

    musicPath = dest;
}

void Dialog::lire_infos(void){
    open_file();

    db2.setHostName("localhost");
    db2.setDatabaseName("agenda");
    db2.setUserName("fadelsew");
    db2.setPassword("azerty");


    if (!db2.open()) {
        qFatal("Erreur de connexion à la base de données DANS LIRE: %s",
               qPrintable(db2.lastError().text()));
        return;
    }

    bool nameIsEmpty = true;
    bool descriptionIsEmpty = true;
    bool dateok = false;
    bool musicIsEmpty = true;

    name = ui->lineEdit->text();
    description = ui->textEdit->toPlainText();
    colorName = ui->comboBox->currentText();
    QColor color(colorName);
    date = ui->dateEdit->text();
    time = ui->timeEdit->text();

    QDate dat = QDate::fromString(date, "dd/MM/yyyy");
    QString formattedDate = dat.toString("yyyy-MM-dd");

    QTime heu = QTime::fromString(time, "hh:mm");

    // Convertir la chaîne de caractères en QDate
    QDate dte = QDate::fromString(formattedDate, "yyyy-MM-dd");

    if(!name.isEmpty()){
        nameIsEmpty = false;
    }
    if(!description.isEmpty()){
        descriptionIsEmpty = false;
    }

    QDateTime currentDate = QDateTime::currentDateTime();

    // Convertir la date et l'heure en QDateTime
    QDateTime userDate;
    userDate.setDate(dte);
    userDate.setTime(heu);

    int diff = currentDate.secsTo(userDate);
    if(diff > 0){
        dateok = true;
    }

    QSqlQuery query1;
    query1.prepare("SELECT nom FROM event WHERE nom = :nom");
    query1.bindValue(":nom", name);

    if (!query1.exec()) {
        qFatal("Erreur: %s",
               qPrintable(query1.lastError().text()));
        return;
    }

    QString contentMusic = ui->musicInput->text();
    if(!contentMusic.isEmpty()){
        musicIsEmpty = false;
    }


    if(query1.size() == 0 && nameIsEmpty == false && descriptionIsEmpty == false && dateok == true && musicIsEmpty == false){

        QSqlQuery query;

        query.prepare("INSERT INTO event (nom, description, color, date, heure, music) VALUES (:nom, :description, :color, :date, :heure, :music)");
        query.bindValue(":nom", name);
        query.bindValue(":description", description);
        query.bindValue(":color", color.name());
        query.bindValue(":date", dte.toString("yyyy-MM-dd"));
        query.bindValue(":heure", heu.toString("hh:mm"));

        query.bindValue(":music", musicPath);
//        query.bindValue(":music", musicData, QSql::In | QSql::Binary);

        if (!query.exec()) {
            qFatal("Erreur lors de l'insertion dans la base de données : %s",
                   qPrintable(query.lastError().text()));
            return;
        }

        ui->lineEdit->setText("");
        ui->textEdit->setText("");
        ui->musicInput->setText("");


        db2.close();
        close();
    }

    QString textToDisplay;
    if(query1.size() > 0)
        textToDisplay = "Ce nom existe déja. Veuillez en choisir un autre.";
    if(nameIsEmpty == true || descriptionIsEmpty == true)
        textToDisplay = "Tous les champs sont requis! ";
    if(dateok == false)
        textToDisplay = "La date doit etre posterieure a la date courante ";
    if(musicIsEmpty == true)
        textToDisplay = "Remplissez le champ de musique";

    QMessageBox::information(this, "Information", textToDisplay);
}

