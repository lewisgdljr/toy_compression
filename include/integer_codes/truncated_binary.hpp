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

struct truncated_binary {
  template <typename T, typename Iterator,
	    typename = std::enable_if_t<std::is_unsigned_v<T>>>
  static void encode( T x, T n, binary_io::bit_writer<Iterator>& storage ) {
    if ( n == 0 ) {
      throw std::invalid_argument(
				  "for the truncated binary code, n can't be 0" );
    }

    auto k      = static_cast<T>( std::floor( std::log2( n ) ) );
    auto u      = ( static_cast<T>( 1 ) << ( k + 1 ) ) - n;
    bool lesser = x < u;
    x           = lesser ? x : x + u;
    k           = lesser ? k : k + 1;
    storage.write_bits( x, k );
  }

  template <typename T, typename Iterator, typename Iterator2,
	    typename = std::enable_if_t<std::is_unsigned_v<T>>>
  static T decode( T n, binary_io::bit_reader<Iterator, Iterator2>& storage ) {
    if ( n == 0 ) {
      throw std::invalid_argument(
				  "for the truncated binary code, n can't be 0" );
    }
    auto k          = static_cast<T>( std::floor( std::log2( n ) ) );
    auto u          = ( static_cast<T>( 1 ) << ( k + 1 ) ) - n;
    auto x          = storage.template read_bits<T>( k );
    bool greater_eq = x >= u;
    x = greater_eq ? ( ( x << 1 ) | storage.read_bit() ) - u : x;
    return x;
  }
};
