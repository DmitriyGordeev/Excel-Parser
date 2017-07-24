#include "previewtable.h"
#include "ui_previewtable.h"

#include <QDebug>

PreviewTable::PreviewTable(vector<Merch*> data, QWidget *parent) :
    _data(data),
    QDialog(parent),
    ui(new Ui::PreviewTable)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Артикул" << "Категории" << "Бренд" << "Модель" << "Цена");

    ui->tableWidget->setRowCount(_data.size());

    for(int i=0; i < _data.size(); i++)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(_data[i]->vcode));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(_data[i]->cat));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(_data[i]->brand));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(_data[i]->model));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(_data[i]->price)));
    }

    ui->tableWidget->sortByColumn(0);
}

PreviewTable::~PreviewTable()
{
    delete ui;
}

//Loading filtered values to the table
void PreviewTable::_load(int column, QString filter)
{

    if(column == -1 || filter.isEmpty())
        return;

    //Filtering by Code
    if(column == 0)
    {
        for(int i=0; i < _data.size(); i++)
        {
            if((_data[i]->vcode).indexOf(filter, Qt::CaseInsensitive) != -1)
            {
                ui->tableWidget->setItem(i, 0, new QTableWidgetItem(_data[i]->vcode));
                ui->tableWidget->setItem(i, 1, new QTableWidgetItem(_data[i]->cat));
                ui->tableWidget->setItem(i, 2, new QTableWidgetItem(_data[i]->brand));
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem(_data[i]->model));
                ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(_data[i]->price)));
            }
        }
    }
    else if(column == 1)
    {
        for(int i=0; i < _data.size(); i++)
        {
            if((_data[i]->cat).indexOf(filter, Qt::CaseInsensitive) != -1)
            {
                ui->tableWidget->setItem(i, 0, new QTableWidgetItem(_data[i]->vcode));
                ui->tableWidget->setItem(i, 1, new QTableWidgetItem(_data[i]->cat));
                ui->tableWidget->setItem(i, 2, new QTableWidgetItem(_data[i]->brand));
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem(_data[i]->model));
                ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(_data[i]->price)));
            }
        }
    }
    else if(column == 2)
    {
        for(int i=0; i < _data.size(); i++)
        {
            if((_data[i]->brand).indexOf(filter, Qt::CaseInsensitive) != -1)
            {
                ui->tableWidget->setItem(i, 0, new QTableWidgetItem(_data[i]->vcode));
                ui->tableWidget->setItem(i, 1, new QTableWidgetItem(_data[i]->cat));
                ui->tableWidget->setItem(i, 2, new QTableWidgetItem(_data[i]->brand));
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem(_data[i]->model));
                ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(_data[i]->price)));
            }
        }
    }
    else if(column == 3)
    {
        for(int i=0; i < _data.size(); i++)
        {
            if((_data[i]->model).indexOf(filter, Qt::CaseInsensitive) != -1)
            {
                ui->tableWidget->setItem(i, 0, new QTableWidgetItem(_data[i]->vcode));
                ui->tableWidget->setItem(i, 1, new QTableWidgetItem(_data[i]->cat));
                ui->tableWidget->setItem(i, 2, new QTableWidgetItem(_data[i]->brand));
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem(_data[i]->model));
                ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(_data[i]->price)));
            }
        }
    }
    else if(column == 4)
    {
        for(int i=0; i < _data.size(); i++)
        {
            if( (QString::number(_data[i]->price)).indexOf(filter, Qt::CaseInsensitive) != -1)
            {
                ui->tableWidget->setItem(i, 0, new QTableWidgetItem(_data[i]->vcode));
                ui->tableWidget->setItem(i, 1, new QTableWidgetItem(_data[i]->cat));
                ui->tableWidget->setItem(i, 2, new QTableWidgetItem(_data[i]->brand));
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem(_data[i]->model));
                ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(_data[i]->price)));
            }
        }
    }
}

void PreviewTable::on_btn_search_clicked()
{
    QTableWidget* table = ui->tableWidget;
    QString exp = ui->lineEdit->text();

    int column = table->currentColumn();
    _load(column, exp);
}


