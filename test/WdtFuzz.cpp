/**
 * Copyright (c) 2014-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#include <wdt/Wdt.h>

#include <thread>

using namespace std;

namespace facebook {
namespace wdt {
}  // namespace wdt
}  // namespace facebook

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {

  std::string q((const char*)data, size);
  facebook::wdt::WdtUri uri(q);

  return 0;
}
