#ifndef CATEGORYSETTINGS_H
#define CATEGORYSETTINGS_H

#include <QDialog>

namespace Ui {
class CategorySettings;
}

class CategorySettings : public QDialog
{
    Q_OBJECT

public:
    explicit CategorySettings(QString filename, QWidget *parent = 0);
    ~CategorySettings();

    QStringList getCategories();

private slots:
    void on_btn_add_clicked();

    void on_btn_delete_clicked();

    void on_btn_exit_clicked();

    void on_btn_save_clicked();

private:
    Ui::CategorySettings *ui;
    QString _filename;
    QStringList _categoryList;
};

#endif // CATEGORYSETTINGS_H
