#include "kun/core/functional/log.h"
#include <spdlog/spdlog.h>

namespace kun::__log
{
void implLogMsg(String log) { spdlog::info(log); }
void implLogWarn(String log) { spdlog::warn(log); }
void implLogErr(String log) { spdlog::error(log); }
}// namespace kun::__log