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
static std::string ice_host = "127.0.0.1";
static std::string ice_secret = "";
static uint32_t ice_port = 6502;
static int config_keys_num = sizeof(config_keys);

MumbleCollector *collector = nullptr;

#ifdef HAVE_COLLECTD
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

static int my_init(void) {
  collector = new MumbleCollector(ice_host, ice_port, ice_secret);
  return 0;
}

static int my_shutdown(void) {
  if (collector) {
    delete collector;
  }
  return 0;
}

static int my_read(void) {
  value_t values[1];
  value_list_t vl = VALUE_LIST_INIT;

  values[0].gauge = collector->getUserCount();

  vl.values = values;
  vl.values_len = STATIC_ARRAY_SIZE(values);
  sstrncpy(vl.host, hostname_g, sizeof(vl.host));
  sstrncpy(vl.plugin, PLUGIN_NAME, sizeof(vl.plugin));
  sstrncpy(vl.type, "count", sizeof(vl.type));
  plugin_dispatch_values(&vl);
  return 0;
}

/**
 * Definition for the datatype to be used by collectd
 */
static data_source_t mumble_dsrc[] = {{"count", DS_TYPE_GAUGE, 0, NAN}};
static data_set_t mumble_ds = {PLUGIN_NAME, STATIC_ARRAY_SIZE(mumble_dsrc),
                               mumble_dsrc};

extern "C" {
void module_register() {
  plugin_register_data_set(&mumble_ds);
  plugin_register_config(PLUGIN_NAME, my_config, config_keys, config_keys_num);
  plugin_register_init(PLUGIN_NAME, my_init);
  plugin_register_shutdown(PLUGIN_NAME, my_shutdown);
  plugin_register_read(PLUGIN_NAME, my_read);
}
}

#else

int main(int argc, char **argv) {
  MumbleCollector collector;
  std::cout << collector.getUserCount() << std::endl;
  return 0;
}
#endif
