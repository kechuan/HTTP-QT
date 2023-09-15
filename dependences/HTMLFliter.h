#ifndef HTMLFLITER_H
#define HTMLFLITER_H

#include <regex>
#include <string>
#include <QDebug>
#include "enumIndex.h"

inline std::regex DiskReg(R"(\w:)");
inline std::regex HTMLLinkReg(R"(<\s*a\s+[^>]*href\s*=\s*"([^/view/]+[^"]*))", std::regex::icase);                      //target group1
inline std::regex HTMLDiskReg(R"((path=)+(.*))", std::regex::icase);                                                    //target group1
inline std::regex HTMLNameReg(R"(<\s*div\s+[^>]*class\s*=\"(dirname|filename)\">+([^<]*))", std::regex::icase);       //target group2
inline std::regex HTMLSizeReg(R"(<\s*div\s*+[^>]*class\s*=\"filesize\">+([^<]*))", std::regex::icase);                //target group1
inline std::regex HTMLExtReg(R"(\.+(.*)$)");

inline std::regex MenuContentMatch(R"(<ul>+(.*?)+</ul>)");                                                      //target group0
inline std::regex list(R"(<div\s*id=\"list\">)");

inline std::smatch matches;

inline int TypeJudge(std::string& FileNameSuffix){

    std::regex text_fliter (R"(txt|md|ini|json|cfg$)",std::regex::icase);
    std::regex image_fliter (R"(jpe?g|png|bmp|webp$)",std::regex::icase);
    std::regex video_fliter (R"(mkv|mp4|flv|webv|m2ts|rmvb$)",std::regex::icase);
    std::regex audio_fliter (R"(mp3|aac|flac|ogg|opus|aiff|alac|cue$)",std::regex::icase);
    std::regex compress_fliter (R"(zip|rar|7z|iso|\d{3}$)",std::regex::icase);
    std::regex code_fliter (R"(h|c?pp|py|ruby|js|ts$)",std::regex::icase);

    std::array<std::regex,6> fliterArray = {
        text_fliter,
        image_fliter,
        video_fliter,
        audio_fliter,
        compress_fliter,
        code_fliter
    };

    //默认以文件开头
    int fliterIndex = Text;

    while(fliterIndex<fliterArray.size()){
        while (std::regex_search(FileNameSuffix, matches, fliterArray.at(fliterIndex-1))) {
            if(matches[0]!="") {
                return fliterIndex;
            }
        }

        ++fliterIndex;
    }

    return UnknownFile;

}


inline void copy_template(std::string& string, std::regex &regexp, size_t &&matchGroup , std::vector<std::string> &stringVector){
    std::copy(
        std::sregex_token_iterator(string.begin(), string.end(), regexp, matchGroup), //有效迭代器 代表first
        std::sregex_token_iterator(), //空迭代器 代表范围的终止 last
        //相当于旧容器的起始与终止
        std::back_inserter(stringVector) //获取到的值操作
    );
}


inline std::string HTMLFliter(std::string &Information,std::vector<std::string> &PathVector){
    std::smatch matches;

    std::string tempString = std::move(Information);
    std::string MenuContent;

    while (std::regex_search(tempString, matches, MenuContentMatch)) { //修剪并获取信息
        tempString = matches.suffix(); //命中之后就把命中内容从string修剪掉
        MenuContent+=matches[0]; //matches[0] 代表find到的所有东西 同时在新字符串内增加内容 从而实现匹配内容的转移
    }

//    qDebug("MenuContent:%s",MenuContent.c_str());

    copy_template(MenuContent,HTMLLinkReg,1,PathVector);

//    qDebug("Path.size():%zu\n",PathVector.size());
//    for(auto &Path:PathVector) qDebug("Path:%s\n",Path.c_str());

    while (std::regex_search(tempString, matches, list)) {    //仅修剪
        tempString = matches.suffix(); //命中之后就把命中内容从string修剪掉 这里指代<div id="list">开始之外的所有内容
    }

    return tempString;
};

//获取磁盘根目录信息
inline void HTMLExtract(std::string &Information,std::vector<std::string> &LinkVector, std::vector<std::string> &NameVector){

    //reset all content
    LinkVector.clear(); LinkVector.shrink_to_fit();
    NameVector.clear(); NameVector.shrink_to_fit();

    copy_template(Information,HTMLLinkReg,1,LinkVector);

    for(auto &currentLink:LinkVector){
        std::copy( //STL -> copy
            std::sregex_token_iterator(currentLink.begin(), currentLink.end(), HTMLDiskReg, 2),
            std::sregex_token_iterator(),
            std::back_inserter(NameVector)
        );
    }

}

inline void HTMLExtract(std::string &Information,std::vector<std::string> &LinkVector, std::vector<std::string> &PathVector, std::vector<std::string> &NameVector, std::vector<std::string> &SizeVector,std::vector<char> &TypeVector){
    LinkVector.clear(); LinkVector.shrink_to_fit();
    PathVector.clear(); PathVector.shrink_to_fit();
    NameVector.clear(); NameVector.shrink_to_fit();
    SizeVector.clear(); SizeVector.shrink_to_fit();
    TypeVector.clear(); TypeVector.shrink_to_fit();


    std::string FilteredInformation = std::move(HTMLFliter(Information,PathVector));
    copy_template(FilteredInformation,HTMLLinkReg,1,LinkVector);
    copy_template(FilteredInformation,HTMLSizeReg,1,SizeVector);
    copy_template(FilteredInformation,HTMLNameReg,2,NameVector);

    std::vector<std::string> EXTVector;

    for(int SizeIndex = 0;SizeIndex<SizeVector.size();++SizeIndex){
        if(SizeVector.at(SizeIndex)!="—"){
            copy_template(NameVector.at(SizeIndex),HTMLExtReg,1,EXTVector); //获取非"—"的 文件名格式
        }
    }

    int Residual = SizeVector.size() - EXTVector.size();

    for(int fillIndex = 0; fillIndex<Residual; ++fillIndex){
        TypeVector.emplace_back(Dir);
    }

    for(auto &Path:PathVector) qDebug("Path:%s\n",Path.c_str());

    for(auto &Type:EXTVector){
        TypeVector.emplace_back(TypeJudge(Type));
    }

}

#endif
