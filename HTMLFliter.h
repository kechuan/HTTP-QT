#ifndef HTMLFLITER_H
#define HTMLFLITER_H

#include <regex>
#include <string>
#include <QDebug>

void copy_template(std::string& string, std::regex &regexp, size_t &&matchGroup , std::vector<std::string> &vector){
    std::copy(
        std::sregex_token_iterator(string.begin(), string.end(), regexp, matchGroup),
        std::sregex_token_iterator(),
        std::back_inserter(vector)
    );
}

std::regex HTMLLinkReg(R"(<\s*a\s+[^>]*href\s*=\s*"([^/view/]+[^"]*))", std::regex::icase); //target group1
std::regex HTMLDiskReg(R"((path=)+(.*))", std::regex::icase);                               //target group1
std::regex HTMLNameReg(R"(<\s*div\s+[^>]*class\s*=\"(dirname|filename)\">+([^<]*))", std::regex::icase);       //target group2
std::regex HTMLSizeReg(R"(<\s*div\s*+[^>]*class\s*=\"filesize\">+([^<]*))", std::regex::icase);                //target group1

std::regex MenuContentMatch(R"!!(<ul>+(.*?)+</ul>)!!");                                     //*target group0
std::regex list(R"(<div\s*id=\"list\">)");

std::smatch matches;

std::string HTMLFliter(std::string &Information,std::vector<std::string> &PathVector){
    std::string tempString = std::move(Information);
    std::string MenuContent;

    while (std::regex_search(tempString, matches, MenuContentMatch)) { //修剪并获取信息
        tempString = matches.suffix(); //命中之后就把命中内容从string修剪掉
        MenuContent+=matches[0]; //matches[0] 代表find到的所有东西 同时在新字符串内增加内容 从而实现匹配内容的转移
    }

//    copy_template(MenuContent.begin(),MenuContent.end(),HTMLLinkReg,1,PathVector);
    copy_template(MenuContent,HTMLLinkReg,1,PathVector);

    // qDebug("Path.size():%zu\n",PathVector.size());
    // for(auto &Path:PathVector) qDebug("Path:%s\n",Path.c_str());

    while (std::regex_search(tempString, matches, list)) {    //仅修剪
        tempString = matches.suffix(); //命中之后就把命中内容从string修剪掉 这里指代<div id="list">开始之外的所有内容
    }

    return tempString;
};


void HTMLExtract(std::string &Information,std::vector<std::string> &LinkVector, std::vector<std::string> &NameVector){

    //reset all content
    LinkVector.clear();
    NameVector.clear();

    copy_template(Information,HTMLLinkReg,1,LinkVector);

    for(auto &i:LinkVector){
        std::copy( //STL -> copy
            std::sregex_token_iterator(i.begin(), i.end(), HTMLDiskReg, 2),
            std::sregex_token_iterator(),
            std::back_inserter(NameVector)
        );
    }

//    for(auto &Link:LinkVector) qDebug("Link:%s",Link.c_str()); //不要直接使用 for(iter:<容器>) 因为会导致一次拷贝构造
//    for(auto &Disk:NameVector) qDebug("Disk:%s",Disk.c_str());

    
}

void HTMLExtract(std::string &Information,std::vector<std::string> &LinkVector, std::vector<std::string> &PathVector, std::vector<std::string> &NameVector, std::vector<std::string> &SizeVector){

    //reset all content
    LinkVector.clear();
    PathVector.clear();
    NameVector.clear();
    SizeVector.clear();

    //Fliter real Information.
    std::string FilteredInformation = std::move(HTMLFliter(Information,PathVector));
    copy_template(FilteredInformation,HTMLLinkReg,1,LinkVector);

//    for(auto &Link:LinkVector) qDebug("Link:%s",Link.c_str()); //不要直接使用 for(iter:<容器>) 因为会导致一次拷贝构造
    copy_template(FilteredInformation,HTMLSizeReg,1,SizeVector);

//    for(auto &Size:SizeVector) qDebug("Size:%s",Size.c_str());
    copy_template(FilteredInformation,HTMLNameReg,2,NameVector);
    
    
//    for(auto &Name:NameVector) qDebug("Name:%s",Name.c_str());
    
    qDebug("the length of SizeVector:%zu",SizeVector.size());
    qDebug("the length of NameVector:%zu",NameVector.size());

}

#endif
