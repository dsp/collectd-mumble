/**
 * (c) 2016 David "Danilaw" Soria Parra
 * vim: set ts=2 sts=2 sw=2 expandtab: 
 */
#include "collector.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

MumbleCollector::MumbleCollector(const std::string _host, const uint32_t _port,
                                 const std::string _secret) :
    host(_host),
    port(_port),
    secret(_secret) {

  auto props = Ice::createProperties();
  props->setProperty("Ice.ImplicitContext", "Shared");

  Ice::InitializationData idata;
  idata.properties = props;

  this->ice = Ice::initialize(idata);
}

std::string MumbleCollector::connectStr() const {
  std::ostringstream stringStream;
  stringStream << "Meta:tcp ";
  stringStream << "-h " << this->host << " ";
  stringStream << "-p " << this->port << " ";
  stringStream << "-t 1000";
  return stringStream.str();
}

/**
 * Connect to the Mumble server via the ICE protocl and initialize a Meta class
 * which is then
 * returned. This meta class can be used to further examine the data on the
 * server, such as enumerating
 * servers.
 */
Murmur::MetaPrx MumbleCollector::connect() {

  if (this->secret.size() > 0) {
    this->ice->getImplicitContext()->put("secret", this->secret);
  }

  auto obj = this->ice->stringToProxy(this->connectStr());
  auto meta = Murmur::MetaPrx::checkedCast(obj);
  if (!meta) {
    throw connection_exp;
  }
  return meta;
}

MumbleCollector::~MumbleCollector() {
  if (this->ice) {
    this->ice->destroy();
  }
}

uint32_t MumbleCollector::getUserCount() {
  try {
    auto meta = this->connect();
    for (auto& server : meta->getBootedServers()) {
      return static_cast<uint32_t>(server->getUsers().size());
    }
    return 0;
  } catch (ConnectionException& e) {
    return 0;
  }
}
