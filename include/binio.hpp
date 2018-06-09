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
#ifndef BINIO_HPP
#define BINIO_HPP

#include <cstdint>
#include <iostream>
#include <iterator>
#include <limits>
#include <utility>

namespace binio {
   template <typename Iterator, typename Iterator2>
   struct bit_reader {
      Iterator      pos;
      Iterator2     end;
      std::uint8_t  buf;
      std::uint64_t total_count;
      unsigned int  bits_left;

      bit_reader( Iterator&& begin_, Iterator2&& end_ ) :
         pos{begin_},
         end{end_},
         buf{0},
         total_count{0},
         bits_left{0} {}

      void input_byte() {
         if ( pos == end ) {
            throw std::out_of_range(
               "Attempt to read bits beyond end of range" );
         }
         bits_left = std::numeric_limits<std::uint8_t>::digits;
         buf       = *pos;
         ++pos;
      }

      bool read_bit() {
         if ( bits_left == 0 ) {
            input_byte();
         }
         ++total_count;
         --bits_left;
         return ( buf >> bits_left ) & 1;
      }

      template <typename T>
      T read_bits( unsigned int bits ) {
         using UT = std::make_unsigned_t<T>;
         UT temp{0};
         for ( unsigned int i = 0; i < bits; i++ ) {
            temp = ( temp << 1 ) | read_bit();
         }
         return temp;
      }
   };

   template <typename Iterator, typename Iterator2,
             typename
             = typename std::iterator_traits<Iterator>::iterator_category>
   auto make_bit_reader( Iterator&& begin, Iterator2&& end )
      -> bit_reader<Iterator, Iterator2> {
      return bit_reader<Iterator, Iterator2>{std::forward<Iterator>( begin ),
                                             std::forward<Iterator2>( end )};
   }

   template <typename Container,
             typename = decltype( std::declval<Container>().cbegin() )>
   auto make_bit_reader( Container const& c ) {
      return make_bit_reader( c.cbegin(), c.cend() );
   }

   template <typename stream_t>
   auto make_bit_reader( stream_t& stream ) -> decltype(
      make_bit_reader( std::istream_iterator<unsigned char>( stream ),
                       std::istream_iterator<unsigned char>() ) ) {
      return make_bit_reader( std::istream_iterator<unsigned char>( stream ),
                              std::istream_iterator<unsigned char>() );
   }

   template <typename Iterator>
   struct bit_writer {
      Iterator      pos;
      std::uint8_t  buf;
      std::uint64_t total_count;
      unsigned int  bits_left;
      bit_writer( Iterator&& begin_ ) :
         pos{std::forward<Iterator>( begin_ )},
         buf{0},
         total_count{0},
         bits_left{std::numeric_limits<std::uint8_t>::digits} {}
      ~bit_writer() {
         if ( bits_left != std::numeric_limits<std::uint8_t>::digits ) {
            flush();
         }
      }
      bit_writer( bit_writer const& )  = delete;
      bit_writer( bit_writer const&& ) = delete;
      operator=( bit_writer const& ) = delete;
      operator=( bit_writer const&& ) = delete;

      void output_byte() {
         *pos = buf;
         buf  = 0;
         ++pos;
         bits_left = std::numeric_limits<std::uint8_t>::digits;
      }

      void write_bit( bool bit ) {
         --bits_left;
         ++total_count;
         buf |= ( static_cast<std::uint8_t>( bit ) << bits_left );
         if ( bits_left == 0 ) {
            output_byte();
         }
      }

      template <typename T, typename UT = std::make_unsigned_t<T>>
      void write_bits( T value, unsigned int num_bits ) {
         for ( int i = num_bits; i > 0; --i ) {
            UT mask = static_cast<UT>( 1 ) << ( i - 1 );
            write_bit( ( static_cast<UT>( value ) & mask ) != 0 );
         }
      }

      void flush() { output_byte(); }
   };

   template <typename Iterator,
             typename
             = typename std::iterator_traits<Iterator>::iterator_category>
   auto make_bit_writer( Iterator&& begin ) -> bit_writer<Iterator> {
      return bit_writer<Iterator>{std::forward<Iterator>( begin )};
   }

   template <typename Container,
             typename = decltype( std::declval<Container>().begin() )>
   auto make_bit_writer( Container&& c ) {
      return make_bit_writer( std::back_inserter( c ) );
   }

   template <typename stream_t>
   auto make_bit_writer( stream_t& stream ) -> decltype(
      make_bit_writer( std::ostream_iterator<unsigned char>( stream ) ) ) {
      return make_bit_writer( std::ostream_iterator<unsigned char>( stream ) );
   }
} // namespace binio

#endif // BINIO_HPP
