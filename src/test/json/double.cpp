// Copyright (c) 2015-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hpp"

#include <tao/json/from_string.hpp>
#include <tao/json/value.hpp>

namespace tao
{
   namespace json
   {
      union uint64_double
      {
         explicit uint64_double( const uint64_t u )
            : u( u )
         {
         }

         uint64_t u;
         double d;
      };

      inline uint64_t d2u( const double d )
      {
         return uint64_double( d ).u;
      }

      void test_double( const std::string& input, const double value )
      {
         const double d = json::from_string( input ).get_double();
         TEST_ASSERT( d2u( d ) == d2u( value ) );
      }

      void unit_test()
      {
         test_double( "0.0", 0.0 );
         test_double( "-0.0", -0.0 );
         test_double( "1.0", 1.0 );
         test_double( "-1.0", -1.0 );
         test_double( "1.5", 1.5 );
         test_double( "-1.5", -1.5 );
         test_double( "3.1416", 3.1416 );
         test_double( "0E10", 0.0 );
         test_double( "0E+10", 0.0 );
         test_double( "0E-10", 0.0 );
         test_double( "0.00E10", 0.0 );
         test_double( "0.00E+10", 0.0 );
         test_double( "0.00E-10", 0.0 );
         test_double( "0.00E0", 0.0 );
         test_double( "0.00E+0", 0.0 );
         test_double( "0.00E-0", 0.0 );
         test_double( "0.00E000", 0.0 );
         test_double( "0.00E+000", 0.0 );
         test_double( "0.00E-000", 0.0 );
         test_double( "1.000E0000", 1.0 );
         test_double( "1.000E+0000", 1.0 );
         test_double( "1.000E-0000", 1.0 );
         test_double( "1.000E00001", 10.0 );
         test_double( "1.000E+00001", 10.0 );
         test_double( "1.000E-00001", 0.1 );
         test_double( "1E10", 1E10 );
         test_double( "1e10", 1e10 );
         test_double( "1E+10", 1E+10 );
         test_double( "1E-10", 1E-10 );
         test_double( "-1E10", -1E10 );
         test_double( "-1e10", -1e10 );
         test_double( "-1E+10", -1E+10 );
         test_double( "-1E-10", -1E-10 );
         test_double( "1.234E+10", 1.234E+10 );
         test_double( "1.234E-10", 1.234E-10 );
         test_double( "1.79769e+308", 1.79769e+308 );
         test_double( "2.22507e-308", 2.22507e-308 );
         test_double( "-1.79769e+308", -1.79769e+308 );
         test_double( "-2.22507e-308", -2.22507e-308 );
         test_double( "4.9406564584124654e-324", 4.9406564584124654e-324 );  // Min denormal
         test_double( "2.2250738585072009e-308", 2.2250738585072009e-308 );  // Max subnormal double
         test_double( "2.2250738585072014e-308", 2.2250738585072014e-308 );  // Min normal positive double
         test_double( "1.7976931348623157e+308", 1.7976931348623157e+308 );  // Max double
         test_double( "1e-10000", 0.0 );                                     // Must underflow
         test_double( "18446744073709551616", 18446744073709551616.0 );      // 2^64 (max of uint64_t + 1, force to use double)
         test_double( "-9223372036854775809", -9223372036854775809.0 );      // -2^63 - 1(min of int64_t + 1, force to use double)
         test_double( "0.9868011474609375", 0.9868011474609375 );            // https://github.com/miloyip/rapidjson/issues/120
         test_double( "123e34", 123e34 );                                    // Fast Path Cases In Disguise
         test_double( "45913141877270640000.0", 45913141877270640000.0 );
         test_double( "2.2250738585072011e-308", 2.2250738585072011e-308 );  // http://www.exploringbinary.com/php-hangs-on-numeric-value-2-2250738585072011e-308/
         //test_double( "1e-00011111111111", 0.0 );
         //test_double( "-1e-00011111111111", -0.0 );
         test_double( "1e-214748363", 0.0 );
         test_double( "1e-214748364", 0.0 );
         //test_double( "1e-21474836311", 0.0 );
         test_double( "0.017976931348623157e+310", 1.7976931348623157e+308 );  // Max double in another form

         // Since
         // abs((2^-1022 - 2^-1074) - 2.2250738585072012e-308) = 3.109754131239141401123495768877590405345064751974375599... ¡Á 10^-324
         // abs((2^-1022) - 2.2250738585072012e-308) = 1.830902327173324040642192159804623318305533274168872044... ¡Á 10 ^ -324
         // So 2.2250738585072012e-308 should round to 2^-1022 = 2.2250738585072014e-308
         test_double( "2.2250738585072012e-308", 2.2250738585072014e-308 );  // http://www.exploringbinary.com/java-hangs-when-converting-2-2250738585072012e-308/

         // More closer to normal/subnormal boundary
         // boundary = 2^-1022 - 2^-1075 = 2.225073858507201136057409796709131975934819546351645648... 10^-308
         test_double( "2.22507385850720113605740979670913197593481954635164564e-308", 2.2250738585072009e-308 );
         test_double( "2.22507385850720113605740979670913197593481954635164565e-308", 2.2250738585072014e-308 );

         // 1.0 is in (1.0 - 2^-54, 1.0 + 2^-53)
         // 1.0 - 2^-54 = 0.999999999999999944488848768742172978818416595458984375
         test_double( "0.999999999999999944488848768742172978818416595458984375", 1.0 );                  // Round to even
         test_double( "0.999999999999999944488848768742172978818416595458984374", 0.99999999999999989 );  // Previous double
         test_double( "0.999999999999999944488848768742172978818416595458984376", 1.0 );                  // Next double
         // 1.0 + 2^-53 = 1.00000000000000011102230246251565404236316680908203125
         test_double( "1.00000000000000011102230246251565404236316680908203125", 1.0 );                  // Round to even
         test_double( "1.00000000000000011102230246251565404236316680908203124", 1.0 );                  // Previous double
         test_double( "1.00000000000000011102230246251565404236316680908203126", 1.00000000000000022 );  // Next double

         // Numbers from https://github.com/floitsch/double-conversion/blob/master/test/cctest/test-strtod.cc

         test_double( "72057594037927928.0", 72057594037927928.0 );
         test_double( "72057594037927936.0", 72057594037927936.0 );
         test_double( "72057594037927932.0", 72057594037927936.0 );
         test_double( "7205759403792793199999e-5", 72057594037927928.0 );
         test_double( "7205759403792793200001e-5", 72057594037927936.0 );

         test_double( "9223372036854774784.0", 9223372036854774784.0 );
         test_double( "9223372036854775808.0", 9223372036854775808.0 );
         test_double( "9223372036854775296.0", 9223372036854775808.0 );
         test_double( "922337203685477529599999e-5", 9223372036854774784.0 );
         test_double( "922337203685477529600001e-5", 9223372036854775808.0 );

         test_double( "10141204801825834086073718800384", 10141204801825834086073718800384.0 );
         test_double( "10141204801825835211973625643008", 10141204801825835211973625643008.0 );
         test_double( "10141204801825834649023672221696", 10141204801825835211973625643008.0 );
         test_double( "1014120480182583464902367222169599999e-5", 10141204801825834086073718800384.0 );
         test_double( "1014120480182583464902367222169600001e-5", 10141204801825835211973625643008.0 );

         test_double( "5708990770823838890407843763683279797179383808", 5708990770823838890407843763683279797179383808.0 );
         test_double( "5708990770823839524233143877797980545530986496", 5708990770823839524233143877797980545530986496.0 );
         test_double( "5708990770823839207320493820740630171355185152", 5708990770823839524233143877797980545530986496.0 );
         test_double( "5708990770823839207320493820740630171355185151999e-3", 5708990770823838890407843763683279797179383808.0 );
         test_double( "5708990770823839207320493820740630171355185152001e-3", 5708990770823839524233143877797980545530986496.0 );

         test_double( "2.22507385850720113605740979670913197593481954635164564802342610972482222202107694551652952390813508"
                      "7914149158913039621106870086438694594645527657207407820621743379988141063267329253552286881372149012"
                      "9811224514518898490572223072852551331557550159143974763979834118019993239625482890171070818506906306"
                      "6665599493827577257201576306269066333264756530000924588831643303777979186961204949739037782970490505"
                      "1080609940730262937128958950003583799967207254304360284078895771796150945516748243471030702609144621"
                      "5722898802581825451803257070188608721131280795122334262883686223215037756666225039825343359745688844"
                      "2390026549819838548794829220689472168983109969836584681402285424333066033985088644580400103493397042"
                      "7567186443383770486037861622771738545623065874679014086723327636718751234567890123456789012345678901"
                      "e-308",
                      2.2250738585072014e-308 );

         test_double( "30e001", 300.0 );

         // TODO: Disabled, triggers infinity
         // test_double( std::string( 2000, '1' ), std::numeric_limits< double >::max() );

         TEST_THROWS( from_string( "1.0e1234567890" ) );
         TEST_THROWS( from_string( std::string( 1048577, '1' ) ) );
         {
            const auto a = from_string( "42.0" ).as< double >();
            TEST_ASSERT( sizeof( a ) == sizeof( double ) );
            TEST_ASSERT( a == 42.0 );
         }
         {
            const auto a = from_string( "42.3" ).as< short >();
            TEST_ASSERT( sizeof( a ) == sizeof( short ) );
            TEST_ASSERT( a == 42 );
         }
         {
            const auto a = from_string( "42" ).as< double >();
            TEST_ASSERT( sizeof( a ) == sizeof( double ) );
            TEST_ASSERT( a == 42.0 );
         }
         TEST_THROWS( value( "foo" ).as< double >() );
         TEST_THROWS( value( "bar" ).as< int >() );
      }

   }  // json

}  // tao

#include "main.hpp"
