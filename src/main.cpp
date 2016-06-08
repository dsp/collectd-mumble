/**
 * (c) 2016 David "Danilaw" Soria Parra
 * vim: set ts=2 sts=2 sw=2 expandtab: 
 */
#ifdef HAVE_COLLECTD
extern "C" {
#define HAVE_ENDIAN_H 1
#include <endian.h>
#include <stdint.h>
#include <stdbool.h>
#include <collectd/liboconfig/oconfig.h>
#include <collectd/core/daemon/collectd.h>
#include <collectd/core/daemon/common.h> /* auxiliary functions */
#include <collectd/core/daemon/plugin.h> /* plugin_register_*, plugin_dispatch_values */
}
#endif

#include <cstdlib>
#include <memory>
#include <string>

#include "collector.h"

// static const char * PLUGIN_NAME = "mumble";
#define PLUGIN_NAME "mumble"

static const char *config_keys[] = {"IceHost", "IcePort", "IceSecret"};
static std::string ice_host("127.0.0.1");
static std::string ice_secret("");
static uint32_t ice_port = 6502;
static int config_keys_num = sizeof(config_keys);

static int my_config(const char *key, const char *value) {
  if (strcmp(key, "IcePort") == 0) {
    auto mport = strtol(key, nullptr, 10);
    assert(mport > 0 && mport < 65536);
    ice_port = static_cast<uint32_t>(mport);
    return 0;
  }

  if (strcmp(key, "IceHost") == 0) {
    ice_host = std::string(value);
    return 0;
  }

  if (strcmp(key, "IceSecret") == 0) {
    ice_secret = std::string(value);
    return 0;
  }
  return 1;
}

static void submit_value(
    const std::string plugin_instance,
    const std::string type, 
    gauge_t value) {
  value_t values[1];
  value_list_t vl = VALUE_LIST_INIT;

  values[0].gauge = value;

  vl.values = values;
  vl.values_len = STATIC_ARRAY_SIZE(values);
  sstrncpy(vl.host, hostname_g, sizeof(vl.host));
  sstrncpy(vl.plugin, PLUGIN_NAME, sizeof(vl.plugin));
  sstrncpy(vl.type, type.c_str(), sizeof(vl.type));
  if (plugin_instance.size() > 0) {
    sstrncpy(vl.plugin_instance, plugin_instance.c_str(),
        sizeof(vl.plugin_instance));
  }
  plugin_dispatch_values(&vl);
}

static int my_read(void) {
  try {
    MumbleCollector collector(ice_host, ice_port, ice_secret);
    auto meta = collector.connect();
    for (auto& server : meta->getBootedServers()) {
      auto id = std::to_string(server->id());
      submit_value(id, "users_connected",
          static_cast<gauge_t>(server->getUsers().size()));
    }
  } catch (ConnectionException const& e) {
    return 1;
  }

  return 0;
}

/**
 * Definition for the datatype to be used by collectd
 */
static data_source_t mumble_dsrc[] = {{"value", DS_TYPE_GAUGE, 0, NAN}};
static data_set_t mumble_ds = {"users_connected", STATIC_ARRAY_SIZE(mumble_dsrc),
                               mumble_dsrc};

extern "C" {
void module_register() {
  plugin_register_data_set(&mumble_ds);
  plugin_register_config(PLUGIN_NAME, my_config, config_keys, config_keys_num);
  plugin_register_read(PLUGIN_NAME, my_read);
}
}
