#ifndef MERCHSORTER_H
#define MERCHSORTER_H

#include "merch.h"
#include <vector>
#include <map>

#include <QStringList>

using namespace std;

class MerchSorter
{
public:
    MerchSorter() {}
    void fill(const vector<vector<Merch*> >& data, const QString& trash, const QStringList& phrases);

    vector<Merch*> mainAnalys(int& totalCompared);


private:
    Merch* _compare(Merch* first, Merch* second);
    vector<Merch*> _compareTwoV(const vector<Merch*>& fv, const vector<Merch*>& sv, int& countCompared);

    vector<vector<Merch*> > _data;
    QString _trash;
    QStringList _phrases;
};

#endif // MERCHSORTER_H
