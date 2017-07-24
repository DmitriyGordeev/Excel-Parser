#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

#include "xlsxwrap.h"
#include "merchsorter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addMerchVector(vector<Merch*> newVec);
    void addLoadedFile(QString filename);
    void addCountFile(int value);

private slots:

    void on_action_triggered();

    void on_btn_loadFile_clicked();

    void on_btn_sort_clicked();

    void on_btn_exit_clicked();

    void on_btn_deleteFile_clicked();

    void on_btn_preview_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    int _xlsxUnloading(QString filename, vector<Merch*> data, QSet<QString> propnames);

    MerchSorter _merchSorter;
    QStringList _loadedFiles;
    vector<int> _counts;
    QStringList _categories;
    vector<vector<Merch*>> _vvMerchs;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
