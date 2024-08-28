#pragma once

#include <assert.h>

namespace utilities {

#define MSG_ASSERT(expr, message) assert((expr) && (message))

}