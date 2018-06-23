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

struct unary {
   template <typename T, typename Iterator,
             typename = std::enable_if_t<std::is_unsigned_v<T>>>
   static void encode( T x, binary_io::bit_writer<Iterator>& storage ) {
      TOY_COMPRESSION_ASSERT( unary::encode, x > 0 );
      T temp{x};
      while ( temp > 1 ) {
         --temp;
         storage.write_bit( 0 );
      }
      storage.write_bit( 1 );
   }

   template <typename T, typename Iterator, typename Iterator2,
             typename = std::enable_if_t<std::is_unsigned_v<T>>>
   static T decode( binary_io::bit_reader<Iterator, Iterator2>& storage ) {
      T temp{1};
      while ( !storage.read_bit() ) {
         ++temp;
      }
      return temp;
   }
};
