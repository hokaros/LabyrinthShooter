

#include <iostream>
#include "client.h"
#include "Server.h"

struct a {
    int b;
    int c;
}d;


int main()
{
    
    connection::Message msg;
    int x = 5;
    msg += x;
    int y;
    msg >> y;
    
    d.b = 100;
    d.c = 200;

    msg += d;

    int c2, b2;
    msg >> c2;
    msg >> b2;

    msg.messageHeader.messageId = Id::GameStarted;
    std::cout << c2;
    std::cout << y;

    struct gameInfo {
        float zm1;
        int zm2;
        double zm3;
    }tmp;

    tmp.zm1 = 1.0f;
    tmp.zm2 = 3;
    tmp.zm3 = 4.01f;

    float a;
    int b;
    double c;
    
    msg += tmp;
  //  msg >> c;
    //std::cout <<std::endl<< c;
   
    
    connection::Client client;
    //client.Connect("123", 90);

    client.Connect("51.38.81.49", 80);
    system("pause");

    client.sendMessage(msg);

    system("pause");
}


