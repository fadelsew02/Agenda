#ifndef EVENTS_SAVE_H
#define EVENTS_SAVE_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QTableWidget>
#include <QLabel>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDate>
#include <QSqlQueryModel>

namespace Ui {
class events_save;
}

class events_save : public QDialog
{
    Q_OBJECT

public:
    explicit events_save(QWidget *parent = nullptr);
    ~events_save();
    void delete_table();

public slots:
    void periodChange();

private:
    Ui::events_save *ui;
    int seven;
    int three;
    int fourteen;
    int thirtyOne;
    QSqlQueryModel *modal;
    QComboBox *combo;
    QGridLayout *layoutPrincipal;
    QTableView *view;
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL","connection3");
};

#endif // EVENTS_SAVE_H
