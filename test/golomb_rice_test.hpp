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
