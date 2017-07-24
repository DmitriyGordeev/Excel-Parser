#include "settingsparser.h"
#include "io.h"

#include <algorithm>
#include <QFileInfo>

bool SettingsParser::search(QString filename)
{
    QStringList settings;
    IO::qread("settings.txt", settings);

    //Clearing previous data, if exists
    clear();

    int index = settings.indexOf(filename);
    if(index != -1)
    {
        //Splitted methodStr to method and methodColumn:
        QString  methodStr = settings.at(index + 1);
        QStringList methodData = methodStr.split(' ');
        if(methodData.size() == 2)
        {
            _method = methodData[0];
            _methodColumn = methodData[1];
        }
        else if(methodData.size() == 1)
        {
            _method = methodData[0];
            _methodColumn = "";
        }


        _codeColumn = settings.at(index + 2);
        _nameColumn = settings.at(index + 3);
        _priceColumn = settings.at(index + 4);


        //Getting filter data
        QString filterStr = settings.at(index + 5);
        if(filterStr != EMPTYFILTER)
        {
            QStringList filters = filterStr.split(';');
            for(int i=0; i < filters.size(); i++)
            {
                QStringList col_and_exps = filters[i].split(':');
                _filterData.emplace_back(col_and_exps[0], col_and_exps[1]);
            }
        }


        //Getting properties data
        QString propsStr = settings.at(index + 6);
        if(propsStr != EMPTYPROPS)
        {
            QStringList props = propsStr.split(',');
            for(int i=0; i < props.size(); i++)
            {
                QStringList name_and_col = props[i].split('.');
                _propsData.emplace_back(name_and_col[0], name_and_col[1]);
            }
        }


        //Getting brands
        QString brands = settings.at(index + 7);
        QStringList brandsList = brands.split(',');
        for(int i=0; i < brandsList.size(); i++)
        {
            QStringList brandData = brandsList[i].split('.');
            if(brandData.size() == 1)
                _brands.emplace_back(brandData[0], EMPTYBRANDNAME);
            else if(brandData.size() == 2)
                _brands.emplace_back(brandData[0], brandData[1]);
        }


        //Getting categories
        QString categories = settings.at(index + 8);
        _categories = categories.split(',');

        return true;
    } else
        return false;
}

void SettingsParser::save()
{
    QStringList settings;
    IO::qread("settings.txt", settings);
    int index = settings.indexOf(_filename);
    //if such file already exists in the settings.txt:
    if(index != -1)
    {
        settings[index] = _filename;

        if(_methodColumn.isEmpty())
            settings[index + 1] = _method;
        else
            settings[index + 1] = _method + " " + _methodColumn;

        settings[index + 2] = _codeColumn;
        settings[index + 3] = _nameColumn;
        settings[index + 4] = _priceColumn;

        //Setting filterData:
        QStringList filters;
        for(int i=0; i < _filterData.size(); i++)
            filters.push_back(_filterData[i].first + ":" + _filterData[i].second);
        QString filterStr = filters.join(';');
        if(filterStr.isEmpty())
            settings[index + 5] = EMPTYFILTER;
        else
            settings[index + 5] = filterStr;

        //Setting propsData:
        QStringList props;
        for(int i=0; i < _propsData.size(); i++)
            props.push_back(_propsData[i].first + "." + _propsData[i].second);
        QString propsStr = props.join(',');
        if(propsStr.isEmpty())
            settings[index + 6] = EMPTYPROPS;
        else
            settings[index + 6] = propsStr;

        //Transforming vector<Brand> to QString for record
        QStringList brandsList;
        for(int i=0; i < _brands.size(); i++)
            brandsList.push_back(_brands[i].join("."));

        settings[index + 7] = brandsList.join(',');
        settings[index + 8] = _categories.join(',');

        IO::qwrite("settings.txt", settings);
    }
    else
    {
        QStringList newSettings;
        newSettings << "fileseparation" << _filename;

        if(_methodColumn.isEmpty())
            newSettings << _method;
        else
            newSettings << _method + " " + _methodColumn;

        newSettings << _codeColumn << _nameColumn << _priceColumn;

        //Setting filterData:
        QStringList filters;
        for(int i=0; i < _filterData.size(); i++)
            filters.push_back(_filterData[i].first + ":" + _filterData[i].second);
        QString filterStr = filters.join(';');
        if(filterStr.isEmpty())
            newSettings << EMPTYFILTER;
        else
            newSettings << filterStr;

        //Setting propsData:
        QStringList props;
        for(int i=0; i < _propsData.size(); i++)
            props.push_back(_propsData[i].first + "." + _propsData[i].second);
        QString propsStr = props.join(',');
        if(propsStr.isEmpty())
            newSettings << EMPTYPROPS;
        else
            newSettings << propsStr;


        //Transforming vector<Brand> to QString for record
        QStringList brandsList;
        for(int i=0; i < _brands.size(); i++)
            brandsList.push_back(_brands[i].join("."));

        newSettings << brandsList.join(',') << _categories.join(',');

        settings.append(newSettings);
        IO::qwrite("settings.txt", settings);
    }
}

