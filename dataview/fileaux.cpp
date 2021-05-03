
#include "fileaux.h"
#include <QMessageBox>
#include <QString>
#include <QFileInfo>
#include <string>
#include <vector>
#include <fstream>
#include <pwutils/pwstrings.h>
#include <pwutils/pwmath.h>
#include <parambin.hpp>

namespace fileaux{

void openFile(const std::string& fname,std::ifstream& infile)
{
    infile.open(fname.c_str());
    if(!infile.is_open()){
        QMessageBox::information(nullptr,"error",
          QString("Failed to open the file %1").arg(QString::fromStdString(fname)));
    }
    return;
}

ParamBin getHeaderContent(std::ifstream& infile) {
    ParamBin bin;
    std::string line_feed;
    int oldpos = infile.tellg();
    while(std::getline(infile,line_feed)){
       line_feed = pw::eatWhiteSpace(line_feed);
       if(line_feed.empty()){
           oldpos = infile.tellg();
           continue;
       } else if(line_feed[0] == '#') {
           int char_count = pw::countCharacters(line_feed,':');
           if(char_count != 1)
               continue;
           std::vector<std::string> parsed_param = pw::parseString(line_feed,':');
           std::string param_name = parsed_param[0];
           param_name = pw::eatWhiteSpace(param_name," \t#");
           std::string param_val = parsed_param[1];
           param_val = pw::eatWhiteSpace(param_val);
           bin.set(param_name,param_val);
           oldpos = infile.tellg();
           continue;
       } else{
           infile.seekg(oldpos);
           return bin;
       }
    }
    return bin;
}

std::string headerSignature(std::ifstream& infile)
{
    ParamBin bin(getHeaderContent(infile));
    if(bin.inBin("file_type"))
        return bin.getStr("file_type");
    else
        return "unknown";
}

std::string dataSignature(std::ifstream& infile)
{
    ParamBin bin(getHeaderContent(infile));
    std::vector<std::string> first_line;
    getLineOfData(infile,first_line);
    if(pw::rowIsDoubles(first_line)){
       std::vector<std::string> second_line;
       getLineOfData(infile,second_line);
       if(pw::rowIsDoubles(second_line)){
           std::vector<std::string> third_line;
           getLineOfData(infile,third_line);
           if(pw::rowIsDoubles(third_line)){
               if(first_line.size() == second_line.size() &&
                       second_line.size() == third_line.size()){
                   if(first_line.size() == 2){
                       if(bin.inBin("logy"))
                           return "two_col_logy_data";
                       else
                           return "two_col_data";
                   }
                   else if(first_line.size() == 3)
                       return "three_col_data";
                   else
                       return "many_col_data";
               }
               return "unknown";
           }
       }
    }
    return "unknown";
}

std::string readSignature(const std::string& fname)
{
    std::ifstream infile;
    openFile(fname,infile);
    std::string signature = headerSignature(infile);
    if(signature == "unknown"){
        infile.clear();
        infile.seekg(0,std::ios_base::beg);
        signature = dataSignature(infile);
    }
    infile.close();
    return signature;
}

void getLineOfData(std::ifstream& infile,std::vector<std::string>& line_data)
{
    std::string line_feed;
    while(std::getline(infile,line_feed)){
       line_feed = pw::eatWhiteSpace(line_feed);
       // Ignore empty lines and comment lines
       if(line_feed.empty())
           continue;
       else if(line_feed[0] == '#')
           continue;
       else{
           // BINGO
           line_data = pw::parseString(line_feed,' ');
           return;
       }
    }
}

void eatFileHeaders(std::ifstream& infile) {
    std::string line_feed;
    int oldpos = infile.tellg();
    while(std::getline(infile,line_feed)){
       line_feed = pw::eatWhiteSpace(line_feed);
       if(line_feed.empty() || line_feed[0] == '#'){
           oldpos = infile.tellg();
           continue;
       }
       infile.seekg(oldpos);
       return;
    }
}

ParamBin readTwoColDoubles(const std::string& fname,std::vector<double>& x,std::vector<double>& y){
    std::ifstream infile;
    openFile(fname,infile);
    ParamBin bin = fileaux::getHeaderContent(infile);
    while(!infile.eof()){
        double a,b;
        infile >> a >> b;
        x.push_back(a);
        y.push_back(b);
    }
    return bin;
}


QString getLocalFileName(const QString& full_filename)
{
    QFileInfo file_info(full_filename);
    return file_info.fileName();
}

}
