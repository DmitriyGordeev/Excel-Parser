#include "merchsorter.h"
#include "strf.h"
#include <algorithm>

void MerchSorter::fill(const vector<vector<Merch*>>& data, const QString& trash, const QStringList& phrases)
{
    _data = data;
    _trash = trash;
    _phrases = phrases;
}

vector<Merch*> MerchSorter::mainAnalys(int& totalCompared)
{
    vector<Merch*> result = _data.at(0);
    totalCompared = 0;

    for (int i = 1; i < _data.size(); i++)
        result = _compareTwoV(result, _data.at(i), totalCompared);

    return result;
}

vector<Merch*> MerchSorter::_compareTwoV(const vector<Merch*>& a, const vector<Merch*>& b, int& countCompared)
{
    vector<Merch*> c_a = a;
    vector<Merch*> c_b = b;

    countCompared;

    for(int i=0; i < a.size(); i++)
    {
        for(int j=0; j < b.size(); j++)
        {
            Merch* p = _compare(a[i], b[j]);
            if(p == a[i])
            {
                countCompared++;
                c_b[j] = nullptr;
                break;
            }
            else if(p == b[j])
            {
                countCompared++;
                c_a[i] = nullptr;
                break;
            }
        }
    }

    c_a.erase(remove(c_a.begin(), c_a.end(), nullptr), c_a.end());
    c_b.erase(remove(c_b.begin(), c_b.end(), nullptr), c_b.end());

    c_a.insert(c_a.end(), c_b.begin(), c_b.end());

    return c_a;
}

Merch* MerchSorter::_compare(Merch* first, Merch* second)
{
    if((first->cat).toLower() == (second->cat).toLower())
    {
        if((first->brand).toLower() == (second->brand).toLower())
        {
            if(strf::compare(first->model, second->model, _trash, _phrases))
            {
                first->compared = "compared with " + second->brand + " " + second->model;
                second->compared = "compared with " + first->brand + " " + first->model;

                if(first->price <= second->price)
                    return first;
                else
                    return second;
            }
        }
    }

    return nullptr;
}
