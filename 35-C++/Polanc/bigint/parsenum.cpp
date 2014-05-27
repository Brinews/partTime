/*
 * parsenum.cpp
 *
 * Author: Oscar Benjamin
 * Date: 21 Mar 2014
 *
 * This file implements the parse_num<T> template for the two types BigInt and
 * float.
 */

#include <sstream>

#include "bigint.h"
#include "parsenum.h"

//
// For parse_num we use template specialisation to define two different
// versions. This one is parse_num<BigInt> and the other is parse_num<float>.
// So even though we use the same print_double template for both types we can
// still call different subfunctions for parse_num.
//
template<> bool parse_num(std::string str, BigInt& number)
{
  // Check for possible minus sign
  unsigned int skip = 0;
  if(!str.empty() && str[0] == '-')
    skip = 1;

  // Confirm that the string is not empty and not just a minus sign
  if(str.size() - skip == 0)
    return false;

  // Check that all remaining characters are digits
  for(unsigned int i=skip; i<str.size(); ++i)
    if(str[i] < '0' || str[i] > '9')
      return false;

  // Create a new bigint and copy it into number
  number = BigInt(str);
  return true;
}

// parse_num<float>
template<> bool parse_num(std::string str, float& number)
{
  std::istringstream is(str);
  is >> number;
  return (is && is.eof());
}
