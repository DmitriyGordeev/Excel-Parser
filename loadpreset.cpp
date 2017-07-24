#include "loadpreset.h"
#include "ui_loadpreset.h"

#include "categoryrename.h"
#include "xlsxfilter.h"
#include "io.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QProgressDialog>


LoadPreset::LoadPreset(MainWindow* main, QWidget *parent) :
    _mainWin(main),
    QDialog(parent),
    ui(new Ui::LoadPreset)
{
    ui->setupUi(this);
    ui->brandsList->setColumnCount(2);
    ui->brandsList->setHorizontalHeaderLabels(QStringList() << "Eng" << "Рус");
    ui->brandsList->setColumnWidth(0, 110);
    ui->brandsList->setColumnWidth(1, 110);

    ui->filterTable->setColumnCount(2);
    ui->filterTable->setColumnWidth(0, 80);
    ui->filterTable->setHorizontalHeaderLabels(QStringList() << "Столбец" << "Выражение");

    ui->propsTable->setColumnCount(2);
    ui->propsTable->setHorizontalHeaderLabels(QStringList() << "Название" << "Столбец");

    QStringList filenames = _settings.getFilesList();
    ui->comboSettings->addItems(filenames);
}

LoadPreset::~LoadPreset()
{
    delete ui;
}

void LoadPreset::on_buttonLoad_clicked()
{
    //1. Loading brands list from brandsList table and name
    QFileInfo FI(_filepath);
    _settings.filename(FI.fileName());
    _brands = _brandsLoader(ui->brandsList);

    //2. Determine the mode
    QString catColumn = "";
    if(ui->radioColumn->isChecked())
    {
        _mode = MODE::COLUMN;
        catColumn = ui->lineColumn->text();
    }
    else if(ui->radioGroup->isChecked())
    {
        _mode = MODE::GROUP;
        catColumn = ui->lineGroup->text();
    }
    else if(ui->radioPrefix->isChecked())
        _mode = MODE::PREFIX;


    //3. Determine the main columns: code, name, price
    QString lineCode  = (ui->lineCode->text()).toUpper();
    QString lineName  = (ui->lineName->text()).toUpper();
    QString linePrice = (ui->linePrice->text()).toUpper();


    if(lineCode.isEmpty()
            || lineName.isEmpty()
            || linePrice.isEmpty()
            || (ui->radioColumn->isChecked() && catColumn.isEmpty())
            || (ui->radioGroup->isChecked() && catColumn.isEmpty())
            || _brands.empty())

    {
        QMessageBox::information(this, "Не все поля заполнены", "Необходимо заполнить все поля");
        return;
    }
    else
    {
        //3.5 Assign settings:
        if(_mode == MODE::COLUMN)
        {
            _settings.method("column");
            _settings.methodColumn(ui->lineColumn->text());
        }
        else if(_mode == MODE::GROUP)
        {
            _settings.method("group");
            _settings.methodColumn(ui->lineGroup->text());
        }
        else if(_mode == MODE::PREFIX)
            _settings.method("prefix");

        _settings.code(lineCode);
        _settings.name(lineName);
        _settings.price(linePrice);
        _settings.brands(_brands);

        //4. Loading extra properties for merchs:
        vector<pair<QString, QString>> propsData;
        for(int i=0; i < ui->propsTable->rowCount(); i++)
        {
            if(ui->propsTable->item(i,0) && ui->propsTable->item(i,1))
                if( !(ui->propsTable->item(i,0)->text()).isEmpty() && !(ui->propsTable->item(i,1)->text()).isEmpty())
                    propsData.emplace_back(ui->propsTable->item(i,0)->text(), ui->propsTable->item(i,1)->text());
        }

        _settings.props(propsData);

        //4.5. XLSXWrap loading and parsing
        XLSXWrap xw(_xlsx, _filepath, lineCode, lineName, linePrice, propsData, _brands, _mode, catColumn);
        map<QString, vector<Merch*>> parsedMap = xw.getMerchs();


        //5. Loading category renamer
        CategoryRename cr(_mainWin, _filepath, _settings, xw.getMerchCount(), parsedMap);
        cr.setModal(true);
        cr.exec();

        if(cr.success)
            this->close();
    }
}

