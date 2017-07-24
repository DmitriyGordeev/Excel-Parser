#include "xlsxfilter.h"
#include <QDebug>

#include <algorithm>

XLSXFilter::XLSXFilter(const QString& filename)
{
    //Reading data
    QXlsx::Document xlsx(filename);

    _rows = xlsx.dimension().rowCount();
    _cols = xlsx.dimension().columnCount();

    vector<QVariant> rowData;

    for(int i=0; i < _rows; i++)
    {
        for(int j=0; j < _cols; j++)
            rowData.push_back(xlsx.read(i + 1, j + 1));

        _data.push_back(rowData);
        rowData.clear();
    }

    //Fill _columnNums:
    _columnNums["A"] = 1;
    _columnNums["B"] = 2;
    _columnNums["C"] = 3;
    _columnNums["D"] = 4;
    _columnNums["E"] = 5;
    _columnNums["F"] = 6;
    _columnNums["G"] = 7;
    _columnNums["H"] = 8;
    _columnNums["I"] = 9;
    _columnNums["J"] = 10;
    _columnNums["K"] = 11;
    _columnNums["L"] = 12;
    _columnNums["M"] = 13;
    _columnNums["N"] = 14;
    _columnNums["O"] = 15;
    _columnNums["P"] = 16;
    _columnNums["Q"] = 17;
    _columnNums["R"] = 18;
    _columnNums["S"] = 19;
    _columnNums["T"] = 20;
    _columnNums["U"] = 21;
    _columnNums["V"] = 22;
    _columnNums["W"] = 23;
    _columnNums["X"] = 24;
    _columnNums["Y"] = 25;
    _columnNums["Z"] = 26;

}

XLSXFilter::XLSXFilter(QXlsx::Document* xlsx)
{
    //Reading data
    _rows = xlsx->dimension().rowCount();
    _cols = xlsx->dimension().columnCount();

    vector<QVariant> rowData;

    for(int i=0; i < _rows; i++)
    {
        for(int j=0; j < _cols; j++)
            rowData.push_back(xlsx->read(i + 1, j + 1));

        _data.push_back(rowData);
        rowData.clear();
    }


    //Fill _columnNums:
    _columnNums["A"] = 1;
    _columnNums["B"] = 2;
    _columnNums["C"] = 3;
    _columnNums["D"] = 4;
    _columnNums["E"] = 5;
    _columnNums["F"] = 6;
    _columnNums["G"] = 7;
    _columnNums["H"] = 8;
    _columnNums["I"] = 9;
    _columnNums["J"] = 10;
    _columnNums["K"] = 11;
    _columnNums["L"] = 12;
    _columnNums["M"] = 13;
    _columnNums["N"] = 14;
    _columnNums["O"] = 15;
    _columnNums["P"] = 16;
    _columnNums["Q"] = 17;
    _columnNums["R"] = 18;
    _columnNums["S"] = 19;
    _columnNums["T"] = 20;
    _columnNums["U"] = 21;
    _columnNums["V"] = 22;
    _columnNums["W"] = 23;
    _columnNums["X"] = 24;
    _columnNums["Y"] = 25;
    _columnNums["Z"] = 26;

}

void XLSXFilter::filter(vector<pair<QString, QString>> vp)
{
    vector<int> indicies;

    for(int i=0; i < _data.size(); i++)
    {
        if(_filter(vp, i))
        {
            indicies.push_back(i);
            qDebug() << "index = " << i;
        }
    }

    _refreshData(indicies);

}

QXlsx::Document* XLSXFilter::getXLSX()
{
    QXlsx::Document* newXlsx = new QXlsx::Document();

    for(int i=0; i < _data.size(); i++)
       for(int j=0; j < _data[i].size(); j++)
          newXlsx->write(i + 1, j + 1, _data[i][j]);

    int row = newXlsx->dimension().rowCount();
    int col = newXlsx->dimension().columnCount();

    //Debugging:
    for(int i=0; i < row; i++)
        for(int j=0; j < col; j++)
            qDebug() << "new Xlsx: " << newXlsx->read(i + 1, j + 1);

    return newXlsx;
}

bool XLSXFilter::_filter(vector<pair<QString, QString>> vp, int row)
{
    for(int i=0; i < vp.size(); i++)
    {
        if(_filterOne(vp[i], row))
            return true;
    }

    return false;
}

bool XLSXFilter::_filterOne(pair<QString, QString> values, int row)
{
    int col = _columnNums[values.first] - 1;

    QStringList vals = (values.second).split(',');

    for(int i=0; i < vals.size(); i++)
    {
        if(_data[row][col].isValid())
        {
            QString str = (_data[row][col]).toString();

            if(str.toLower() == vals[i].toLower())
                return true;
        }
    }

    return false;
}

void XLSXFilter::_refreshData(vector<int> indicies)
{
    vector<vector<QVariant>> newData;

    qDebug() << "";

    for(int i=0; i < _data.size(); i++)
    {
        if( find(indicies.begin(), indicies.end(), i) == indicies.end() )
        {
            qDebug() << "i = " << i;
            newData.push_back(_data[i]);
        }
    }

    _data.clear();
    _data = newData;
}














