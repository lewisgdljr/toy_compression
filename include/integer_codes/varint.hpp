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

struct varint {
   template <typename T, typename Iterator,
             typename = std::enable_if_t<std::is_unsigned_v<T>>>
   static void encode( T x, binary_io::bit_writer<Iterator>& storage ) {
      constexpr int digits = std::numeric_limits<T>::digits;
      // a varint can't get bigger than this, really
      constexpr auto bytes_to_reserve
         = static_cast<std::size_t>( digits + 6 ) / 7;
      std::array<std::uint8_t, bytes_to_reserve> temp_buffer{};
      auto pos{bytes_to_reserve - 1}; // last byte
      gsl::at( temp_buffer, pos-- ) = static_cast<std::uint8_t>( x & 127 );

      while ( x >>= 7 ) {
         --x;
         gsl::at( temp_buffer, pos-- )
            = static_cast<std::uint8_t>( 128 | ( x & 127 ) );
      }
      for ( pos++; pos < bytes_to_reserve; pos++ ) {
         storage.template write_bits<std::uint8_t>( gsl::at( temp_buffer, pos ),
                                                    8 );
      }
   }

   template <typename T, typename Iterator, typename Iterator2,
             typename = std::enable_if_t<std::is_unsigned_v<T>>>
   static T decode( binary_io::bit_reader<Iterator, Iterator2>& storage ) {
      T            output_val       = 0;
      std::uint8_t continuation_val = 0;
      do {
         auto val         = storage.template read_bits<std::uint8_t>( 8 );
         continuation_val = val & 0x80;
         val &= 0x7f;
         if ( continuation_val ) {
            ++val;
         }
         output_val = ( output_val << 7 ) + val;
      } while ( continuation_val );
      return output_val;
   }
};
