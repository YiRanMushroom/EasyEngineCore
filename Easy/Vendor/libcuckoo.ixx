module;

#include <algorithm>
#include <array>
#include <atomic>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

import Easy.Core.Basic;

#include <libcuckoo/cuckoohash_map.hh>

export module Easy.Vendor.libcuckoo;

namespace libcuckoo {
    export using libcuckoo::cuckoohash_map;
}