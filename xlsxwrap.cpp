#include <QDebug>
#include "xlsxwrap.h"

#include <QFileInfo>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


//XLSXWrap Constructors
XLSXWrap::XLSXWrap(QString filename, QString codeColumn, QString nameColumn, QString priceColumn, vector<Brand> brands, MODE mode, QString catCol)
{
    _xlsx = new QXlsx::Document(filename);

    QFileInfo finfo(filename);

    _rows = _xlsx->dimension().rowCount();
    _cols = _xlsx->dimension().columnCount();
    _mode = mode;

    _merchCount = 0;

    //Parsing category from additional column
    if(_mode == MODE::COLUMN && catCol != "")
    {
        for(int i = 1; i <= _rows; i++)
        {
            QVariant qv_price = _xlsx->read(priceColumn + QString::number(i));
            if(qv_price.isValid() && strf::isnum(qv_price.toString()))
            {
                QString code = _xlsx->read(codeColumn + QString::number(i)).toString();
                QString name = _xlsx->read(nameColumn + QString::number(i)).toString();
                float price = qv_price.toFloat();
                QString category = _xlsx->read(catCol + QString::number(i)).toString();

                Merch* pm = new Merch(code, name, price);
                pm->cat = category;
                pm->filename = finfo.fileName();
                _makeMerchColumn(pm, brands);

                _catMap[category].push_back(pm);
                _merchCount++;
            }
        }
    }
    else if(_mode == MODE::COLUMN && catCol == ""){
        qDebug() << "MODE == COLUMN, but catCol is empty - XLSXWrap constructor";
    }
    //
    //
    //Parsing category from group hierarchy
    else if(_mode == MODE::GROUP && catCol != "")
    {
        QString category = "";
        for(int i = 1; i <= _rows; i++)
        {
            QVariant qv_group = _xlsx->read(catCol + QString::number(i));
            QVariant qv_price = _xlsx->read(priceColumn + QString::number(i));

            //Check group name rule for parsing - name exist but the price dont
            if(qv_group.isValid() && !qv_price.isValid())
            {
                category = qv_group.toString();
                qDebug() << category;
            }
            else if(qv_price.isValid())
            {
                QString code = _xlsx->read(codeColumn + QString::number(i)).toString();
                QString name = _xlsx->read(nameColumn + QString::number(i)).toString();
                float   price = qv_price.toFloat();

                Merch* pm = new Merch(code, name, price);
                pm->cat = category;
                pm->filename = finfo.fileName();
                _makeMerchGroup(pm, brands);

                _catMap[category].push_back(pm);
                _merchCount++;
            }
        }
    }
    //
    //
    //Parsing category from prefix of merch name
    else if(_mode == MODE::PREFIX)
    {
        for(int i = 1; i <= _rows; i++)
        {
            QVariant qv_price = _xlsx->read(priceColumn + QString::number(i));

            if(qv_price.isValid() && strf::isnum(qv_price.toString()))
            {
                QString code = _xlsx->read(codeColumn + QString::number(i)).toString();
                QString name = _xlsx->read(nameColumn + QString::number(i)).toString();
                float price = qv_price.toFloat();

                Merch* pm = new Merch(code, name, price);
                QString cat = _makeMerchPrefix(pm, brands);
                pm->filename = finfo.fileName();

                _catMap[cat].push_back(pm);
                _merchCount++;
            }
        }
    }
}