void SettingsParser::save(QString filename, QString method, QString methodColumn, QString code, QString name, QString price, vector<pair<QString, QString>> filterData, vector<pair<QString, QString>> propsData, vector<Brand> brands, QStringList cats)
{
    QStringList settings;
    IO::qread("settings.txt", settings);

    int index = settings.indexOf(filename);
    //if such file already exists in th settings.txt:
    if(index != -1)
    {
        if(methodColumn.isEmpty())
            settings[index + 1] = _method;
        else
            settings[index + 1] = _method + " " + methodColumn;
        settings[index + 2] = code;
        settings[index + 3] = name;
        settings[index + 4] = price;

        //Setting filterData:
        QStringList filters;
        for(int i=0; i < filterData.size(); i++)
            filters.push_back(filterData[i].first + ":" + filterData[i].second);
        QString filterStr = filters.join(';');
        if(filterStr.isEmpty())
            settings[index + 5] = EMPTYFILTER;
        else
            settings[index + 5] = filterStr;

        //Setting propsData:
        QStringList props;
        for(int i=0; i < propsData.size(); i++)
            props.push_back(propsData[i].first + "." + propsData[i].second);
        QString propsStr = props.join(',');
        if(propsStr.isEmpty())
            settings[index + 6] = EMPTYPROPS;
        else
            settings[index + 6] = propsStr;

        //Transforming vector<Brand> to QString for record
        QStringList brandsList;
        for(int i=0; i < brands.size(); i++)
            brandsList.push_back(brands[i].join("."));

        settings[index + 7] = brandsList.join(',');
        settings[index + 8] = cats.join(',');

        IO::qwrite("settings.txt", settings);
    }
    else
    {
        QStringList newSettings;
        newSettings << "fileseparation" << _filename;

        if(_methodColumn.isEmpty())
            newSettings << _method;
        else
            newSettings << _method + " " + _methodColumn;

        newSettings << _codeColumn << _nameColumn << _priceColumn;

        //Setting filterData:
        QStringList filters;
        for(int i=0; i < filterData.size(); i++)
            filters.push_back(filterData[i].first + ":" + filterData[i].second);
        QString filterStr = filters.join(';');
        if(filterStr.isEmpty())
            newSettings << EMPTYFILTER;
        else
            newSettings << filterStr;

        //Setting propsData:
        QStringList props;
        for(int i=0; i < propsData.size(); i++)
            props.push_back(propsData[i].first + "." + propsData[i].second);
        QString propsStr = props.join(',');
        if(propsStr.isEmpty())
            newSettings << EMPTYPROPS;
        else
            newSettings << propsStr;

        //Transforming vector<Brand> to QString for record
        QStringList brandsList;
        for(int i=0; i < brands.size(); i++)
            brandsList.push_back(brands[i].join("."));

        newSettings << brandsList.join(',') << cats.join(',');

        settings.append(newSettings);
        IO::qwrite("settings.txt", settings);
    }
}

QStringList SettingsParser::getFilesList()
{
    //Getting all files, that already have settings:
    QStringList settings;
    QStringList filenames;
    IO::qread("settings.txt", settings);
    for(int i=0; i < settings.size(); i++)
    {
        if(settings[i] == "fileseparation")
            filenames.push_back(settings[i + 1]);
    }

    return filenames;
}

void SettingsParser::clear()
{
    _filename = "";
    _method = "";
    _methodColumn = "";
    _nameColumn = "";
    _codeColumn = "";
    _priceColumn = "";
    _filterData.clear();
    _propsData.clear();
    _brands.clear();
    _categories.clear();
}

//Setters:
void SettingsParser::filename(QString filename)
{
    _filename = filename;
}

void SettingsParser::method(QString data)
{
    _method = data;
}

void SettingsParser::methodColumn(QString data)
{
    _methodColumn = data;
}

void SettingsParser::code(QString data)
{
    _codeColumn = data;
}

void SettingsParser::name(QString data)
{
    _nameColumn = data;
}

void SettingsParser::price(QString data)
{
    _priceColumn = data;
}

void SettingsParser::filter(vector<pair<QString, QString>> filterData)
{
    _filterData = filterData;
}

void SettingsParser::props(vector<pair<QString, QString>> propsData)
{
    _propsData = propsData;
}

void SettingsParser::brands(vector<Brand> data)
{
    _brands = data;
}

void SettingsParser::cats(QStringList data)
{
    _categories = data;
}

//Getters:
QString SettingsParser::filename()
{
    return _filename;
}

QString SettingsParser::method()
{
    return _method;
}

QString SettingsParser::methodColumn()
{
    return _methodColumn;
}

QString SettingsParser::code()
{
    return _codeColumn;
}

QString SettingsParser::name()
{
    return _nameColumn;
}

QString SettingsParser::price()
{
    return _priceColumn;
}

vector<pair<QString, QString>> SettingsParser::filter()
{
    return _filterData;
}

vector<pair<QString, QString>> SettingsParser::props()
{
    return _propsData;
}

vector<Brand> SettingsParser::brands()
{
    return _brands;
}

QStringList SettingsParser::cats()
{
    return _categories;
}
