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

#include "toy_test.hpp"
#include "toy_compression.hpp"
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
#include "binary_io_test.hpp"
#include "elias_delta_test.hpp"
#include "elias_gamma_test.hpp"
#include "golomb_rice_test.hpp"
#include "truncated_binary_test.hpp"
#include "unary_test.hpp"
#include "varint_test.hpp"
#include "zigzag_test.hpp"

int main() {
   toy_test::run_suites(
      {reader_suite, writer_suite, unary_suite, truncated_binary_suite,
       centered_truncated_binary_suite, binary_in_range_suite,
       elias_gamma_suite, elias_delta_suite, golomb_suite, rice_suite,
       zigzag_suite, offset_zigzag_suite, varint_suite} );
}
