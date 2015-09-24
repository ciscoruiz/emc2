#ifndef __emc2_cache_TEntry_h
#define __emc2_cache_TEntry_h

#include <memory>
#include <map>
#include <stdexcept>
#include <mutex>
#include <condition_variable>

#include <Status.h>

namespace emc2 {

namespace cache {

template <class _Key, class _Value> class TCache;

template <class _Key, class _Value> class TEntry {
public:
   TEntry () : m_status (Status::Empty) {;}
   
   const _Value& getValue () const throw (std::runtime_error) { 
      if (m_status != Status::Available)
         throw std::runtime_error ("Value is not available");
      
      return m_value;       
   }

private:
   Status::_v m_status;
   std::mutex m_mutex;
   std::condition_variable m_available;
   _Value m_value;
  
   const _Value& read (const TCache <_Key,_Value>& cache, const _Key& key) 
      throw (std::runtime_error) 
   {
      std::unique_lock <std::mutex> guard (m_mutex);

      if (invalidValue ()) {
         try {
            m_status = Status::Uploading;
            m_value = cache.upload (key);
            m_status = Status::Available;
            m_available.notify_all ();
         }
         catch (std::runtime_error&) {
            m_status = Status::Error;
            m_available.notify_all ();
            throw;
         }
      }

      while (isUploading ())
         m_available.wait (guard);
      
      if (m_status == Status::Error)
         throw std::runtime_error ("Value was not upload due to some error");

      return std::ref (m_value);
   } 
   
   bool invalidValue () const noexcept { return m_status == Status::Empty || m_status == Status::Error; }
   bool isUploading () const noexcept { return m_status == Status::Uploading; }
   
   friend class TCache <_Key,_Value>;
};

}
}

#endif
