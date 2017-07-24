#pragma once
#ifndef STRF_H
#define STRF_H

#include <string>
#include <algorithm>
#include <QStringList>

class strf
{
public:
    static bool isnum(QString str);
    static QString cut_before(const QString& str, const QString& word, QString& prefix, bool& cutted);
    static QString cut_after(const QString& str, const QString& word, bool& cutted);
    static QString notrash(const QString& str, const QString& trash);
    static QString nophrases(const QString& str, const QStringList& phrases);
    static bool compare(const QString& str1, const QString& str2, const QString& trash, const QStringList& phrases);

    static bool compareLengths(const QString& s1, const QString& s2);
};

#endif // STRF_H
