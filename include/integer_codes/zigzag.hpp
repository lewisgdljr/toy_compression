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

namespace {
  template <typename T>
  using enable_enc
  = std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>>;

  template <typename T>
  using enable_dec
  = std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>;
} // namespace

struct zigzag {
  template <typename T, typename = enable_enc<T>>
  constexpr static auto encode( T x ) -> unsigned_of<T> {
    return ( static_cast<unsigned_of<T>>( std::abs( x ) ) << 1 )
      | static_cast<unsigned_of<T>>( x <= 0 );
  }

  template <typename T, typename = enable_dec<T>>
  constexpr static auto decode( const T x ) -> signed_of<T> {
#define BIT_HACK
#ifdef BIT_HACK
    const signed_of<T> sign
      = -static_cast<signed_of<T>>( ( x & 1 ) && ( x != 1 ) );
    const signed_of<T> magnitude = x >> 1;
    return ( magnitude + sign ) ^ sign;
#else
    const T            sign      = ( x & 1 ) && ( x != 1 );
    const signed_of<T> magnitude = x >> 1;
    return sign ? -magnitude : magnitude;
#endif // BIT_HACK
#undef BIT_HACK
  }
};

struct offset_zigzag {
  template <typename T, typename = enable_enc<T>>
  constexpr static auto encode( T x, T offset ) -> unsigned_of<T> {
    return zigzag::encode( x - offset );
  }

  template <typename T, typename = enable_dec<T>>
  constexpr static auto decode( const T x, const signed_of<T> offset )
    -> signed_of<T> {
    return zigzag::decode( x ) + offset;
  }
};
