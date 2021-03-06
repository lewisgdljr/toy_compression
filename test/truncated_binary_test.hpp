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

toy_test::suite truncated_binary_suite{
   "Test for truncated binary coder",
   {
      {"encode() throws an exception for n = 0",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          THROWS(
             integer_codes::truncated_binary::encode<test_type>( 0, 0, writer ),
             std::invalid_argument );
       }},
      {"encode() throws an exception for x > n",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          THROWS(
             integer_codes::truncated_binary::encode<test_type>( 4, 3, writer ),
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

void test_centered_truncated_binary_coder( test_type value, test_type n ) {
   container c;
   {
      auto writer = make_bit_writer( c );
      integer_codes::centered_truncated_binary::encode( value, n, writer );
   }
   auto reader  = make_bit_reader( c );
   auto decoded = integer_codes::centered_truncated_binary::decode<test_type>(
      n, reader );
   ASSERT( decoded == value );
}

toy_test::suite centered_truncated_binary_suite{
   "Test for centered truncated binary coder",
   {
      {"encode() throws an exception for n = 0",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          THROWS( integer_codes::centered_truncated_binary::encode<test_type>(
                     1, 0, writer ),
                  std::invalid_argument );
       }},

      {"decode() throws an exception for n = 0",
       [] {
          container c( 100 );
          auto      reader = make_bit_reader( c );
          THROWS( integer_codes::centered_truncated_binary::decode<test_type>(
                     0, reader ),
                  std::invalid_argument );
       }},

      {"encodes a value of 3 using n = 6",
       [] { test_centered_truncated_binary_coder( 3, 6 ); }},

      {"encodes a value of 3 using n = 4",
       [] { test_centered_truncated_binary_coder( 3, 4 ); }},

      {"encodes a value of 3 using n = 8",
       [] { test_centered_truncated_binary_coder( 3, 8 ); }},

      {"encodes a value of 7 using n = 8",
       [] { test_centered_truncated_binary_coder( 7, 8 ); }},

   }};

void test_binary_in_range( test_type value, test_type low, test_type high ) {
   container c;
   {
      auto writer = make_bit_writer( c );
      integer_codes::binary_in_range::encode( value, low, high, writer );
   }
   auto reader = make_bit_reader( c );
   auto decoded
      = integer_codes::binary_in_range::decode<test_type>( low, high, reader );
   ASSERT( decoded == value );
}

toy_test::suite binary_in_range_suite{
   "Test for binary in range coder",
   {
      {"encode() throws an exception for low = 0",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          THROWS( integer_codes::binary_in_range::encode<test_type>( 1, 0, 10,
                                                                     writer ),
                  std::invalid_argument );
       }},

      {"encode() throws an exception for low > high",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          THROWS( integer_codes::binary_in_range::encode<test_type>( 1, 4, 1,
                                                                     writer ),
                  std::invalid_argument );
       }},

      {"encode() throws an exception for x < low",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          THROWS( integer_codes::binary_in_range::encode<test_type>( 3, 4, 5,
                                                                     writer ),
                  std::invalid_argument );
       }},
      {"encode() throws an exception for x > high",
       [] {
          container c;
          auto      writer = make_bit_writer( c );
          THROWS( integer_codes::binary_in_range::encode<test_type>( 6, 4, 5,
                                                                     writer ),
                  std::invalid_argument );
       }},

      {"decode() throws an exception for low = 0",
       [] {
          container c( 100 );
          auto      reader = make_bit_reader( c );
          THROWS(
             integer_codes::binary_in_range::decode<test_type>( 0, 10, reader ),
             std::invalid_argument );
       }},

      {"decode() throws an exception for low > high",
       [] {
          container c( 100 );
          auto      reader = make_bit_reader( c );
          THROWS(
             integer_codes::binary_in_range::decode<test_type>( 4, 1, reader ),
             std::invalid_argument );
       }},

      {"encodes a value of 3 using range = [2,6]",
       [] { test_binary_in_range( 3, 2, 6 ); }},

      {"encodes a value of 3 using range = [1,4]",
       [] { test_binary_in_range( 3, 1, 4 ); }},

      {"encodes a value of 3 using n = [1, 8]",
       [] { test_binary_in_range( 3, 1, 8 ); }},

      {"encodes a value of 7 using n = [1, 8]",
       [] { test_binary_in_range( 7, 1, 8 ); }},

   }};
