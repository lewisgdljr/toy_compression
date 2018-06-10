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
#ifndef INTEGER_CODES_HPP
#define INTEGER_CODES_HPP

namespace integer_codes {
   template <typename T>
   using unsigned_of = typename std::make_unsigned_t<T>;

   template <typename T>
   using signed_of = typename std::make_signed_t<T>;

   struct unary {
      template <typename T, typename Iterator,
                typename = std::enable_if_t<std::is_unsigned_v<T>>>
      static void encode( T x, binio::bit_writer<Iterator>& storage ) {
         if ( x == 0 ) {
            throw std::invalid_argument( "unary code can't encode 0" );
         }
         T temp{x};
         while ( temp > 1 ) {
            --temp;
            storage.write_bit( 0 );
         }
         storage.write_bit( 1 );
      }

      template <typename T, typename Iterator, typename Iterator2,
                typename = std::enable_if_t<std::is_unsigned_v<T>>>
      static T decode( binio::bit_reader<Iterator, Iterator2>& storage ) {
         T temp{1};
         while ( !storage.read_bit() ) {
            ++temp;
         }
         return temp;
      }
   };

   struct truncated_binary {
      template <typename T, typename Iterator,
                typename = std::enable_if_t<std::is_unsigned_v<T>>>
      static void encode( T x, T n, binio::bit_writer<Iterator>& storage ) {
         if ( n == 0 ) {
            throw std::invalid_argument(
               "for the truncated binary code, n can't be 0" );
         }

         T    k      = static_cast<T>( std::floor( std::log2( n ) ) );
         T    u      = ( 1 << ( k + 1 ) ) - n;
         bool lesser = x < u;
         x           = lesser ? x : x + u;
         k           = lesser ? k : k + 1;
         storage.write_bits( x, k );
      }

      template <typename T, typename Iterator, typename Iterator2,
                typename = std::enable_if_t<std::is_unsigned_v<T>>>
      static T decode( T n, binio::bit_reader<Iterator, Iterator2>& storage ) {
         if ( n == 0 ) {
            throw std::invalid_argument(
               "for the truncated binary code, n can't be 0" );
         }
         T    k          = static_cast<T>( std::floor( std::log2( n ) ) );
         T    u          = ( 1 << ( k + 1 ) ) - n;
         T    x          = storage.template read_bits<T>( k );
         bool greater_eq = x >= u;
         x = greater_eq ? ( ( x << 1 ) | storage.read_bit() ) - u : x;
         return x;
      }
   };

   struct elias_gamma {
      template <typename T, typename Iterator,
                typename = std::enable_if_t<std::is_unsigned_v<T>>>
      static void encode( T x, binio::bit_writer<Iterator>& storage ) {
         if ( x == 0 ) {
            throw std::invalid_argument( "elias gamma code can't encode 0" );
         }

         T b = 1 + static_cast<T>( std::floor( std::log2( x ) ) );
         unary::template encode<T>( b, storage );
         storage.write_bits( x - ( 1 << ( b - 1 ) ), b - 1 );
      }

      template <typename T, typename Iterator, typename Iterator2,
                typename = std::enable_if_t<std::is_unsigned_v<T>>>
      static T decode( binio::bit_reader<Iterator, Iterator2>& storage ) {
         T b = unary::template decode<T>( storage );
         T x = storage.template read_bits<T>( b - 1 );
         return ( 1 << ( b - 1 ) ) + x;
      }
   };

   struct elias_delta {
      template <typename T, typename Iterator,
                typename = std::enable_if_t<std::is_unsigned_v<T>>>
      static void encode( T x, binio::bit_writer<Iterator>& storage ) {
         if ( x == 0 ) {
            throw std::invalid_argument( "elias delta code can't encode 0" );
         }
         T b = 1 + static_cast<T>( std::floor( std::log2( x ) ) );
         elias_gamma::template encode<T>( b, storage );
         storage.template write_bits<T>( ( x - ( 1 << ( b - 1 ) ) ), b - 1 );
      }

      template <typename T, typename Iterator, typename Iterator2,
                typename = std::enable_if_t<std::is_unsigned_v<T>>>
      static T decode( binio::bit_reader<Iterator, Iterator2>& storage ) {
         T b = elias_gamma::template decode<T>( storage );
         T x = storage.template read_bits<T>( b - 1 );
         return ( 1 << ( b - 1 ) ) + x;
      }
   };

   struct golomb {
      template <typename T, typename Iterator,
                typename = std::enable_if_t<std::is_unsigned_v<T>>>
      static void encode( T x, T b, binio::bit_writer<Iterator>& storage ) {
         if ( ( x == 0 ) || ( b == 0 ) ) {
            throw std::invalid_argument(
               "golomb code can't encode 0, and can't "
               "encode with golomb parameter 0" );
         }

         T q = ( x - 1 ) / b;
         T r = ( x - 1 ) % b;
         unary::template encode<T>( q + 1, storage );
         truncated_binary::template encode<T>( r, b, storage );
      }

      template <typename T, typename Iterator, typename Iterator2,
                typename = std::enable_if_t<std::is_unsigned_v<T>>>
      static T decode( T b, binio::bit_reader<Iterator, Iterator2>& storage ) {
         if ( b == 0 ) {
            throw std::invalid_argument(
               "golomb code can't decode with golomb parameter 0" );
         }

         T q = unary::template decode<T>( storage ) - 1;
         T r = truncated_binary::template decode<T>( b, storage );
         return r + ( q * b ) + 1;
      }
   };

   struct rice {
      template <typename T, typename Iterator,
                typename = std::enable_if_t<std::is_unsigned_v<T>>>
      static void encode( T x, T k, binio::bit_writer<Iterator>& storage ) {
         if ( x == 0 ) {
            throw std::invalid_argument( "rice code can't encode 0" );
         }

         T b = 1 << k;
         golomb::encode( x, b, storage );
      }

      template <typename T, typename Iterator, typename Iterator2,
                typename = std::enable_if_t<std::is_unsigned_v<T>>>
      static T decode( T k, binio::bit_reader<Iterator, Iterator2>& storage ) {
         T b = 1 << k;
         return golomb::template decode<T>( b, storage );
      }
   };
} // namespace integer_codes

#endif // INTEGER_CODES_HPP