void LoadPreset::on_buttonSelect_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Выберите файл"), QCoreApplication::applicationDirPath(), "xlsx files (*.xlsx)");
    if(filename.isEmpty())
        return;

    _filepath = filename;

    _xlsx = new QXlsx::Document(_filepath);

    QFileInfo FI(filename);
    ui->labelFilePath->setText(FI.fileName());
    //_settings.filename(FI.fileName());

    if(_settings.search(FI.fileName()))
    {
        qDebug() << "found settings for this file";
        _setup();
    }

    //Preparing ready state
    if(_isReady())
        ui->buttonLoad->setEnabled(true);
    else
        ui->buttonLoad->setEnabled(false);
}

void LoadPreset::on_btn_brandAdd_clicked()
{
    ui->brandsList->setRowCount(ui->brandsList->rowCount() + 1);

    if(_isReady())
        ui->buttonLoad->setEnabled(true);
    else
        ui->buttonLoad->setEnabled(false);
}

void LoadPreset::on_btn_brandDelete_clicked()
{
    int index = ui->brandsList->currentRow();
    ui->brandsList->removeRow(index);

    if(_isReady())
        ui->buttonLoad->setEnabled(true);
    else
        ui->buttonLoad->setEnabled(false);
}

vector<Brand> LoadPreset::_brandsLoader(QTableWidget* table)
{
    vector<Brand> result;
    for(int i=0; i < table->rowCount(); i++)
    {
        //if eng and rus cells are both exists
        if(table->item(i,0) && table->item(i,1))
        {
            //if both are not empty
            if( !(table->item(i,0)->text()).isEmpty() && !(table->item(i,1)->text()).isEmpty() )
                result.emplace_back(table->item(i,0)->text(), table->item(i,1)->text());

            //if eng is empty but rus is not
            else if( (table->item(i,0)->text()).isEmpty() && !(table->item(i,1)->text()).isEmpty() )
                result.emplace_back(EMPTYBRANDNAME, table->item(i,1)->text());

            //if eng is not empty but rus
            else if( !(table->item(i,0)->text()).isEmpty() && (table->item(i,1)->text()).isEmpty() )
                result.emplace_back(table->item(i,0)->text(), EMPTYBRANDNAME);

            //if both are empty - ignore;
        }
        //if eng cell is NULL but rus is exists
        else if(!table->item(i,0) && table->item(i,1))
        {
            if(!(table->item(i,1)->text()).isEmpty())
                result.emplace_back(EMPTYBRANDNAME, table->item(i,1)->text());
        }
        //if rus in NULL but eng is exists
        else if(table->item(i,0) && !table->item(i,1))
        {
            if(!(table->item(i,0)->text()).isEmpty())
                result.emplace_back(table->item(i,0)->text(), EMPTYBRANDNAME);
        }
    }

    //Sorting brands by string.length
    //to avoid issue like: Hotpoint-Ariston & Ariston comparison
    sort(result.begin(), result.end(), Brand::compareLengths);

    return result;
}

void LoadPreset::on_buttonCancel_clicked()
{
    this->close();
}

void LoadPreset::on_radioColumn_toggled(bool checked)
{
    if(checked)
        ui->lineColumn->setEnabled(true);
    else
        ui->lineColumn->setEnabled(false);
}

void LoadPreset::on_radioGroup_toggled(bool checked)
{
    if(checked)
        ui->lineGroup->setEnabled(true);
    else
        ui->lineGroup->setEnabled(false);
}

