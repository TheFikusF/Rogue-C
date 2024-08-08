#include "ASSERT.h"

void my_assert(bool condition, std::string msg) {
    if(condition == false) {
        LOG_ERROR(msg);
    }
}