XLSXWrap::XLSXWrap(QXlsx::Document* xlsx, QString filename, QString codeColumn, QString nameColumn, QString priceColumn, vector<pair<QString, QString>> propCols, vector<Brand> brands, MODE mode, QString catCol)
{
    _xlsx = xlsx;

    QFileInfo finfo(filename);

    _rows = _xlsx->dimension().rowCount();
    _cols = _xlsx->dimension().columnCount();
    _mode = mode;

    _merchCount = 0;

    //Parsing category from additional column
    if(_mode == MODE::COLUMN && catCol != "")
    {
        for(int i = 1; i <= _rows; i++)
        {
            QVariant qv_price = _xlsx->read(priceColumn + QString::number(i));
            if(qv_price.isValid() && strf::isnum(qv_price.toString()))
            {
                QString code = _xlsx->read(codeColumn + QString::number(i)).toString();
                QString name = _xlsx->read(nameColumn + QString::number(i)).toString();
                float price = qv_price.toFloat();
                QString category = _xlsx->read(catCol + QString::number(i)).toString();

                Merch* pm = new Merch(code, name, price);
                pm->cat = category;
                pm->filename = finfo.fileName();
                _makeMerchColumn(pm, brands);

                //Reading properties:
                for(int k=0; k < propCols.size(); k++)
                {
                    QVariant qv_value = _xlsx->read(propCols[k].second + QString::number(i));

                    qDebug() << "property: " + propCols[k].first + " = " << qv_value;

                    pm->addProp(propCols[k].first, qv_value);
                }

                //Adding new merch*
                _catMap[category].push_back(pm);
                _merchCount++;
            }
        }
    }
    else if(_mode == MODE::COLUMN && catCol == ""){
        qDebug() << "MODE == COLUMN, but catCol is empty - XLSXWrap constructor";
    }
    //
    //
    //Parsing category from group hierarchy
    else if(_mode == MODE::GROUP && catCol != "")
    {
        QString category = "";
        for(int i = 1; i <= _rows; i++)
        {
            QVariant qv_group = _xlsx->read(catCol + QString::number(i));
            QVariant qv_price = _xlsx->read(priceColumn + QString::number(i));

            //Check group name rule for parsing - if group cell is valid but price is not or price is not numeric
            if(qv_group.isValid())
            {
                if(!qv_price.isValid() || !strf::isnum(qv_price.toString()))
                    category = qv_group.toString();
            }
            else if(qv_price.isValid() && strf::isnum(qv_price.toString()))
            {
                QString code = _xlsx->read(codeColumn + QString::number(i)).toString();
                QString name = _xlsx->read(nameColumn + QString::number(i)).toString();
                float   price = qv_price.toFloat();

                Merch* pm = new Merch(code, name, price);
                pm->cat = category;
                pm->filename = finfo.fileName();
                _makeMerchGroup(pm, brands);

                //Reading properties:
                for(int k=0; k < propCols.size(); k++)
                {
                    QVariant qv_value = _xlsx->read(propCols[k].second + QString::number(i));

                    qDebug() << "property: " + propCols[k].first + " = " << qv_value;

                    pm->addProp(propCols[k].first, qv_value);
                }

                _catMap[category].push_back(pm);
                _merchCount++;
            }
        }
    }
    //
    //
    //Parsing category from prefix of merch name
    else if(_mode == MODE::PREFIX)
    {
        for(int i = 1; i <= _rows; i++)
        {
            QVariant qv_price = _xlsx->read(priceColumn + QString::number(i));
            if(qv_price.isValid() && strf::isnum(qv_price.toString()))
            {
                QString code = _xlsx->read(codeColumn + QString::number(i)).toString();
                QString name = _xlsx->read(nameColumn + QString::number(i)).toString();
                float price = qv_price.toFloat();

                Merch* pm = new Merch(code, name, price);
                QString cat = _makeMerchPrefix(pm, brands);
                pm->filename = finfo.fileName();

                //Reading properties:
                for(int k=0; k < propCols.size(); k++)
                {
                    QVariant qv_value = _xlsx->read(propCols[k].second + QString::number(i));

                    qDebug() << "property: " + propCols[k].first + " = " << qv_value;

                    pm->addProp(propCols[k].first, qv_value);
                }


                _catMap[cat].push_back(pm);
                _merchCount++;
            }
        }
    }
}

//XLSXWrap destructor
XLSXWrap::~XLSXWrap() {}

//Returns _pMerchs array
map<QString, vector<Merch*>> XLSXWrap::getMerchs()
{
    return _catMap;
}

