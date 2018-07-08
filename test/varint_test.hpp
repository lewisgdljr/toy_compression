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
toy_test::suite varint_suite{"Test for varint coder",
                             {
                                {"encodes 0", [] { test_varint( 0 ); }},
                                {"encodes 1", [] { test_varint( 1 ); }},
                                {"encodes 128", [] { test_varint( 128 ); }},
                                {"encodes 275", [] { test_varint( 275 ); }},
                                {"encodes 1,948", [] { test_varint( 1948 ); }},
                                {"encodes 65538", [] { test_varint( 65538 ); }},
                             }};
