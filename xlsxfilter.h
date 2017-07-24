#ifndef XLSXFILTER_H
#define XLSXFILTER_H

#include <qtxlsx/xlsxdocument.h>
#include <QStringList>
#include <vector>
#include <map>
using namespace std;


class XLSXFilter
{
public:
    XLSXFilter(const QString& filename);
    XLSXFilter(QXlsx::Document* xlsx);
    void filter(vector<pair<QString, QString>> vp);

    QXlsx::Document* getXLSX();
    void save(const QString& filename);

private:
    bool _filter(vector<pair<QString, QString>> vp, int row);
    bool _filterOne(pair<QString, QString> values, int row);
    void _refreshData(vector<int> indicies);

    map<QString, int> _columnNums;
    vector<vector<QVariant>> _data;

    int _rows;
    int _cols;

};

#endif // XLSXFILTER_H
