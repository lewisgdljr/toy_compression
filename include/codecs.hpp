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
#ifndef CODECS_HPP_INCLUDED
#define CODECS_HPP_INCLUDED

#include <type_traits>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <limits>
#include <utility>
#include <exception>

namespace toy_compression {

#ifndef BINIO_HPP
#include "binio.hpp"
#endif // BINIO_HPP

#ifndef INTEGER_CODES_HPP
#include "integer_codes.hpp"
#endif // INTEGER_CODES_HPP

#ifndef ZIGZAG_HPP
#include "zigzag.hpp"
#endif //ZIGZAG_HPP

}
#endif // CODECS_HPP_INCLUDED
