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
