/*
 * Toy Test - Toy Unit Testing
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
#ifndef TESTING_HPP_INCLUDED
#define TESTING_HPP_INCLUDED

#include <functional>
#include <initializer_list>
#include <iostream>
#include <vector>

namespace toy_test {
   struct test_case {
      const char*           name;
      std::function<void()> run;
      void                  operator()() const { run(); }
   };

   struct failure {
      const char* expr;
      int         line;
   };

   struct suite {
      const char*            name;
      std::vector<test_case> tests;

      bool run() const {
         bool ok{true};
         std::cout << "[SUITE] Running test suite: \"" << name << "\""
                   << std::endl
                   << std::endl;
         for ( auto&& test : tests ) {
            try {
               test();
               std::cout << "[OK.] \"" << test.name << "\" passed."
                         << std::endl;
            } catch ( failure& caught ) {
               ok = false;
               std::cout << "[FAIL!] \"" << test.name << "\" failed."
                         << std::endl;
               std::cout << "Failing condition: \"" << caught.expr
                         << "\" at line: " << caught.line << std::endl;
            }
         }
         if ( ok ) {
            std::cout << std::endl
                      << "[OK] All tests passed for suite: \"" << name << "\""
                      << std::endl;
         } else {
            std::cout << std::endl
                      << "[FAIL!] Test failures detected in suite: \"" << name
                      << "\"" << std::endl;
         }
         return ok;
      }
   };

   bool run_suite( suite const& suite ) { return suite.run(); }

   bool run_suites( std::initializer_list<suite const> const& suites ) {
      bool ok = true;
      for ( auto const& a : suites ) {
         ok &= run_suite( std::forward<suite const>( a ) );
      }

      if ( ok ) {
         std::cout << std::endl
                   << "[OK] All tests passed." << std::endl
                   << std::endl;
      } else {
         std::cout << std::endl
                   << "[FAIL!] Test failures detected." << std::endl
                   << " Check the output for details." << std::endl
                   << std::endl;
      }
      return ok;
   }

#define ASSERT( condition )                       \
   void( ( condition ) ? 0                        \
                       : throw toy_test::failure( \
                            {"ASSERT(" #condition ")", __LINE__} ) )

#define THROWS( expression, exception )                           \
   try {                                                          \
      ( expression );                                             \
      throw toy_test::failure(                                    \
         {"THROWS(" #expression ", " #exception ")", __LINE__} ); \
   } catch ( exception& ) {                                       \
   } catch ( ... ) {                                              \
      throw toy_test::failure(                                    \
         {"THROWS(" #expression ", " #exception ")", __LINE__} ); \
   }
} // namespace toy_test
#endif // TESTING_HPP_INCLUDED
