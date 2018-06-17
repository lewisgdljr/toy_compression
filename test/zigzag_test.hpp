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
