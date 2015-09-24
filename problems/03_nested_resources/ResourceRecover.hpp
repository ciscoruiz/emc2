#ifndef ResourceRecover_hpp
#define ResourceRecover_hpp

#include <condition_variable>

#include <ResourceContainer.hpp>

class ResourceRecover  : public ResourceContainer {
public:
   ResourceRecover () : m_stop (false) {;}
   ResourceRecover (const ResourceRecover&) = delete;

   void requestStop () noexcept { 
      std::unique_lock <std::mutex> r2 (m_mutex);      
      m_stop = true;       
   }
   
   bool stop (std::unique_lock <std::mutex>& guard) const noexcept { 
      assert (guard.mutex () == &m_mutex);
      return m_stop;      
   }
   
   void error (std::unique_lock <std::mutex>& guard, std::shared_ptr <Resource> resource) noexcept {
      assert (guard.mutex () == &m_mutex);

      insert (guard, resource);
      m_condition.notify_one ();
   }

   void wait (std::unique_lock <std::mutex>& guard) noexcept {
      assert (guard.mutex () == &m_mutex);

      auto now = std::chrono::system_clock::now();
      m_condition.wait_until (guard, now + std::chrono::milliseconds(500));
   }   

private:
   bool m_stop;
   std::condition_variable m_condition;
};

#endif


