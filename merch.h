#pragma once
#ifndef MERCH_H
#define MERCH_H

#endif // MERCH_H


#include <QString>
#include <QVariant>
#include <iostream>
#include <map>

struct Merch
{
    Merch(QString Vcode, QString Name, float Price)
    {
        vcode = Vcode;
        name = Name;
        price = Price;
    }

    QString vcode;
    QString name;
    float price;

    QString cat;
    QString brand;
    QString model;

    QString filename;

    QString compared;

    std::map<QString, QVariant> props;
    void addProp(QString name, QVariant value)
    {
        props[name] = value;
    }

    QStringList getPropNames()
    {
        QStringList names;
        for(auto it = props.begin(); it != props.end(); ++it)
            names.push_back(it->first);
        return names;
    }

    void print(bool defaults = true)
    {
        if(defaults)
            std::cout << "vcode = " << vcode.toStdString() << " | name = " << name.toStdString() << " | price = " << price << std::endl;
        else
            std::cout << "Cat = " << cat.toStdString() << " | Brand = " << brand.toStdString() << " | Model = " << model.toStdString() << std::endl;
    }
};
