#include "io.h"
#include <iostream>

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <QObject>

bool IO::readFile(std::string filepath, std::vector<std::string>& buffer)
{
    std::ifstream file(filepath);
    if (file.fail())
    {
        perror(filepath.c_str());
        return false;
    }

    std::string line = "";
    while (getline(file, line))
        buffer.push_back(line);

    return true;
}

void IO::writeFile(std::vector<std::string> content, std::string filepath)
{
    std::ofstream file(filepath);
    for (auto it = content.begin(); it != content.end(); ++it)
        file << *it << std::endl;

    std::cout << filepath + ": recorded " + std::to_string(content.size()) + " rows" << std::endl;
}

//qread reads line by line ignoring empty lines
bool IO::qread(QString filename, QStringList& content, bool rootdir)
{
    QString filePath;
    if(rootdir)
        filePath = QCoreApplication::applicationDirPath() + "/" + filename;
    else
        filePath = filename;

    QFile inputFile(QObject::tr(filePath.toStdString().c_str()));
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          if(!line.isEmpty())
            content.push_back(line);
       }
       inputFile.close();
       return true;
    }
    else {
        qDebug() << "IO::qread() - Error!";
        return false;
    }
}

void IO::qwrite(QString filename, QStringList content)
{
    QString filePath = QCoreApplication::applicationDirPath() + "/" + filename;

    QFile fout(QObject::tr(filePath.toStdString().c_str()));
    if (fout.open(QFile::WriteOnly | QFile::Text)) {
       QTextStream s(&fout);
       for (int i = 0; i < content.size(); ++i)
         s << content.at(i) << '\n';
    }
    else {
       qDebug() << "error opening output file\n";
    }
    fout.close();
}
