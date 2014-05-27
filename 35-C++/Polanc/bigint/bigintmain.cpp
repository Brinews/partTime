/*
 * rationalmain.cpp
 *
 * Author: Oscar Benjamin
 * Date: 20 Mar 2014
 *
 * This file defines the main function for demonstrating the capabilities of
 * the BigInt class defined in bigint.cpp/.h.
 */

#include <iostream>
#include "bigint.h"

int main(int argc, char *argv[])
{
  // Create a BigInt - initial value zero
  BigInt b1;
  std::cout << "b1 = " << b1 << std::endl;

  // Create a BigInt from an int
  BigInt b2(1234);
  std::cout << "b2 = " << b2 << std::endl;

  // Create a temporary BigInt inline
  std::cout << "BigInt(-12) = " << BigInt(-12) << std::endl;

  // Add two BigInts
  BigInt b3 = BigInt(1234567) + BigInt(123);
  std::cout << "b3 = " << b3 << std::endl;

  // Create a BigInt from a string
  // This way there's no maximum limit on the size of the BigInt
  BigInt b4 = BigInt("10000000000000000000000");
  std::cout << "b4 = " << b4 << std::endl;

  // Multiply two BigInts - no overflow!
  std::cout << "b4*b4 = " << b4 * b4 << std::endl;

  std::cout << "b4 == 0? " << (BigInt() == 0) << std::endl;

  // Divide two BigInts
  std::cout << "1024/3 -> " << BigInt(1024) / BigInt(3) << std::endl;
  std::cout << "-1024/3 -> " << BigInt(-1024) / BigInt(3) << std::endl;

  // Create a BigInt from a command line string
  if(argc > 1)
    std::cout << argv[1] << " * 3 = " << BigInt(argv[1]) * BigInt(3) << std::endl;
  else
    std::cerr << "give a command line argument!" << std::endl;

  // In BigInt v0.0 these below didn't work
  if(BigInt(0) == BigInt(0))
    std::cout << "0 equals 0" << std::endl;

  if(BigInt(12) < 13)
    std::cout << "12 < 13" << std::endl;

  if(!(BigInt(12) > 13))
    std::cout << "12 is not greater than 13" << std::endl;

  if(BigInt(100) == 100)
    std::cout << "100 equals 100" << std::endl;

  std::cout << "123 modulo 100 equals " << (BigInt(123) % 100) << std::endl;

  // In BigInt v0.0 and v0.1 this didn't work:
  std::cout << "2 / 2 == " << BigInt(2) / BigInt(2) << std::endl;

  // These were fixed in v0.3
  std::cout << "-3 / 4 == " << BigInt(-3) / BigInt(4) << std::endl;
  std::cout << "-4 / 4 == " << BigInt(-4) / BigInt(4) << std::endl;
  std::cout << "-5 / 4 == " << BigInt(-5) / BigInt(4) << std::endl;

  return 0;
}
