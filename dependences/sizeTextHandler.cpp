#include "sizeTextHandler.h"

#include <regex>

using string = std::string;


double StringToSize(std::string &SizeText){

    std::regex storageReg(R"(\d{1,3}.?\d{1,2}+(B|KB|MB|GB)$)");
    std::regex speedReg(R"(\d{1,3}.?\d{1,2}+(B/s|KB/s|MB/s|GB/s)$)");

    std::smatch matches;

    std::regex_search(SizeText, matches, storageReg);

    std::string flitterType;

    if(matches[1].length()){
        flitterType = std::move(matches[1]); //captureList group1
        qDebug("Storage matches Type [1]:%s",matches[1].str().c_str());
    }

    else{
        std::regex_search(SizeText, matches, speedReg);
        flitterType = std::move(matches[1]);
//        qDebug("Speed matches[1]:%s",matches[1].str().c_str());
    }

    
    double fliterSize;

    if(flitterType=="B"||flitterType=="B/s"){
        SizeText = std::move(matches[0]);
        fliterSize = stoi(SizeText);
    }

    else if(flitterType=="KB"||flitterType=="KB/s"){
        SizeText = std::move(matches[0]);
        fliterSize = stod(SizeText);
        fliterSize*=1024;
    }

    else if(flitterType=="MB"||flitterType=="MB/s"){
        SizeText = std::move(matches[0]);
        fliterSize = stod(SizeText);
        fliterSize = fliterSize*1024*1024;
    }

    else if(flitterType=="GB"||flitterType=="GB/s"){
        SizeText = std::move(matches[0]);
        fliterSize = stod(SizeText);
        fliterSize = fliterSize*1024*1024*1024;
    }

   return fliterSize;
}


std::string SizeToString(double &SpeedValue){
	std::string itemSpeed;

    if(SpeedValue>1024*1024*1024){
        SpeedValue = SpeedValue/1024/1024/1024;
        itemSpeed = std::to_string(SpeedValue).substr(0,6)+"GB/s";
    }

    else if(SpeedValue>1024*1024){
        SpeedValue = SpeedValue/1024/1024;
        itemSpeed = std::to_string(SpeedValue).substr(0,6)+"MB/s";

    }

    else if(SpeedValue>1024){
        SpeedValue = SpeedValue/1024;
        itemSpeed = std::to_string(SpeedValue).substr(0,6)+"KB/s";

    }

    else{
        itemSpeed = std::to_string(SpeedValue).substr(0,6)+"B/s";
    }

    return itemSpeed;
}