bool LoadPreset::_isReady()
{
    bool isRadioColumn = ui->radioColumn->isChecked();
    bool isRadioGroup = ui->radioGroup->isChecked();
    bool isRadioPrefix = ui->radioPrefix->isChecked();

    QString lineColumn = ui->lineColumn->text();
    QString lineGroup = ui->lineGroup->text();

    QString lineCode = ui->lineCode->text();
    QString lineName = ui->lineName->text();
    QString linePrice = ui->linePrice->text();

    bool isTableEmpty = ui->brandsList->rowCount() == 0;

    if(_filepath.isEmpty() || isTableEmpty || lineCode.isEmpty() || lineName.isEmpty() || linePrice.isEmpty())
        return false;

    if(!isRadioColumn && !isRadioGroup && !isRadioPrefix)
        return false;

    if(isRadioColumn && lineColumn.isEmpty())
        return false;
    else if(isRadioGroup && lineGroup.isEmpty())
        return false;

    return true;
}

void LoadPreset::_clearSettings()
{
    //CLEARING PRIVIOIUS PRESETS:
    ui->radioColumn->setChecked(true);
    ui->radioGroup->setChecked(false);
    ui->radioPrefix->setChecked(false);

    ui->lineColumn->setText("");
    ui->lineGroup->setText("");

    ui->lineCode->setText("");
    ui->lineName->setText("");
    ui->linePrice->setText("");
    ui->filterTable->clear();
    ui->propsTable->clear();
    ui->brandsList->clear();
}

void LoadPreset::_setup()
{
    //setting up method
    QString method = _settings.method();
    if(method == "column")
    {
        ui->radioColumn->setChecked(true);
        ui->lineColumn->setText(_settings.methodColumn());
    }
    else if(method == "group")
    {
        ui->radioGroup->setChecked(true);
        ui->lineGroup->setText(_settings.methodColumn());
    }
    else if(method == "prefix")
        ui->radioPrefix->setChecked(true);

    //setting up main presets
    ui->lineCode->setText(_settings.code());
    ui->lineName->setText(_settings.name());
    ui->linePrice->setText(_settings.price());

    //setting up filters
    vector<pair<QString, QString>> filterData = _settings.filter();
    ui->filterTable->setRowCount(filterData.size());
    for(int i=0; i < filterData.size(); i++)
    {
        ui->filterTable->setItem(i, 0, new QTableWidgetItem(filterData[i].first));
        ui->filterTable->setItem(i, 1, new QTableWidgetItem(filterData[i].second));
    }

    //setting up properties
    vector<pair<QString, QString>> propsData = _settings.props();
    ui->propsTable->clearContents();
    ui->propsTable->setRowCount(propsData.size());
    for(int i=0; i < propsData.size(); i++)
    {
        ui->propsTable->setItem(i, 0, new QTableWidgetItem(propsData[i].first));
        ui->propsTable->setItem(i, 1, new QTableWidgetItem(propsData[i].second));
    }

    //setting up brands and fill table
    _brands = _settings.brands();
    qDebug() << "Brands size = " << _settings.brands().size();
    ui->brandsList->setRowCount(_brands.size());
    for(int i=0; i < _brands.size(); i++)
    {
        if(_brands[i].eng != EMPTYBRANDNAME)
        {
            QTableWidgetItem* item_eng = new QTableWidgetItem(_brands[i].eng);
            ui->brandsList->setItem(i, 0, item_eng);
        }

        if(_brands[i].rus != EMPTYBRANDNAME)
        {
            QTableWidgetItem* item_rus = new QTableWidgetItem(_brands[i].rus);
            ui->brandsList->setItem(i, 1, item_rus);
        }
    }
}

void LoadPreset::on_lineCode_textEdited(const QString &arg1)
{
    if(_isReady())
        ui->buttonLoad->setEnabled(true);
    else
        ui->buttonLoad->setEnabled(false);
}

void LoadPreset::on_lineName_textEdited(const QString &arg1)
{
    if(_isReady())
        ui->buttonLoad->setEnabled(true);
    else
        ui->buttonLoad->setEnabled(false);
}

