/**
 * (c) 2016 David "Danilaw" Soria Parra
 * vim: set ts=2 sts=2 sw=2 expandtab: 
 */
#include "collector.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

MumbleCollector::MumbleCollector(std::string _host, uint32_t _port,
                                 std::string _secret) {
  auto props = Ice::createProperties();
  props->setProperty("Ice.ImplicitContext", "Shared");

  Ice::InitializationData idata;
  idata.properties = props;

  this->ice = Ice::initialize(idata);
  this->port = _port;
  this->host = _host;
  this->secret = _secret;
}

std::string MumbleCollector::connectStr() {
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
    return nullptr;
  }
  return meta;
}

MumbleCollector::~MumbleCollector() {
  if (this->ice) {
    this->ice->destroy();
  }
}

uint32_t MumbleCollector::getUserCount() {
  auto meta = this->connect();
  for (auto server : meta->getBootedServers()) {
    return static_cast<uint32_t>(server->getUsers().size());
  }
  return 0;
}
