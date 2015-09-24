#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <condition_variable>

void pim_pam ()
{
   std::cout << "Pim-pam," << std::flush;
}

void pim_pam_pum () {
   std::cout << "Pim-pam-pum," << std::flush;
}

void chimpum () {
   std::cout << "Chimpum" << std::endl;
}


int main ()
{

   std::thread t1 (pim_pam);
   std::thread t2 (pim_pam_pum);

   t1.join ();
   t2.join ();

   std::thread t3 (chimpum);
   t3.join ();

   return 0;
}

