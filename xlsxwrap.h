#ifndef XLSXWRAP_H
#define XLSXWRAP_H
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <qtxlsx/xlsxdocument.h>

#include "merch.h"
#include "brand.h"
#include "strf.h"

QTXLSX_USE_NAMESPACE
using namespace std;

bool isnum(QVariant qv);

enum class MODE
{
    COLUMN,
    PREFIX,
    GROUP
};

QString cut_cat(const QString& str, const vector<Brand>& brands, QString& prefix, QString& brand, int& brandIndex);

class XLSXWrap
{
public:
    XLSXWrap(QString filename, QString codeColumn, QString nameColumn, QString priceColumn, vector<Brand> brands, MODE mode, QString catCol = "");
    XLSXWrap(QXlsx::Document* xlsx, QString filename, QString codeColumn, QString nameColumn, QString priceColumn, vector<pair<QString, QString>> propCols, vector<Brand> brands, MODE mode, QString catCol = "");
    ~XLSXWrap();

    map<QString, vector<Merch*> > getMerchs();
    QStringList getCatList();
    int getMerchCount()
    {
        return _merchCount;
    }

private:
    void _makeMerchColumn(Merch* pMerch, vector<Brand> brands);
    void _makeMerchGroup(Merch* pMerch, vector<Brand> brands);
    QString _makeMerchPrefix(Merch* pMerch, vector<Brand> brands);


    QXlsx::Document* _xlsx;
    map<QString, vector<Merch*> > _catMap;

    MODE _mode;
    int _rows;
    int _cols;

    int _merchCount;
};



#endif // XLSXWRAP_H
