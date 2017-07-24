#ifndef IO_H
#define IO_H

#pragma once
#include <vector>
#include <string>
#include <fstream>

#include <QString>

class IO
{
public:
    static bool readFile(std::string filepath, std::vector<std::string>& buffer);
    static void writeFile(std::vector<std::string> content, std::string filepath);
    static bool qread(QString filename, QStringList& content, bool rootdir = true);
    static void qwrite(QString filename, QStringList content);
};

#endif // IO_H
