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
#ifndef TOY_COMPRESSION_HPP_INCLUDED
#define TOY_COMPRESSION_HPP_INCLUDED

#define stringify2( x ) #x
#define stringify( x ) stringify2( x )
#define TOY_COMPRESSION_ASSERT( coder, condition )                \
   if ( !( condition ) ) {                                        \
      throw std::invalid_argument( #coder __FILE__ ":" stringify( \
         __LINE__ ) " => exception(" #condition ") failed." );    \
   }

#include "gsl/gsl"
#include <array>
#include <cmath>
#include <cstdint>
#include <exception>
#include <iostream>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>

namespace toy_compression {

#include "binary_io/binary_io.hpp"
#include "integer_codes/integer_codes.hpp"

} // namespace toy_compression
#endif // TOY_COMPRESSION_HPP_INCLUDED
