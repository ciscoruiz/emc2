#ifndef Resource_hpp
#define Resource_hpp

#include <mutex>

class Resource {
public:
   Resource (const int id, const int availabilityFactor) : 
      m_id (id), m_availabilityFactor (availabilityFactor) 
   {
      m_sum = m_useCounter = m_recoveryCounter = m_failCounter = 0;
   }

   Resource (const Resource&) = delete;

   int getId () const noexcept { return m_id; }
      
   void use (const uint32_t value) throw (std::runtime_error) {
      std::unique_lock <std::mutex> guard (m_mutex);

      if ((rand () % 100) >= m_availabilityFactor) {
         ++ m_failCounter;
         throw std::runtime_error ("Out of service");
      }
      
      m_sum += value;
      m_useCounter ++;      
   }
   
   bool recover () noexcept {
      std::unique_lock <std::mutex> guard (m_mutex);
      
      bool result = ((rand () % 100) < m_availabilityFactor);
      
      if (result)
         m_recoveryCounter ++;
      
      return result;
   }

private:
   const int m_id;
   const int m_availabilityFactor;
   std::mutex m_mutex;
   uint32_t m_sum;
   uint32_t m_useCounter;
   uint32_t m_failCounter;
   uint32_t m_recoveryCounter;
   
   friend std::ostream& operator<< (std::ostream& os, const Resource& resource);
};

std::ostream& operator<< (std::ostream& os, const Resource& resource) {
   os << "Id=" << resource.m_id << " Sum=" << resource.m_sum;
   os << " UseCnt=" << resource.m_useCounter << " RecoveryCnt=" << resource.m_recoveryCounter;
   os << " FailtCnt=" << resource.m_failCounter;
   return os;
}


#endif

