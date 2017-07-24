#include "categorysettings.h"
#include "ui_categorysettings.h"

#include <QListWidgetItem>
#include <QtCore>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

#include "io.h"

CategorySettings::CategorySettings(QString filename, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategorySettings)
{
    ui->setupUi(this);

    _filename = filename;

    ui->label->setText(filename);

    if(!IO::qread(_filename, _categoryList))
    {
        IO::qwrite(_filename, QStringList());
    }

    //Loading into listWidget
    for(int i=0; i < _categoryList.size(); i++)
    {
        QListWidgetItem *newItem = new QListWidgetItem(_categoryList[i]);
        ui->listWidget->addItem(newItem);
    }

}

CategorySettings::~CategorySettings()
{
    delete ui;
}

QStringList CategorySettings::getCategories()
{
    return _categoryList;
}

void CategorySettings::on_btn_add_clicked()
{
    QString str = ui->lineEdit->text();
    if(!str.isEmpty() && (ui->listWidget->findItems(str, Qt::MatchFixedString).size() == 0))
    {
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(str);
        ui->listWidget->addItem(newItem);
        _categoryList.push_back(str);
        ui->lineEdit->setText("");
    }
}

void CategorySettings::on_btn_delete_clicked()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    foreach(QListWidgetItem * item, items)
    {
        int index = ui->listWidget->row(item);
        _categoryList.erase(_categoryList.begin() + index);
        delete ui->listWidget->takeItem(index);
    }
}

void CategorySettings::on_btn_exit_clicked()
{
    this->close();
}

void CategorySettings::on_btn_save_clicked()
{
    IO::qwrite(_filename, _categoryList);
}
