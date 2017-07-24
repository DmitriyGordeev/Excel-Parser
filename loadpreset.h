#ifndef LOADPRESET_H
#define LOADPRESET_H

#include <QDialog>
#include <vector>
#include "brand.h"
#include "xlsxwrap.h"
#include "mainwindow.h"
#include "settingsparser.h"

#include <QTableWidget>

namespace Ui {
class LoadPreset;
}

class LoadPreset : public QDialog
{
    Q_OBJECT

public:
    explicit LoadPreset(MainWindow *mainwin, QWidget *parent = 0);
    ~LoadPreset();

private slots:
    void on_buttonLoad_clicked();

    void on_buttonSelect_clicked();

    void on_btn_brandAdd_clicked();

    void on_btn_brandDelete_clicked();

    void on_buttonCancel_clicked();

    void on_radioColumn_toggled(bool checked);

    void on_radioGroup_toggled(bool checked);

    void on_lineCode_textEdited(const QString &arg1);

    void on_lineName_textEdited(const QString &arg1);

    void on_linePrice_textEdited(const QString &arg1);

    void on_lineColumn_textEdited(const QString &arg1);

    void on_lineGroup_textEdited(const QString &arg1);

    void on_radioColumn_clicked();

    void on_radioGroup_clicked();

    void on_radioPrefix_clicked();

    void on_btn_addFilter_clicked();

    void on_btn_deleteFilter_clicked();

    void on_btn_filter_clicked();

    void on_btn_addProp_clicked();

    void on_btn_deleteProp_clicked();

    void on_comboSettings_activated(const QString &arg1);


    void on_btn_brandsFromFile_clicked();

private:
    vector<Brand> _brandsLoader(QTableWidget* table);
    void _setup();
    void _clearSettings();
    bool _isReady();
    void _saveFile();


    vector<Brand> _brands;
    MODE _mode;
    SettingsParser _settings;
    QXlsx::Document* _xlsx;


    QString _filepath;
    MainWindow* _mainWin;
    Ui::LoadPreset *ui;
};

#endif // LOADPRESET_H
