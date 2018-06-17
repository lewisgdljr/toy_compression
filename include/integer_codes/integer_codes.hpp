/*
 * Toy Compression - Toy Compression Code
 * Written in 2018 by Gerald Lewis <lewisgdljr@gmail.com>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#pragma once

namespace integer_codes {
   template <typename T>
   using unsigned_of = typename std::make_unsigned_t<T>;

   template <typename T>
   using signed_of = typename std::make_signed_t<T>;

#include "unary.hpp"
#include "truncated_binary.hpp"
#include "elias_gamma.hpp"
#include "elias_delta.hpp"
#include "golomb_rice.hpp"
#include "varint.hpp"
#include "zigzag.hpp"

} // namespace integer_codes
