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

#include "toy_compression.hpp"
#include "testing.hpp"
#include <cstdint>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <vector>

using container = std::vector<std::uint8_t>;
using iterator  = typename container::iterator;

using test_type        = unsigned;
using signed_test_type = int;

using namespace toy_compression;

using binary_io::make_bit_reader;
using binary_io::make_bit_writer;

/* subsystem test functions... */
void test_unary_coder( test_type value ) {
   container c;
   {
      auto writer = make_bit_writer( c );
      integer_codes::unary::encode( value, writer );
   }
   auto reader  = make_bit_reader( c );
   auto decoded = integer_codes::unary::decode<test_type>( reader );
   ASSERT( decoded == value );
}

void test_truncated_binary_coder( test_type value, test_type n ) {
   container c;
   {
      auto writer = make_bit_writer( c );
      integer_codes::truncated_binary::encode( value, n, writer );
   }
   auto reader = make_bit_reader( c );
   auto decoded
      = integer_codes::truncated_binary::decode<test_type>( n, reader );
   ASSERT( decoded == value );
}

void test_elias_gamma_coder( test_type value ) {
   container c;
   {
      auto writer = make_bit_writer( c );
      integer_codes::elias_gamma::encode( value, writer );
   }
   auto reader  = make_bit_reader( c );
   auto decoded = integer_codes::elias_gamma::decode<test_type>( reader );
   ASSERT( decoded == value );
}

void test_elias_delta_coder( test_type value ) {
   container c;
   {
      auto writer = make_bit_writer( c );
      integer_codes::elias_delta::encode( value, writer );
   }
   auto reader  = make_bit_reader( c );
   auto decoded = integer_codes::elias_delta::decode<test_type>( reader );
   ASSERT( decoded == value );
}

void test_golomb( test_type x, test_type b ) {
   container c;
   {
      auto writer = make_bit_writer( c );
      integer_codes::golomb::encode( x, b, writer );
   }
   auto reader = make_bit_reader( c );
   auto result = integer_codes::golomb::template decode<test_type>( b, reader );
   ASSERT( result == x );
}

void test_rice( test_type x, test_type b ) {
   container c;
   {
      auto writer = make_bit_writer( c );
      integer_codes::rice::encode( x, b, writer );
   }
   auto reader = make_bit_reader( c );
   auto result = integer_codes::rice::template decode<test_type>( b, reader );
   ASSERT( result == x );
}

void test_zigzag( signed_test_type value ) {
   auto encoded = integer_codes::zigzag::encode( value );
   auto decoded = integer_codes::zigzag::decode( encoded );
   ASSERT( decoded == value );
}

void test_offset_zigzag( signed_test_type value, signed_test_type offset ) {
   auto encoded = integer_codes::offset_zigzag::encode( value, offset );
   auto decoded = integer_codes::offset_zigzag::decode( encoded, offset );
   ASSERT( decoded == value );
}

void test_varint( test_type value ) {
   container c;
   {
      auto writer = make_bit_writer( c );
      integer_codes::varint::encode( value, writer );
   }
   auto reader  = make_bit_reader( c );
   auto decoded = integer_codes::varint::decode<test_type>( reader );
   ASSERT( decoded == value );
}

/* suites */
toy_test::suite const reader_suite{
   "Test for bit_reader",
   {
      {"bit_reader can be created from a pair of iterators",
       [] {
          container c;
          (void)make_bit_reader( c.begin(), c.end() );
          ASSERT( true );
       }},

      {"bit_reader can be created from a container",
       [] {
          container c;
          (void)make_bit_reader( c );
          ASSERT( true );
       }},

#ifdef DISABLE_TO_AVOID_WARNING
      {"bit_reader can be created from an istream",
       [] {
          std::istringstream ss;
          (void)make_bit_reader( ss );
          ASSERT( true );
       }},
#endif // DISABLE_TO_AVOID_WARNING

      {"bit_reader throws exception when read from empty",
       [] {
          container c{};
          auto      reader = make_bit_reader( c );
          THROWS( reader.read_bit(), std::out_of_range );
       }},

      {"bit_reader.read_bit() returns the first available bit",
       [] {
          container c{0x70};
          auto      reader = make_bit_reader( c );
          ASSERT( reader.read_bit() == 0 );
       }},

      {"bit_reader.read_bits() returns multiple bits",
       [] {
          container c{0x70, 0x0f, 0x0};
          auto      reader = make_bit_reader( c );
          (void)reader.read_bit();
          auto temp = reader.read_bits<test_type>( 16 );
          ASSERT( temp == 0xe01e );
       }},

   }};

toy_test::suite const writer_suite{
   "Test for bit_writer",
   {
      {"bit_writer can be created from an iterator",
       [] {
          container c;
          (void)make_bit_writer( std::back_inserter( c ) );
          ASSERT( true );
       }},

      {"bit_writer can be created from a container",
       [] {
          container c;
          (void)make_bit_writer( c );
          ASSERT( true );
       }},

#ifdef DISABLE_TO_AVOID_WARNING
      {"bit_writer can be created from an ostream",
       [] {
          std::ostringstream ss;
          {
             auto writer = make_bit_writer( ss );
             for ( int i = 0; i < 10; i++ ) {
                writer.write_bit( true );
             }
          }
          { (void)make_bit_writer( ss ); }

          ASSERT( true );
       }},
#endif // DISABLE_TO_AVOID_WARNING

      {"bit_writer.write_bit() writes a bit to the buffer",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          writer.write_bit( true );
          writer.write_bit( true );
          writer.write_bit( true );
          writer.write_bit( false );
          writer.flush();
          ASSERT( c[0] == 0xe0 );
       }},

      {"bit_writer.write_bits() writes a group of bits to the buffer",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          writer.write_bits<test_type>( 0xfaf, 12 );
          writer.flush();
          ASSERT( ( c[0] == 0xfa ) && ( c[1] == 0xf0 ) );
       }},

      {"bit_writer flushes partial bytes to the buffer",
       [] {
          container c;
          {
             auto writer = make_bit_writer( c );
             writer.write_bit( true );
             writer.write_bit( true );
          }
          ASSERT( c[0] == 0xc0 );
       }},
   }};

