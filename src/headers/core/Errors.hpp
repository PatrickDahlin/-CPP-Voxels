#ifndef _ERRORS_HPP_
#define _ERRORS_HPP_
#include <iostream>

#define error(x) std::cout << "-----------------------------" << std::endl << "ERROR " << __FILE__ << ":" << __LINE__ << " > " << x << std::endl << "-----------------------------" << std::endl;

#else

#define error(x)

#endif