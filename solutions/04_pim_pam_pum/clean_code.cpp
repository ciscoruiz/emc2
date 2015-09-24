#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <condition_variable>

struct Status {
   enum _v { PimPam, PimPamPum, Chimpum, End }; 
};

std::mutex st_mutex;
std::condition_variable st_condStatus;
Status::_v st_status = Status::PimPam;

void smartWriter (const Status::_v myStatus, const Status::_v nextStatus)
{
   static const char* texts [] = { "Pim-pam,", "Pim-pam-pum,", "Chimpum\n" };

   std::unique_lock <std::mutex> guard (st_mutex);

   while (st_status != myStatus)
      st_condStatus.wait (guard);

   std::cout << texts [myStatus] << std::flush;

   st_status = nextStatus;
   st_condStatus.notify_all ();
}


int main ()
{
   std::vector <std::thread> threads;

   threads.push_back (std::thread (smartWriter, Status::PimPam, Status::PimPamPum));
   threads.push_back (std::thread (smartWriter, Status::PimPamPum, Status::Chimpum));
   threads.push_back (std::thread (smartWriter, Status::Chimpum, Status::End));

   for (std::thread& thr : threads) {
      thr.join ();
   }

   return 0;
}

