#pragma once
#ifndef BRAND_H
#define BRAND_H

#endif // BRAND_H

#include <QString>

#define EMPTYBRANDNAME "EMPTYBRANDNAME"

struct Brand
{
    Brand(QString Eng, QString Rus)
    {
        eng = Eng;
        rus = Rus;
    }

    QString join(QString sep)
    {
        return eng + sep + rus;
    }

    QString rus;
    QString eng;

    static bool compareLengths(const Brand& b1, const Brand& b2)
    {
        if(b1.eng.length() >= b2.eng.length())
            return true;
        return false;
    }

};



