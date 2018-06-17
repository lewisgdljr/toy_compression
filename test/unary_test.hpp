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
