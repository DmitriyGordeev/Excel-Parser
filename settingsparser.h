#ifndef SETTINGSPARSER_H
#define SETTINGSPARSER_H
#include <QtCore>
#include "xlsxwrap.h"
#include "brand.h"

#define EMPTYFILTER "EMPTYFILTER"
#define EMPTYPROPS "EMPTYPROPS"

class SettingsParser
{
public:
    SettingsParser() {}

    bool search(QString filename);
    void save();
    void save(QString filename, QString method, QString methodColumn, QString code, QString name, QString price, vector<pair<QString, QString>> filterData, vector<pair<QString, QString>> propsData, vector<Brand> brands, QStringList cats);

    QStringList getFilesList();

    QString filename();
    QString method();
    QString methodColumn();
    QString code();
    QString name();
    QString price();
    vector<pair<QString, QString>> filter();
    vector<pair<QString, QString>> props();
    vector<Brand> brands();
    QStringList cats();

    void filename(QString filename);
    void method(QString data);
    void methodColumn(QString data);
    void code(QString data);
    void name(QString data);
    void price(QString data);
    void filter(vector<pair<QString, QString>> filterData);
    void props(vector<pair<QString, QString>> propsData);
    void brands(vector<Brand> data);
    void cats(QStringList data);

    void clear();

private:
    QString _filename;
    QString _method;
    QString _methodColumn;
    QString _codeColumn;
    QString _nameColumn;
    QString _priceColumn;
    vector<pair<QString, QString>> _filterData;
    vector<pair<QString, QString>> _propsData;
    vector<Brand> _brands;
    QStringList _categories;

};

#endif // SETTINGSPARSER_H
