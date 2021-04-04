#include "filesystem.h"

#include <sago/platform_folders.h>

namespace vca
{

fs::path
user_config_dir()
{
    return fs::u8path(sago::getConfigHome());
}

fs::path
user_documents_dir()
{
    return fs::u8path(sago::getDocumentsFolder());
}

}
