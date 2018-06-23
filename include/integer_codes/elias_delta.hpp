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

struct elias_delta {
   template <typename T, typename Iterator,
             typename = std::enable_if_t<std::is_unsigned_v<T>>>
   static void encode( T x, binary_io::bit_writer<Iterator>& storage ) {
     TOY_COMPRESSION_ASSERT(elias_delta::encode, x >= 1);
      auto b = 1 + static_cast<T>( std::floor( std::log2( x ) ) );
      elias_gamma::template encode<T>( b, storage );
      storage.template write_bits<T>( ( x - ( 1 << ( b - 1 ) ) ), b - 1 );
   }

   template <typename T, typename Iterator, typename Iterator2,
             typename = std::enable_if_t<std::is_unsigned_v<T>>>
   static T decode( binary_io::bit_reader<Iterator, Iterator2>& storage ) {
      auto b = elias_gamma::template decode<T>( storage );
      auto x = storage.template read_bits<T>( b - 1 );
      return ( 1 << ( b - 1 ) ) + x;
   }
};
