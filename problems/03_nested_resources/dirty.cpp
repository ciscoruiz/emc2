#include <thread>
#include <vector>

#include <iostream>

#include <Resource.hpp>
#include <ResourceBroker.hpp>
#include <ResourceRecover.hpp>

void brokerMethod (ResourceBroker& broker, ResourceRecover& recover)
{
   for (int ii = 0; ii < 1000; ++ ii) {
      std::unique_lock <std::mutex> r1 (broker.getMutex ());

      try {
         std::shared_ptr <Resource> resource = broker.select (r1);
         
         try {
            resource->use (ii);
         }
         catch (std::runtime_error& ex) {
            broker.erase (r1, resource);            
            std::unique_lock <std::mutex> r2 (recover.getMutex ());      
            recover.error (r2, resource);
         }
      }
      catch (std::runtime_error& ex) {
    	  std::this_thread::yield();
      }               
   }  
}

void recoverMethod (ResourceBroker& broker, ResourceRecover& recover)
{
   while (true) {
      std::unique_lock <std::mutex> r2 (recover.getMutex ());      
      
      while (recover.resource_size () == 0)
         recover.wait (r2);
      
      ResourceRecover::resource_iterator ii = recover.resource_begin ();
      
      while (ii != recover.resource_end ()) {
         auto resource = ResourceRecover::resource (ii);
         
         if (resource->recover ()) {
            std::unique_lock <std::mutex> r1 (broker.getMutex ());
            broker.insert (r1, resource);
            ii = recover.erase (r2, ii);            
            continue;
         }
         ++ ii;
      }      
   }
}

int main ()
{
   ResourceBroker broker;
   ResourceRecover recover;
   
   std::vector <std::thread> threads;
   
   for (int ii = 0; ii < 10; ++ ii) {
      std::shared_ptr <Resource> resource (new Resource (ii, 80));
      broker.initialize(resource);
   }   
   
   for (int ii = 0; ii < 10; ++ ii) {
      threads.push_back (std::thread (brokerMethod, std::ref (broker), std::ref (recover)));
   }
   
   std::thread thrRecover (recoverMethod, std::ref (broker), std::ref (recover));
   thrRecover.detach ();
   
   for (std::thread& thr : threads)
      thr.join ();
   
   return 0;
}

