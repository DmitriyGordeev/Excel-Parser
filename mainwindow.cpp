#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "categorysettings.h"
#include "loadpreset.h"
#include "previewtable.h"

#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->fileList->setColumnCount(2);
    ui->fileList->setHorizontalHeaderLabels(QStringList() << "Имя файла" << "Количество товаров");
    ui->fileList->setColumnWidth(0, 140);
    ui->fileList->setColumnWidth(1, 220);

    ui->phrasesTable->setColumnCount(1);
    ui->phrasesTable->setHorizontalHeaderLabels(QStringList() << "Фраза");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addMerchVector(vector<Merch*> newVec)
{
    _vvMerchs.push_back(newVec);
}

void MainWindow::on_action_triggered()
{
    CategorySettings catSet("categories.txt");
    catSet.setModal(true);
    catSet.exec();
}

void MainWindow::on_btn_loadFile_clicked()
{

    LoadPreset loadPreset(this);
    loadPreset.setModal(true);
    loadPreset.exec();


    ui->fileList->setRowCount(_loadedFiles.size());
    for(int i=0; i < _loadedFiles.size(); i++)
    {
        QFileInfo fi(_loadedFiles[i]);
        QTableWidgetItem* item_name = new QTableWidgetItem(fi.fileName());
        item_name->setFlags(item_name->flags() & ~Qt::ItemIsEditable);
        ui->fileList->setItem(i, 0, item_name);

        QTableWidgetItem* item_count = new QTableWidgetItem(QString::number(_counts[i]));
        item_count->setFlags(item_count->flags() & ~Qt::ItemIsEditable);
        ui->fileList->setItem(i, 1, item_count);
    }

}

void MainWindow::addLoadedFile(QString filename)
{
    _loadedFiles.push_back(filename);
}

void MainWindow::addCountFile(int value)
{
    _counts.push_back(value);
}

void MainWindow::on_btn_sort_clicked()
{
    if(_vvMerchs.empty())
    {
        QMessageBox::information(this, "Ошибка", "Необходимо загрузить хотя бы один файл");
        return;
    }

    //Getting phrases from phrasesTable
    QStringList phrases;
    for(int i=0; i < ui->phrasesTable->rowCount(); i++)
        phrases.push_back(ui->phrasesTable->item(i,0)->text());

    //Getting trash
    QString trash = ui->lineTrash->text();

    //Comparing all loaded merchs
    _merchSorter.fill(_vvMerchs, trash, phrases);
    int totalCompared;
    vector<Merch*> resultArray = _merchSorter.mainAnalys(totalCompared);

    //Finding all properties through all merchs:
    QStringList properties;
    for(int i=0; i < resultArray.size(); i++)
        properties.append(resultArray[i]->getPropNames());

    QSet<QString> uniqueProps = QSet<QString>::fromList(properties);

    //unloading resulting file:
    int count = _xlsxUnloading("final.xlsx", resultArray, uniqueProps);

    QString message = "Было выгружено " + QString::number(count) + " товаров\n" + "Прошло сравнение: " + QString::number(totalCompared);
    QMessageBox::information(this, "Результат", message);
}

int MainWindow::_xlsxUnloading(QString filename, vector<Merch*> data, QSet<QString> propNames)
{
    QString path = QCoreApplication::applicationDirPath();
    QXlsx::Document xlsx;

    //setup headers:
    xlsx.write("A1", "Артикул");
    xlsx.write("B1", "Наименование");
    xlsx.write("C1", "Цена");
    xlsx.write("D1", "Из какого файла взят этот товар");
    xlsx.write("E1", "Контроль сравнений");

    //Additions properties headers:
    int column = 6;
    for(QSet<QString>::iterator it = propNames.begin(); it != propNames.end(); ++it)
    {
        xlsx.write(1,column, *it);
        column++;
    }

    for(int i=0; i < data.size(); i++)
    {
        column = 6;
        QString name = data[i]->cat + " " + data[i]->brand + " " + data[i]->model;

        xlsx.write("A" + QString::number(i + 2), data[i]->vcode);
        xlsx.write("B" + QString::number(i + 2), name);
        xlsx.write("C" + QString::number(i + 2), data[i]->price);
        xlsx.write("D" + QString::number(i + 2), data[i]->filename);
        xlsx.write("E" + QString::number(i + 2), data[i]->compared);


        //For every avalible property in header of table
        //looking for existence of this property in each merch
        map<QString, QVariant> pm_props = data[i]->props;
        for(QSet<QString>::iterator it = propNames.begin(); it != propNames.end(); ++it)
        {
            //name_it: iterator of properties map of current merch
            //if such name found then write it to next column
            auto name_it = pm_props.find(*it);
            if(name_it != pm_props.end())
            {
                xlsx.write(i + 2, column, name_it->second);
                column++;
            }

        }

    }

    xlsx.saveAs(path + "/" + filename);
    return data.size();
}

void MainWindow::on_btn_exit_clicked()
{
    this->close();
}

void MainWindow::on_btn_deleteFile_clicked()
{
    //Deleting values from table
    auto items = ui->fileList->selectedItems();
    for(int i=0; i < items.size(); i++)
    {
        int row = items[i]->row();
        ui->fileList->removeRow(items[i]->row());
        _vvMerchs.erase(_vvMerchs.begin() + row);
        _loadedFiles.erase(_loadedFiles.begin() + row);
        _counts.erase(_counts.begin() + row);
    }
}



//on_btn_preview - deprecated option
void MainWindow::on_btn_preview_clicked()
{
    //Getting vector<Merch*> of selected loaded file
    auto items = ui->fileList->selectedItems();
    if(items.isEmpty())
        return;

    int row = items[0]->row();
    PreviewTable pwt(_vvMerchs[row]);
    pwt.setModal(true);
    pwt.exec();
}



void MainWindow::on_pushButton_clicked()
{
    ui->phrasesTable->setRowCount(ui->phrasesTable->rowCount() + 1);
}

void MainWindow::on_pushButton_2_clicked()
{
    if(ui->phrasesTable->rowCount() > 0)
    {
        int row = ui->phrasesTable->currentColumn();
        ui->phrasesTable->removeRow(row);
    }
}
