#include <baifenSql.h>

namespace face2wind{

  BaifenSql *BaifenSql::GetInstance() {
    static BaifenSql m;
    return &m;
  }

}
