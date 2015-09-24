#include <thread>
#include <mutex>

#include <iostream>

class MyClass {
public:
   MyClass () { m_fooCounter = m_gooCounter = 0; }
  
   uint32_t getFoo () const noexcept { return m_fooCounter; }
   uint32_t getGoo () const noexcept { return m_gooCounter; }

   void operator() () noexcept {
      
      for (int ii = 0; ii < 10; ++ ii)
          foo ();
   }

   int32_t  foo () {
      std::unique_lock <std::mutex> guard (m_mutex);

      if (m_fooCounter == 5) {
         m_fooCounter = 0;
         return goo ();
      }

      return ++ m_fooCounter;
   }

   int32_t goo () {
      std::unique_lock <std::mutex> guard (m_mutex);
      return ++ m_gooCounter;
   }

private:
   std::mutex m_mutex;
   uint32_t m_fooCounter;
   uint32_t m_gooCounter;
};

int main ()
{
   MyClass myClass;

   // Creates and run the thread tr.    
   std::thread tr (std::ref (myClass));
   
   // Waits for thread to terminate (if it would be able ...)
   tr.join ();

   std::cout << "Foo=" << myClass.getFoo () << " Goo=" << myClass.getGoo () << std::endl;

   return 0;
}

