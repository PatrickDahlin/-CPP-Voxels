#ifndef _MACROS_HPP_
#define _MACROS_HPP_
#include <iostream>
#include "Errors.hpp"

#define cout(x) std::cout << x;
#define coutln(x) std::cout << x << std::endl;
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
 

#endif