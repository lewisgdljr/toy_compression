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
