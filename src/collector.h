/**
 * (c) 2016 David "Danilaw" Soria Parra
 * vim: set ts=2 sts=2 sw=2 expandtab: 
 */
#ifndef __COLLECTOR_H__
#define __COLLECTOR_H__

#ifdef HAVE_ICE
 #include <Ice/Ice.h>
#endif
#include "Murmur.h"

#include <cstdint>
#include <string>
class MumbleCollector {
  public:
    MumbleCollector(std::string _host, uint32_t _port, std::string _secret);
    ~MumbleCollector();
    uint32_t getUserCount();
  private:
    uint32_t port;
    std::string host;
    std::string secret;
    Ice::CommunicatorPtr ice;
    Murmur::MetaPrx connect();
    std::string connectStr();
};

#endif
