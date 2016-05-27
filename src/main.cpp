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

static const char * NAME = "mumble";

static const char *config_keys[] =
{
    "IceHost",
    "IcePort",
    "IceSecret"
};
static int config_keys_num = sizeof(config_keys);

MumbleCollector* collector = nullptr;

#ifdef HAVE_COLLECTD
static int my_config (const char *key, const char *value)
{
    if (collector) {
//        collector->setProperty(std::string(key), std::string(value));
    }
    return 0;
}

static int my_init (void)
{
    collector = new MumbleCollector();
    return 0;
}
 
static int my_shutdown(void)
{
    if (collector) {
        delete collector;
    }
    return 0;
}

static int my_read (void)
{
    value_t values[3];
    value_list_t vl = VALUE_LIST_INIT;
    vl.values = values;
    vl.values_len = sizeof(values);
    sstrncpy (vl.host, hostname_g, sizeof (vl.host));
    sstrncpy (vl.plugin, "load", sizeof (vl.plugin));
    sstrncpy (vl.type, "load", sizeof (vl.type));
    auto count = collector->getUserCount();
    std::cout << count << std::endl;
    plugin_dispatch_values (&vl);
    return 0;
}

extern "C" {
    void module_register ()
    {
        plugin_register_config (NAME, my_config, config_keys, config_keys_num);
        plugin_register_init (NAME, my_init);
        plugin_register_shutdown (NAME, my_shutdown);
        plugin_register_read (NAME, my_read);
    }
}

#else

int main (int argc,char**argv) {
    MumbleCollector collector;
    std::cout << collector.getUserCount() << std::endl;
    return 0;
}
#endif

