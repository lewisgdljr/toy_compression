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
      TOY_COMPRESSION_ASSERT( truncated_binary::encode, n >= 1 && x <= n );
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
      TOY_COMPRESSION_ASSERT( truncated_binary::decode, n >= 1 );
      auto k          = static_cast<T>( std::floor( std::log2( n ) ) );
      auto u          = ( static_cast<T>( 1 ) << ( k + 1 ) ) - n;
      auto x          = storage.template read_bits<T>( k );
      bool greater_eq = x >= u;
      x = greater_eq ? ( ( x << 1 ) | storage.read_bit() ) - u : x;
      return x;
   }
};

struct centered_truncated_binary {
   template <typename T, typename Iterator,
             typename = std::enable_if_t<std::is_unsigned_v<T>>>
   static void encode( T x, T n, binary_io::bit_writer<Iterator>& storage ) {
      TOY_COMPRESSION_ASSERT( centered_truncated_binary, n >= 1 );
      TOY_COMPRESSION_ASSERT( centered_truncated_binary,
                              ( 1 <= x ) && ( x <= n ) );

      auto const top = static_cast<T>( 1 )
                       << static_cast<T>( std::ceil( std::log2( n ) ) );
      auto const offset   = n - ( top >> 1 );
      auto const centered = ( n + x - offset ) % n;
      truncated_binary::encode( centered, n, storage );
   }

   template <typename T, typename Iterator, typename Iterator2,
             typename = std::enable_if_t<std::is_unsigned_v<T>>>
   static T decode( T n, binary_io::bit_reader<Iterator, Iterator2>& storage ) {
      TOY_COMPRESSION_ASSERT( centered_truncated_binary, n >= 1 );
      auto const top = static_cast<T>( 1 )
                       << static_cast<T>( std::ceil( std::log2( n ) ) );
      auto const offset            = n - ( top >> 1 );
      auto const partially_decoded = truncated_binary::decode( n, storage );
      auto       decoded           = ( partially_decoded + offset ) % n;
      return decoded;
   }
};

struct binary_in_range {
   template <typename T, typename Iterator,
             typename = std::enable_if_t<std::is_unsigned_v<T>>>
   static void encode( T x, T low, T high,
                       binary_io::bit_writer<Iterator>& storage ) {
      TOY_COMPRESSION_ASSERT( binary_in_range, low >= 1 && low < high );
      TOY_COMPRESSION_ASSERT( binary_in_range, x >= low && x <= high );
      centered_truncated_binary::encode( x - low + 1, high - low + 1, storage );
   }

   template <typename T, typename Iterator, typename Iterator2,
             typename = std::enable_if_t<std::is_unsigned_v<T>>>
   static T decode( T low, T high,
                    binary_io::bit_reader<Iterator, Iterator2>& storage ) {
      TOY_COMPRESSION_ASSERT( binary_in_range, low >= 1 );
      TOY_COMPRESSION_ASSERT( binary_in_range, low < high );
      return centered_truncated_binary::decode( high - low + 1, storage ) + low
             - 1;
   }
};
