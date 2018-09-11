// The Art of C++ / Utility
// Copyright (c) 2016-2018 Daniel Frey

#ifndef TAO_UTILITY_GETENV_HPP
#define TAO_UTILITY_GETENV_HPP

#include <string>

#include <postgres_export.h>

namespace tao
{
   namespace utility
   {
      POSTGRES_EXPORT std::string getenv( const std::string& name );
      POSTGRES_EXPORT std::string getenv( const std::string& name, const std::string& default_value );

   }  // namespace utility

}  // namespace tao

#endif
