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

    
    void cliFileDownload(QString& IP,int& Port,QString& Postition);
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


#endif
