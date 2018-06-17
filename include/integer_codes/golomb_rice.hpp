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

struct golomb {
  template <typename T, typename Iterator,
	    typename = std::enable_if_t<std::is_unsigned_v<T>>>
  static void encode( T x, T b, binary_io::bit_writer<Iterator>& storage ) {
    if ( ( x == 0 ) || ( b == 0 ) ) {
      throw std::invalid_argument(
				  "golomb code can't encode 0, and can't "
				  "encode with golomb parameter 0" );
    }

    auto q = ( x - 1 ) / b;
    auto r = ( x - 1 ) % b;
    unary::template encode<T>( q + 1, storage );
    truncated_binary::template encode<T>( r, b, storage );
  }

  template <typename T, typename Iterator, typename Iterator2,
	    typename = std::enable_if_t<std::is_unsigned_v<T>>>
  static T decode( T b, binary_io::bit_reader<Iterator, Iterator2>& storage ) {
    if ( b == 0 ) {
      throw std::invalid_argument(
				  "golomb code can't decode with golomb parameter 0" );
    }

    auto q = unary::template decode<T>( storage ) - 1;
    auto r = truncated_binary::template decode<T>( b, storage );
    return r + ( q * b ) + 1;
  }
};

struct rice {
  template <typename T, typename Iterator,
	    typename = std::enable_if_t<std::is_unsigned_v<T>>>
  static void encode( T x, T k, binary_io::bit_writer<Iterator>& storage ) {
    if ( x == 0 ) {
      throw std::invalid_argument( "rice code can't encode 0" );
    }

    auto b = static_cast<T>( 1 ) << k;
    golomb::encode( x, b, storage );
  }

  template <typename T, typename Iterator, typename Iterator2,
	    typename = std::enable_if_t<std::is_unsigned_v<T>>>
  static T decode( T k, binary_io::bit_reader<Iterator, Iterator2>& storage ) {
    auto b = static_cast<T>( 1 ) << k;
    return golomb::template decode<T>( b, storage );
  }
};
