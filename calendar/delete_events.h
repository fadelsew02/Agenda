#ifndef DELETE_EVENTS_H
#define DELETE_EVENTS_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QMessageBox>

namespace Ui {
class delete_events;
}

class delete_events : public QDialog
{
    Q_OBJECT

public:
    explicit delete_events(QWidget *parent = nullptr);
    ~delete_events();
    void read_and_delete_event();

private:
    Ui::delete_events *ui;
    QString event;
     QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL","connection_delete");
};


#endif // DELETE_EVENTS_H
