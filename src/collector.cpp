#include "collector.h"
#include <cassert>
#include <cstdlib>
MumbleCollector::MumbleCollector(
        ) {
    auto props = Ice::createProperties();
    props->setProperty("Ice.ImplicitContext", "Shared");

    Ice::InitializationData idata;
    idata.properties = props;

    this->ice = Ice::initialize(idata);
    this->port = 6502;
    this->host = "127.0.0.1";
    this->secret = "";
}

void MumbleCollector::setProperty(
        std::string key,
        std::string value
        ) {
    if (key == "port") {
        auto port = strtol(key.c_str(), NULL, 10);
        assert(port > 0 && port < 65536);
        this->port = port;
    }

    if (key == "host") {
        this->host = value;
    }

    if (key == "secret") {
        this->secret = value;
    }
}

Murmur::MetaPrx MumbleCollector::connect(
        ) {

    this->ice->getImplicitContext()->put("secret", "foobar");

    auto obj = this->ice->stringToProxy("Meta:tcp -h 127.0.0.1 -p 6502 -t 1000");
    auto meta = Murmur::MetaPrx::checkedCast(obj);
    if (!meta) {
        return nullptr;
    }
    return meta;
}

MumbleCollector::~MumbleCollector(
        ) {
    if (this->ice) {
        this->ice->destroy();
    }
}

int MumbleCollector::getUserCount() {
    auto meta = this->connect();
    for (auto server : meta->getBootedServers()) {
        return server->getUsers().size();
    }
    return 0;
}
