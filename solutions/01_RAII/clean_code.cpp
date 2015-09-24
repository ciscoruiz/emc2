#include <thread>
#include <mutex>
#include <stdexcept>

#include <vector>

#include <common/Trace.h>

using namespace emc2;

class Account {
public:
   Account (const int32_t balance) : m_balance (balance) {
      std::cout << pthread_self () << " init=" << m_balance << std::endl;
   }

   int32_t getBalance () const noexcept { return m_balance; }

   void withdraw (int32_t value) throw (std::runtime_error) {
      common::Trace trace ("withdraw");

      std::unique_lock <std::mutex> guard (m_mutex);
      
      if (m_balance < value)
         throw std::runtime_error ("Out of balance");

      m_balance -= value;

      std::cout << pthread_self () << " Balance=" << m_balance << std::endl;
   }
   
private:
   std::mutex m_mutex;
   int32_t m_balance;
};

static const int32_t withdrawLoop = 5;

void withdraw  (Account& account, const int32_t value)
{
   try {
      for (int32_t ii = 0; ii < withdrawLoop; ++ ii) {
         account.withdraw (value);
      }
   }
   catch (std::runtime_error& ex) {
      std::cout << pthread_self () << " Exception=" << ex.what () << std::endl;
   }
}

int main ()
{
   common::Trace ("main");
   const int32_t withdrawValue = 3;
   const int32_t maxThread = 5;

   Account account (withdrawLoop * withdrawValue * maxThread - 5);
   
   std::vector<std::thread> threads;

   for (int ii = 0; ii < maxThread; ++ ii) {
      threads.push_back (std::thread (withdraw, std::ref (account), withdrawValue));
   }
   
   for (std::thread& thread : threads) {
      thread.join ();
   }
  
  std::cout << "Final balance=" << account.getBalance () << std::endl;

   return 0;
}


