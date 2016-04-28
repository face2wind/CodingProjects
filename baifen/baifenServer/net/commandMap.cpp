#include <commandMap.h>

namespace face2wind {

  CommandMap &CommandMap::GetInstance() {
    static CommandMap m;
    return m;
  }

}