void LoadPreset::on_linePrice_textEdited(const QString &arg1)
{
    if(_isReady())
        ui->buttonLoad->setEnabled(true);
    else
        ui->buttonLoad->setEnabled(false);
}

void LoadPreset::on_lineColumn_textEdited(const QString &arg1)
{
    if(_isReady())
        ui->buttonLoad->setEnabled(true);
    else
        ui->buttonLoad->setEnabled(false);
}

void LoadPreset::on_lineGroup_textEdited(const QString &arg1)
{
    if(_isReady())
        ui->buttonLoad->setEnabled(true);
    else
        ui->buttonLoad->setEnabled(false);
}

void LoadPreset::on_radioColumn_clicked()
{
    if(_isReady())
        ui->buttonLoad->setEnabled(true);
    else
        ui->buttonLoad->setEnabled(false);
}

void LoadPreset::on_radioGroup_clicked()
{
    if(_isReady())
        ui->buttonLoad->setEnabled(true);
    else
        ui->buttonLoad->setEnabled(false);
}

void LoadPreset::on_radioPrefix_clicked()
{
    if(_isReady())
        ui->buttonLoad->setEnabled(true);
    else
        ui->buttonLoad->setEnabled(false);
}

void LoadPreset::on_btn_addFilter_clicked()
{
    ui->filterTable->setRowCount(ui->filterTable->rowCount() + 1);
}

void LoadPreset::on_btn_deleteFilter_clicked()
{
    int row = ui->filterTable->currentRow();
    ui->filterTable->removeRow(row);
}

void LoadPreset::on_btn_filter_clicked()
{
    if(_filepath.isEmpty() || ui->filterTable->rowCount() == 0)
        return;

    //Loading values from table
    vector<pair<QString, QString>> filterData;
    for(int i=0; i < ui->filterTable->rowCount(); i++)
    {
        if(ui->filterTable->item(i, 0) && ui->filterTable->item(i, 1))
        {
            if( !(ui->filterTable->item(i, 0)->text()).isEmpty() )
                filterData.emplace_back(ui->filterTable->item(i, 0)->text(), ui->filterTable->item(i, 1)->text());
        }
        else if( ui->filterTable->item(i, 0) && !(ui->filterTable->item(i, 1)) )
        {
            if( !(ui->filterTable->item(i, 0)->text()).isEmpty() )
                filterData.emplace_back(ui->filterTable->item(i, 0)->text(), "");
        }
    }

    _settings.filter(filterData);

    XLSXFilter xf(_xlsx);
    xf.filter(filterData);
    _xlsx = xf.getXLSX();

    QFileInfo fi(_filepath);
    _xlsx->saveAs(QCoreApplication::applicationDirPath() + "/edit-" + fi.fileName());
}

void LoadPreset::on_btn_addProp_clicked()
{
    ui->propsTable->setRowCount(ui->propsTable->rowCount() + 1);
}

void LoadPreset::on_btn_deleteProp_clicked()
{
    int row = ui->propsTable->currentRow();
    ui->propsTable->removeRow(row);
}

void LoadPreset::on_comboSettings_activated(const QString &arg1)
{
    if(_settings.search(ui->comboSettings->currentText()))
        _setup();
}


void LoadPreset::on_btn_brandsFromFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Выберите файл"), QCoreApplication::applicationDirPath(), "text files (*.txt)");
    if(filename.isEmpty())
        return;

    QStringList loadedList;
    if(IO::qread(filename, loadedList, false))
    {
        qDebug() << "red!";

        _brands.clear();
        for(int i=0; i < loadedList.size(); i++)
            _brands.emplace_back(loadedList[i], EMPTYBRANDNAME);

        ui->brandsList->setRowCount(_brands.size());

        for(int i=0; i < _brands.size(); i++)
            ui->brandsList->setItem(i, 0, new QTableWidgetItem(_brands[i].eng));
    }

    QMessageBox::information(this, "Бренды добавлены", "Было добавлено " + QString::number(loadedList.size()) + " брендов!");
}
