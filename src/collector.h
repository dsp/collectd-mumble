#ifndef __COLLECTOR_H__
#define __COLLECTOR_H__

#ifdef HAVE_ICE
 #include <Ice/Ice.h>
#endif
#include "Murmur.h"

class MumbleCollector {
  public:
    MumbleCollector();
    ~MumbleCollector();
    void setProperty(std::string key, std::string value);
    int getUserCount();
  private:
    int port;
    std::string host;
    std::string secret;
    Ice::CommunicatorPtr ice;
    Murmur::MetaPrx connect();
};

#endif
