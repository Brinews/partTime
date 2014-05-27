/*
 * rpnmain.cpp
 *
 * Author: Oscar Benjamin
 * Date: 21 Mar 2014
 *
 * This file defines the main functions for a program that evaluates its
 * command line arguments as an RPN stream.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <stack>

#include "bigint.h"
#include "parsenum.h"

// Numeric constants to distinguish different modes
const unsigned int FLOAT = 0;
const unsigned int BIGINT = 1;

template<typename T>
bool print_double(const std::vector<std::string>& tokens);

int main(int argc, char *argv[])
{
  // First deal with the no arguments case.
  // Then we don't need to worry about it later.
  if(argc == 1) {
    std::cerr << "usage: " << argv[0] << " [--float] TOKENS..." << std::endl;
    return 1;
  }

  // Check for options in the first argument
  unsigned int offset = 1; // Number of args to skip
  unsigned int mode;
  if(argv[1] == std::string("--float")) {
    mode = FLOAT;
    offset++;
  }
  else {
    mode = BIGINT;
  }

  // Convert the argument array to a vector of strings
  // Also throw away the first element of argv (it's useless)
  // and the --float argument if present.
  std::vector<std::string> args(argv+offset, argv+argc);

  // Dispatch to the appropriate templated function
  bool success;
  if(mode == FLOAT) {
    success = print_double<float>(args);
  }
  else if(mode == BIGINT) {
    success = print_double<BigInt>(args);
  }
  else {
    std::cerr << "What mode am I in !!!!!!" << std::endl;
    return 1;
  }
  // If success is true return 0, for false return 1.
  return !success;
}

//
// Here we have code that can be reused for BigInt and for float.
// The templates allow us to avoid rewriting this function twice which is a
// big advantage if the function is large. This also makes it easier to
// maintain the code since having multiple versions of the same code makes it
// hard to keep all of hte versions in sync.
//
template<typename T>
bool print_double(const std::vector<std::string>& tokens)
{
  // We'll use a stack to push the numbers on and then pop
  // them off again. Pretty pointless...
  std::stack<T> numstack;
  for(unsigned int i=0; i<tokens.size(); ++i) {
    // Try to parse the token as representing an object of type T
    // This will dispatch to either parse_num<BigInt> or parse_num<float>
    // depending on whether this function was called as print_double<BigInt> or
    // print_double<float>
    T number;
    if(!parse_num<T>(tokens[i], number)) {
      std::cerr << "Invalid token: \"" << tokens[i] << "\"" << std::endl;
      return false;
    }
    // Token was successfuly parsed and result stored in number
    // We'll push it onto the stack.
    numstack.push(number);
  }

  // Okay so now let's pop the numbers off and print them doubling as we go
  while(!numstack.empty()) {
    T num = numstack.top();
    numstack.pop();
    // Print double the number!
    std::cout << num << " * 2 = " << 2 * num << std::endl;
  }

  return true; // success
}

