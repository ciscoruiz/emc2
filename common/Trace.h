#ifndef __emc2_common_Trace_h
#define __emc2_common_Trace_h

#include <iostream>
#include <thread>

namespace emc2 {

namespace common {

class Trace {
public:
   Trace (const char* method) : m_method (method) { std::cout << "Enter " << method << " " << pthread_self () << std::endl; }
   ~Trace () { std::cout << "Exit " << m_method << " " << pthread_self () << std::endl << std::endl; }

private:
   const char* m_method;
};

}

}

#endif