toy_test::suite unary_suite{
   "test for unary coder",
   {
      {"throws an exception for x=0",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          THROWS( integer_codes::unary::encode<test_type>( 0, writer ),
                  std::invalid_argument );
       }},
      {"encodes a one-value", [] { test_unary_coder( 1 ); }},
      {"encodes a small integer", [] { test_unary_coder( 2 ); }},
      {"encodes another small integer", [] { test_unary_coder( 5 ); }},
   }};

toy_test::suite truncated_binary_suite{
   "Test for truncated binary coder",
   {
      {"encode() throws an exception for n = 0",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          THROWS(
             integer_codes::truncated_binary::encode<test_type>( 1, 0, writer ),
             std::invalid_argument );
       }},

      {"decode() throws an exception for n = 0",
       [] {
          container c( 100 );
          auto      reader = make_bit_reader( c );
          THROWS(
             integer_codes::truncated_binary::decode<test_type>( 0, reader ),
             std::invalid_argument );
       }},

      {"encodes a value of 3 using n = 6",
       [] { test_truncated_binary_coder( 3, 6 ); }},

      {"encodes a value of 3 using n = 4",
       [] { test_truncated_binary_coder( 3, 4 ); }},

      {"encodes a value of 3 using n = 8",
       [] { test_truncated_binary_coder( 3, 8 ); }},

      {"encodes a value of 7 using n = 8",
       [] { test_truncated_binary_coder( 7, 8 ); }},

   }};

toy_test::suite elias_gamma_suite{
   "Test for elias gamma coder",
   {
      {"throws an exception for x=0",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          THROWS( integer_codes::elias_gamma::encode<test_type>( 0, writer ),
                  std::invalid_argument );
       }},
      {"encodes a value of 2", [] { test_elias_gamma_coder( 2 ); }},
      {"encodes a value of 3", [] { test_elias_gamma_coder( 3 ); }},
   }};

toy_test::suite elias_delta_suite{
   "Test for elias delta coder",
   {
      {"throws an exception for x=0",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          THROWS( integer_codes::elias_delta::encode<test_type>( 0, writer ),
                  std::invalid_argument );
       }},

      {"encodes a value of 1", [] { test_elias_delta_coder( 1 ); }},
      {"encodes a value of 3", [] { test_elias_delta_coder( 3 ); }},
   }};

toy_test::suite golomb_suite{
   "Test for golomb coder",
   {
      {"encode throws an exception for x=0",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          THROWS( integer_codes::golomb::encode<test_type>( 0, 5, writer ),
                  std::invalid_argument );
       }},

      {"encode throws an exception for b=0",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          THROWS( integer_codes::golomb::encode<test_type>( 5, 0, writer ),
                  std::invalid_argument );
       }},

      {"decode throws an exception for b=0",
       [] {
          container c( 100 );
          auto      reader = make_bit_reader( c );
          THROWS( integer_codes::golomb::decode<test_type>( 0, reader ),
                  std::invalid_argument );
       }},

      {"encodes and decodes small integers",
       [] {
          for ( test_type i = 1; i < 256; i++ ) {
             for ( test_type b = 1; b < 256; b++ ) {
                test_golomb( i, b );
             }
          }
       }},
   }};

toy_test::suite rice_suite{
   "Test for rice coder",
   {
      {"throws an exception for x=0",
       [] {
          container c( 100 );
          auto      writer = make_bit_writer( c );
          THROWS( integer_codes::rice::encode<test_type>( 0, 4, writer ),
                  std::invalid_argument );
       }},

      {"encodes and decodes small integers",
       [] {
          for ( test_type i = 1; i < 256; i++ ) {
             for ( test_type k = 0; k < 16; k++ ) {
                test_rice( i, k );
             }
          }
       }},
   }};

toy_test::suite zigzag_suite{"Test for zigzag coder",
                             {
                                {"encodes 0", [] { test_zigzag( 0 ); }},
                                {"encodes 1", [] { test_zigzag( 1 ); }},
                                {"encodes -1", [] { test_zigzag( -1 ); }},

                             }};

toy_test::suite offset_zigzag_suite{
   "Test for offset zigzag coder",
   {
      {"encodes 0 with offset 12", [] { test_offset_zigzag( 0, 12 ); }},
      {"encodes 1 with offset 12", [] { test_offset_zigzag( 1, 12 ); }},
      {"encodes -1 with offset 12", [] { test_offset_zigzag( -1, 12 ); }},

   }};

toy_test::suite varint_suite{"Test for varint coder",
                             {
                                {"encodes 0", [] { test_varint( 0 ); }},
                                {"encodes 1", [] { test_varint( 1 ); }},
                                {"encodes 128", [] { test_varint( 128 ); }},
                                {"encodes 275", [] { test_varint( 275 ); }},
                                {"encodes 1,948", [] { test_varint( 1948 ); }},
                                {"encodes 65538", [] { test_varint( 65538 ); }},
                             }};

int main() {
   toy_test::run_suites( {reader_suite, writer_suite, unary_suite,
                          truncated_binary_suite, elias_gamma_suite,
                          elias_delta_suite, golomb_suite, rice_suite,
                          zigzag_suite, offset_zigzag_suite, varint_suite} );
}
