// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#ifndef NDEBUG
#  include <cassert>
#endif // NDEBUG

#ifdef NDEBUG
#  define DEBUG_ONLY(code)
#else
#  define DEBUG_ONLY(code) code
#endif // NDEBUG

#define DEBUG_ASSERT(predicate) DEBUG_ONLY(assert(predicate))
