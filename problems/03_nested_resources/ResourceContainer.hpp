#ifndef ResourceContainer_hpp
#define ResourceContainer_hpp

#include <map>
#include <mutex>
#include <cassert>

#include <Resource.hpp>

class ResourceContainer {
   typedef std::map <int, std::shared_ptr <Resource>> Resources;
   
public:
   typedef Resources::iterator resource_iterator;

   ResourceContainer () {;}
   ResourceContainer (const ResourceContainer&) = delete;
   virtual ~ResourceContainer () {;}
   
   std::mutex& getMutex () noexcept { return m_mutex; }
   
   void initialize (std::shared_ptr <Resource> resource) noexcept {
      std::unique_lock <std::mutex> guard (m_mutex);
      insert (guard, resource);      
   }

   virtual void insert (std::unique_lock <std::mutex>& guard, std::shared_ptr <Resource> resource) noexcept {
      assert (guard.mutex () == &m_mutex);
      m_resources.insert (std::make_pair (resource->getId (), resource));
   }

   virtual void erase (std::unique_lock <std::mutex>& guard, std::shared_ptr <Resource> resource) noexcept {
      assert (guard.mutex () == &m_mutex);
      
      resource_iterator ii = m_resources.find (resource->getId ());
      
      if (ii != resource_end ())
         m_resources.erase (ii);
   }

   resource_iterator erase (std::unique_lock <std::mutex>& guard, resource_iterator ii) noexcept {
      assert (guard.mutex () == &m_mutex);

      return m_resources.erase (ii);
   }

   resource_iterator resource_begin () noexcept { return m_resources.begin (); }
   resource_iterator resource_end () noexcept { return m_resources.end (); }
   static std::shared_ptr <Resource>& resource (resource_iterator ii) noexcept { return std::ref (ii->second); }
   static const Resource& resource_ref (resource_iterator ii) noexcept { return std::ref (*(ii->second.get ())); }
   uint32_t resource_size () noexcept { return m_resources.size (); }

protected: 
   std::mutex m_mutex;
      
private:
   Resources m_resources;
};

#endif

