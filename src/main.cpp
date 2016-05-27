#include <cstdlib>
#include <memory>

#ifdef HAVE_COLLECTD
 #include "collectd.h"
 #include "common.h" /* auxiliary functions */
 #include "plugin.h" /* plugin_register_*, plugin_dispatch_values */
#endif

#ifdef HAVE_ICE
 #include <Ice/Ice.h>
#endif

#include "Murmur.h"

static const char *config_keys[] =
{
    "IceHost",
    "IcePort",
    "IceSecret"
};
static int config_keys_num = sizeof(config_keys);

int main(int argc, char ** argv)
{
    try {
        auto ic = Ice::initialize(argc, argv);
        auto obj = ic->stringToProxy("Meta:tcp -h 127.0.0.1 -p 6502 -t 1000");
        auto meta = Murmur::MetaPrx::checkedCast(obj);
        if (!meta) {
            return EXIT_FAILURE;
        }
        if (ic) {
            ic->destroy();
        }
    } catch (const Ice::Exception &ex) {
        return EXIT_FAILURE;
    } catch (const char* msg) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
