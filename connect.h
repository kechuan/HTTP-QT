#ifndef CONNECT_H
#define CONNECT_H

#include <string>
#include <QObject>


class Connect{
private:
	void Abort();

public:
    bool cliPing(QString& IP,int& Port);

    std::string cliFileSurfing(QString& IP,int& Port);
    std::string cliFileSurfing(QString& IP,int& Port,QString& Postition);

    
    void cliFileDownload(QString& IP,int& Port,QString& Postition,QString& itemName);
    void cliPost(QString& Postition);

};

class HTMLServer{

public:
	void serverCreate(QString& IP);
};

class JsonParse{
private:
    std::string ParseJsonString(std::string& document);
};

//想了想 还是不把它们当作一个类进行存放 直接以组合的形式放到这就好 或者是? 直接作为static声明


#endif