//Returns QStringList of all categories
QStringList XLSXWrap::getCatList()
{
    QStringList qsl;

    for(auto it = _catMap.begin(); it != _catMap.end(); ++it)
        qsl.push_back(it->first);

    return qsl;
}

//Fills brand and model of single merch in case of column
void XLSXWrap::_makeMerchColumn(Merch* pMerch, vector<Brand> brands)
{
    for(int i = 0; i < brands.size(); i++)
    {
        if(pMerch->name.contains(brands[i].eng, Qt::CaseInsensitive))
        {
            bool cutted_e;
            pMerch->brand = brands[i].eng;
            pMerch->model = strf::cut_after(pMerch->name, brands[i].eng, cutted_e).trimmed();
            if(!cutted_e)
                qDebug() << "ERROR: _makeMerchColumn() -> case COLUMN -> cut_after() = not cutted!";
            break;
        }
        else if(pMerch->name.contains(brands[i].rus, Qt::CaseInsensitive))
        {
            bool cutted_r;
            pMerch->brand = brands[i].rus;
            pMerch->model = strf::cut_after(pMerch->name, brands[i].rus, cutted_r).trimmed();
            if(!cutted_r)
                qDebug() << "ERROR: _makeMerchColumn() -> case COLUMN -> cut_after() = not cutted!";
            break;
        }
    }
}

//Fills brand and model of single merch in case of hierarchy
void XLSXWrap::_makeMerchGroup(Merch* pMerch, vector<Brand> brands)
{
    for(int i = 0; i < brands.size(); i++)
    {
        if(pMerch->name.contains(brands[i].eng, Qt::CaseInsensitive))
        {
            bool cutted_e;
            pMerch->brand = brands[i].eng;
            pMerch->model = strf::cut_after(pMerch->name, brands[i].eng, cutted_e).trimmed();
            if(!cutted_e)
                qDebug() << "ERROR: _makeMerchColumn() -> case COLUMN -> cut_after() = not cutted!";
            break;
        }
        else if(pMerch->name.contains(brands[i].rus, Qt::CaseInsensitive))
        {
            bool cutted_r;
            pMerch->brand = brands[i].eng;
            pMerch->model = strf::cut_after(pMerch->name, brands[i].rus, cutted_r).trimmed();
            if(!cutted_r)
                qDebug() << "ERROR: _makeMerchColumn() -> case COLUMN -> cut_after() = not cutted!";
            break;
        }
    }
}

//Fills category, brand and model of single merch in case of prefix
QString XLSXWrap::_makeMerchPrefix(Merch* pMerch, vector<Brand> brands)
{
    QString category;
    QString brand;
    int brandIndex = 0;

    QString rest = cut_cat(pMerch->name, brands, category, brand, brandIndex);
    pMerch->cat = category.trimmed();
    pMerch->brand = brands[brandIndex].eng;

    bool ismodelCutted;
    QString model = strf::cut_after(rest, brand, ismodelCutted);
    if(ismodelCutted)
        pMerch->model = model.trimmed();
    else
        pMerch->model = "UNKNOWN!";

    return pMerch->cat;
}

//------------------------------------------------------------
//Additional function: Cuts and saves the category from prefix
QString cut_cat(const QString& str, const vector<Brand>& brands, QString& prefix, QString& brand, int& brandIndex)
{
    QString rest;

    for (int i = 0; i < brands.size(); i++)
    {
        bool cutted_e;
        rest = strf::cut_before(str, brands[i].eng, prefix, cutted_e);
        if (cutted_e)
        {
            brand = brands[i].eng;
            brandIndex = i;
            break;
        }
        else
        {
            bool cutted_r;
            rest = strf::cut_before(str, brands[i].rus, prefix, cutted_r);
            if (cutted_r)
            {
                brand = brands[i].rus;
                brandIndex = i;
                break;
            }

        }
    }

    return rest;
}








