#include "strf.h"
#include <iostream>

//if string is num?
bool strf::isnum(QString str)
{
    return str.remove(QRegExp("[0-9\.,]")).isEmpty();
}

//Cuting and save prefix before "word"
QString strf::cut_before(const QString& str, const QString& word, QString& prefix, bool& cutted)
{
    QString rest;
    prefix.clear();

    //Maybe deleting trash here!!!
    int pos = str.toLower().indexOf(word.toLower());
    if(pos != -1)
    {
        for(int i=0; i < pos; i++)
            prefix.push_back(str[i]);

        for(int i=pos; i < str.size(); i++)
            rest.push_back(str[i]);

        cutted = true;

    } else
        cutted = false;

    return rest;
}

//Save the rest of the std::string after "word"
QString strf::cut_after(const QString& str, const QString& word, bool& cutted)
{
     QString rest;

     int pos = str.toLower().indexOf(word.toLower());
     if(pos != -1)
     {
         for(int i = pos + word.size(); i < str.size(); i++)
             rest.push_back(str[i]);

         cutted = true;
     } else {
         cutted = false;
     }

     return rest;
}

//Delete unwanted symbols from QString
QString strf::notrash(const QString& str, const QString& trash)
{
    QString out = str;

    for(int i=0; i < trash.size(); i++)
        out.remove(trash[i], Qt::CaseInsensitive);

    return out;
}

//Delete unwanted phrases from QString
QString strf::nophrases(const QString& str, const QStringList& phrases)
{
    QString out = str;

    for(int i=0; i < phrases.size(); i++)
        out.remove(phrases[i]);

    return out;
}

//Comparing method
bool strf::compare(const QString& str1, const QString& str2, const QString& trash, const QStringList& phrases)
{
    if (str1.toLower() == str2.toLower())
        return true;
    else
    {
        if (nophrases(notrash(str1, trash), phrases).toLower() == nophrases(notrash(str2, trash), phrases).toLower())
            return true;
        else
            return false;
    }
}

//predicate for qSort() - sorting QStrings by length:
bool strf::compareLengths(const QString& s1, const QString& s2)
{
    if(s1.length() >= s2.length())
        return true;

    return false;
}
