// The Art of C++ / PostgreSQL
// Copyright (c) 2016 Daniel Frey

#include "test_macros.hpp"

#include <tao/utility/getenv.hpp>
#include <tao/postgres/connection.hpp>
#include <tao/postgres/result_traits_optional.hpp>
#include <tao/postgres/result_traits_pair.hpp>
#include <tao/postgres/result_traits_tuple.hpp>

int main()
{
  const auto connection = tao::postgres::connection::create( tao::utility::getenv( "TEST_DATABASE", "dbname=template1" ) );

  TEST_ASSERT( connection->execute( "SELECT 42" )[ 0 ].as< int >() == 42 );
  TEST_ASSERT( connection->execute( "SELECT 1764" )[ 0 ].optional< int >() == 1764 );
  TEST_ASSERT( !connection->execute( "SELECT NULL" )[ 0 ].as< tao::optional< int > >() );

  TEST_ASSERT( connection->execute( "SELECT 42" )[ 0 ][ 0 ].get() == std::string( "42" ) );
  TEST_ASSERT( connection->execute( "SELECT 42" )[ 0 ][ 0 ].as< int >() == 42 );
  TEST_ASSERT( connection->execute( "SELECT 42" )[ 0 ][ 0 ].optional< int >() == 42 );

  TEST_ASSERT( connection->execute( "SELECT 1, 2" )[ 0 ].pair< int, int >() == std::pair< int, int >( 1, 2 ) );
  TEST_ASSERT( connection->execute( "SELECT 1, 2, 3, 4" )[ 0 ].tuple< int, int, int, int >() == std::tuple< int, int, int, int >( 1, 2, 3, 4 ) );

  TEST_THROWS( connection->execute( "SELECT 42" )[ 0 ].as< bool >() );

  const auto result = connection->execute( "SELECT 1 AS a, 2 AS B, 3 AS \"C\"");
  const auto& row = result[ 0 ];

  TEST_ASSERT( row.columns() == 3 );

  TEST_ASSERT( row.name( 0 ) == "a" );
  TEST_ASSERT( row.name( 1 ) == "b" );
  TEST_ASSERT( row.name( 2 ) == "C" );

  TEST_ASSERT( row[ 0 ].name() == "a" );
  TEST_ASSERT( row[ 1 ].name() == "b" );
  TEST_ASSERT( row[ 2 ].name() == "C" );

  TEST_ASSERT( row.index( "a" ) == 0 );
  TEST_ASSERT( row.index( "A" ) == 0 );
  TEST_ASSERT( row.index( "\"a\"" ) == 0 );
  TEST_THROWS( row.index( "\"A\"" ) );

  TEST_ASSERT( row.index( "b" ) == 1 );
  TEST_ASSERT( row.index( "B" ) == 1 );
  TEST_ASSERT( row.index( "\"b\"" ) == 1 );
  TEST_THROWS( row.index( "\"B\"" ) );

  TEST_THROWS( row.index( "c" ) );
  TEST_THROWS( row.index( "C" ) );
  TEST_THROWS( row.index( "\"c\"" ) );
  TEST_ASSERT( row.index( "\"C\"" ) == 2 );

  TEST_THROWS( row.get< std::string >( 3 ) );
  TEST_THROWS( row.get< tao::optional< std::string > >( 3 ) );
  TEST_THROWS( row.get< std::pair< std::string, std::string > >( 2 ) );

  TEST_ASSERT( connection->execute( "SELECT 1 AS a, 2 AS b, 3 AS a" )[ 0 ].slice( 1, 2 ).index( "a" ) == 1 );
  TEST_THROWS( connection->execute( "SELECT 1 AS a, 2 AS b, 3 AS a" )[ 0 ].slice( 2, 1 ).index( "b" ) );
}