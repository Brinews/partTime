/*
 * parsenum.h
 *
 * Author: Oscar Benjamin
 * Date: 21 Mar 2014
 *
 * Header for parsenum.cpp. Exports a templated function parse_num<T> for
 * parsing a string as a number and detecting errors.
 */

#ifndef PARSENUM_H
#define PARSENUM_H

#include <string>

template<typename T>
bool parse_num(std::string str, T& number);

#endif // PARSENUM_H
