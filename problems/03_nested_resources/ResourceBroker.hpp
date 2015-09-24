#ifndef ResourceBroker_hpp
#define ResourceBroker_hpp

#include <stdexcept>

#include <ResourceContainer.hpp>

class ResourceBroker : public ResourceContainer {
public:
   ResourceBroker () {;}
   ResourceBroker (const ResourceBroker&) = delete;

   std::shared_ptr <Resource> select (std::unique_lock <std::mutex>& guard)
      throw (std::runtime_error) 
   {
      assert (guard.mutex () == &m_mutex);

      if (m_position == resource_end ())
         throw std::runtime_error ("There is not any resource");

      std::shared_ptr <Resource>& result = resource (m_position);

      if (++ m_position == resource_end ())
         m_position = resource_begin ();

      return result;
   }
   
   void insert (std::unique_lock <std::mutex>& guard, std::shared_ptr <Resource> resource) noexcept {
      assert (guard.mutex () == &m_mutex);
      
      ResourceContainer::insert (guard, resource);
      m_position = resource_begin ();
   }   

   void erase (std::unique_lock <std::mutex>& guard, std::shared_ptr <Resource> resource) noexcept {
      assert (guard.mutex () == &m_mutex);

      ResourceContainer::erase (guard, resource);

      m_position = resource_begin ();
   }
   
private:   
   resource_iterator m_position;   
};

#endif

