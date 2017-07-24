#ifndef CATEGORYRENAME_H
#define CATEGORYRENAME_H

#include <QDialog>
#include <QStandardItemModel>

#include <map>
#include <vector>
#include "merch.h"
#include "mainwindow.h"
#include "settingsparser.h"
#include "ComboDelegate.h"


namespace Ui {
class CategoryRename;
}

class CategoryRename : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryRename(MainWindow* main, QString filename, SettingsParser sp, int count, std::map<QString, std::vector<Merch*>> catMap, QWidget *parent = 0);
    ~CategoryRename();

    bool success;

private slots:
    void on_btn_save_clicked();

private:
    ComboDelegate* comboDelegate;
    QStandardItemModel *model;

    SettingsParser _settings;

    QString _filename;
    int _count;

    std::map<QString, std::vector<Merch*>> _oldMap;
    std::map<QString, std::vector<Merch*>> _newMap;
    Ui::CategoryRename *ui;
    MainWindow* _mainWin;
};

#endif // CATEGORYRENAME_H
