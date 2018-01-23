#ifndef _ERRORS_HPP_
#define _ERRORS_HPP_
#include <iostream>

#define error(x) std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << " > " << x << std::endl;

#else

#define error(x)

#endif