#ifndef PREVIEWTABLE_H
#define PREVIEWTABLE_H

#include <QDialog>
#include "merch.h"
#include <vector>

using namespace std;

namespace Ui {
class PreviewTable;
}

class PreviewTable : public QDialog
{
    Q_OBJECT

public:
    explicit PreviewTable(vector<Merch*> data, QWidget *parent = 0);
    ~PreviewTable();

private slots:
    void on_btn_search_clicked();

private:
    void _load(int column, QString filter);

    Ui::PreviewTable *ui;
    vector<Merch*> _data;
};

#endif // PREVIEWTABLE_H
