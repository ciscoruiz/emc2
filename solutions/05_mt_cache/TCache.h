#ifndef __emc2_cache_TCache_h
#define __emc2_cache_TCache_h

#include <memory>
#include <map>
#include <stdexcept>

#include <TEntry.h>

namespace emc2 {

namespace cache {

template <class _Key, class _Value> class TCache {
   typedef typename std::shared_ptr <TEntry <_Key,_Value>> EntryPtr;
   typedef typename std::map <_Key, EntryPtr> Entries;
   typedef typename Entries::value_type entry_type;
   
public:
   typedef typename Entries::const_iterator const_entry_iterator;

   TCache () : m_hintCounter (0), m_faultCounter (0), m_errorCounter (0) {;}
   TCache (const TCache&) = delete;
   
   const_entry_iterator entry_begin () const noexcept { return m_entries.begin (); }
   const_entry_iterator entry_end () const noexcept { return m_entries.end (); }
   size_t size () const noexcept { return m_entries.size (); }
   
   static const _Key& key (const_entry_iterator ii) noexcept { return std::ref (ii->first); }
   static const _Value& value (const_entry_iterator ii) throw (std::runtime_error) { return std::ref (ii->second->getValue ()); }

   uint32_t getHintCounter () const noexcept { return m_hintCounter; }
   uint32_t getFaultCounter () const noexcept { return m_faultCounter; }
   uint32_t getErrorCounter () const noexcept { return m_errorCounter; }
   
   const _Value& read (const _Key& key) 
      throw (std::runtime_error)
   {
      EntryPtr entry (find (key));            
            
      try {
         return entry->read (*this, key);
      }
      catch (std::runtime_error&) {
         m_errorCounter ++;
         throw;
      }
   }

protected:
   virtual _Value upload (const _Key& key) const throw (std::runtime_error) = 0;

private:
   std::mutex m_mutex;
   Entries m_entries;
   uint32_t m_hintCounter;
   uint32_t m_faultCounter;
   uint32_t m_errorCounter;
   
   EntryPtr find (const _Key& key)
   {
      std::unique_lock <std::mutex> guard (m_mutex);
   
      auto ii = m_entries.find (key);
   
      if (ii != m_entries.end ()) {
         m_hintCounter ++;
         return ii->second;
      }
   
      EntryPtr entry (new TEntry <_Key, _Value> ());
   
      m_entries.insert (entry_type (key, entry));
      
      m_faultCounter ++;
   
      return entry;
   }
   
   friend class TEntry <_Key,_Value>;
};

}
}

#endif


