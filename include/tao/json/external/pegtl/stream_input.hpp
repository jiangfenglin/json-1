// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_JSON_PEGTL_INCLUDE_STREAM_INPUT_HPP
#define TAOCPP_JSON_PEGTL_INCLUDE_STREAM_INPUT_HPP

#include <cstdio>
#include <istream>

#include "buffer_input.hpp"
#include "config.hpp"
#include "eol.hpp"

#include "internal/cstream_reader.hpp"
#include "internal/istream_reader.hpp"

namespace tao
{
   namespace TAOCPP_JSON_PEGTL_NAMESPACE
   {
      template< typename Eol = lf_crlf_eol >
      struct cstream_input
         : buffer_input< internal::cstream_reader, Eol >
      {
         template< typename T >
         cstream_input( std::FILE* in_stream, const std::size_t in_maximum, T&& in_source )
            : buffer_input< internal::cstream_reader, Eol >( std::forward< T >( in_source ), in_maximum, in_stream )
         {
         }
      };

      template< typename Eol = lf_crlf_eol >
      struct istream_input
         : buffer_input< internal::istream_reader, Eol >
      {
         template< typename T >
         istream_input( std::istream& in_stream, const std::size_t in_maximum, T&& in_source )
            : buffer_input< internal::istream_reader, Eol >( std::forward< T >( in_source ), in_maximum, in_stream )
         {
         }
      };

   }  // namespace TAOCPP_JSON_PEGTL_NAMESPACE

}  // namespace tao

#endif
