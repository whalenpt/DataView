#ifndef FILEAUX_H
#define FILEAUX_H

#include <QString>
#include <string>
#include <fstream>
#include <ParamBin/parambin.hpp>
#include <vector>

namespace fileaux{
    void openFile(const std::string& fname,std::ifstream& infile);
    ParamBin getHeaderContent(std::ifstream& infile);
    std::string headerSignature(std::ifstream& infile);
    std::string dataSignature(std::ifstream& infile);
    std::string readSignature(const std::string& fname);
    void eatFileHeaders(std::ifstream& infile);
    void getLineOfData(std::ifstream& infile,std::vector<std::string>& first);
    ParamBin readTwoColDoubles(const std::string& fname,std::vector<double>& x,std::vector<double>& y);
    QString getLocalFileName(const QString& full_filename);
}

#endif // FILEAUX_H
