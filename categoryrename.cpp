#include "categoryrename.h"
#include "ui_categoryrename.h"

#include <QDebug>
#include <QMessageBox>
#include "io.h"

#include <map>

CategoryRename::CategoryRename(MainWindow* main, QString filename, SettingsParser sp, int count, std::map<QString, std::vector<Merch*>> catMap, QWidget *parent) :
    QDialog(parent),
    _mainWin(main),
    _filename(filename),
    _count(count),
    _settings(sp),
    success(false),
    ui(new Ui::CategoryRename)
{
   ui->setupUi(this);

   ui->tableWidget->setColumnCount(2);
   ui->tableWidget->setColumnWidth(0, 200);
   ui->tableWidget->setColumnWidth(1, 200);
   ui->tableWidget->setRowCount(catMap.size());

   ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Старое значение" << "Новое значение");

   _oldMap = catMap;

   //Reading categories from file for dropDownMenu
   QStringList categoryList;
   if(!IO::qread("categories.txt", categoryList))
   {
       IO::qwrite("categories.txt", QStringList());
       IO::qread("categories.txt", categoryList);
   }

   categoryList.sort();

   //Set categories data for dropDownMenu:
   comboDelegate = new ComboDelegate(categoryList, this);
   model = new QStandardItemModel(2, 2, this);
   ui->tableWidget->setItemDelegate(comboDelegate);

   //Fill old values of table from catMap:
   int k=0;
   for(auto it = catMap.begin(); it != catMap.end(); ++it)
   {
       QTableWidgetItem* item = new QTableWidgetItem(it->first);
       item->setFlags(item->flags() & ~Qt::ItemIsEditable);

       ui->tableWidget->setItem(k, 0, item);
       k++;
   }

   //Fill new values of the table from _settings:
   QStringList cats = _settings.cats();
   if(cats.size() >= ui->tableWidget->rowCount())
   {
       for(int j = 0; j < ui->tableWidget->rowCount(); j++)
       {
           QTableWidgetItem* item = new QTableWidgetItem(QObject::tr(cats[j].toStdString().c_str()));
           ui->tableWidget->setItem(j, 1, item);
       }
   }
   else
   {
       for(int j = 0; j < cats.size(); j++)
       {
           QTableWidgetItem* item = new QTableWidgetItem(QObject::tr(cats[j].toStdString().c_str()));
           ui->tableWidget->setItem(j, 1, item);
       }
   }
}

CategoryRename::~CategoryRename()
{
    delete ui;
}

void CategoryRename::on_btn_save_clicked()
{
    int count = ui->tableWidget->rowCount();
    std::vector<Merch*> resultVec;
    QStringList newCatsForSettings;

    //For every row in table
    for(int i=0; i < count; i++)
    {
        if(ui->tableWidget->item(i, 1))
        {
            QString newCatName = ui->tableWidget->item(i, 1)->text();
            vector<Merch*> v = _oldMap[ui->tableWidget->item(i, 0)->text()];

            newCatsForSettings.push_back(newCatName);

            //Renaming category for every merch in v
            for(int i=0; i < v.size(); i++)
                v[i]->cat = newCatName;

            //Appending v to resultVec
            resultVec.insert(resultVec.begin(), v.begin(), v.end());

        } else {
            QMessageBox::information(this, "Ошибка", "Необходимо, чтобы каждому значению из левого столбца соответсвовало непустое значение из правого столбца");
            return;
        }
    }

    _mainWin->addMerchVector(resultVec);
    _mainWin->addLoadedFile(_filename);
    _mainWin->addCountFile(_count);

    //Save settings:
    _settings.cats(newCatsForSettings);
    _settings.save();

    success = true;

    this->close();
}
