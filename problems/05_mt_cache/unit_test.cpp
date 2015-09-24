#include <boost/test/unit_test.hpp>

#include <vector>
#include <thread>

#include <TCache.h>


typedef std::pair <int,int> MyValue;

class MyCache : public emc2::cache::TCache <int, MyValue> {
public:
   MyCache () {;}
 
private:
   MyValue upload (const int& key) const 
      throw (std::runtime_error) 
   {
      if (key == 333)
         throw std::runtime_error ("Error uploading data");
      
      return MyValue (key, key * 2);
   }
};

void readKeyFrom0to100 (MyCache& cache)
{
   for (int ii = 0; ii < 100; ++ ii) {
      cache.read (ii);
   }
}

BOOST_AUTO_TEST_CASE (reading_same_keys)
{
   std::vector <std::thread> threads;
   int MaxThread=5;
   
   MyCache cache;
   
   for (int ii = 0; ii < MaxThread; ++ ii) {
      threads.push_back (std::thread (readKeyFrom0to100, std::ref (cache)));
   }
   
   for (std::thread& thr : threads)
      thr.join ();
   
   BOOST_REQUIRE_EQUAL (cache.size (), 100);
   BOOST_REQUIRE_EQUAL (cache.getErrorCounter (), 0);
   BOOST_REQUIRE_EQUAL (cache.getFaultCounter (), 100);
   BOOST_REQUIRE_EQUAL (cache.getHintCounter (), MaxThread * 100 - 100);   
   
   for (auto ii = cache.entry_begin (), maxii = cache.entry_end (); ii != maxii; ++ ii) {
      int key = MyCache::key (ii);
      
      const MyValue& myValue (MyCache::value (ii));
      
      BOOST_REQUIRE_EQUAL (key, myValue.first);
      BOOST_REQUIRE_EQUAL (key * 2, myValue.second);      
   }
}

void randomAccess (MyCache& cache)
{
   for (int ii = 0; ii < 100; ++ ii) {
      cache.read (rand ());
   }
}

BOOST_AUTO_TEST_CASE (readingRandom)
{
   std::vector <std::thread> threads;
   int MaxThread=5;
   
   MyCache cache;
   
   for (int ii = 0; ii < MaxThread; ++ ii) {
      threads.push_back (std::thread (randomAccess, std::ref (cache)));
   }
   
   for (std::thread& thr : threads)
      thr.join ();
   
   BOOST_REQUIRE_EQUAL (cache.getFaultCounter (), cache.size ());
   
   for (auto ii = cache.entry_begin (), maxii = cache.entry_end (); ii != maxii; ++ ii) {
      int key = MyCache::key (ii);
      
      const MyValue& myValue (MyCache::value (ii));
      
      BOOST_REQUIRE_EQUAL (key, myValue.first);
      BOOST_REQUIRE_EQUAL (key * 2, myValue.second);      
   }
}

BOOST_AUTO_TEST_CASE (errorPrecondition)
{
   MyCache cache;
   int key = 333;
   
   BOOST_REQUIRE_THROW (cache.read (key), std::runtime_error);   
   BOOST_REQUIRE_EQUAL (cache.getFaultCounter (), 1);
   BOOST_REQUIRE_EQUAL (cache.getHintCounter (), 0);
   BOOST_REQUIRE_EQUAL (cache.getErrorCounter (), 1);
   BOOST_REQUIRE_EQUAL (cache.size (), 1);

   BOOST_REQUIRE_THROW (cache.read (key), std::runtime_error);   
   BOOST_REQUIRE_EQUAL (cache.getFaultCounter (), 1);
   BOOST_REQUIRE_EQUAL (cache.getHintCounter (), 1);
   BOOST_REQUIRE_EQUAL (cache.getErrorCounter (), 2);
   BOOST_REQUIRE_EQUAL (cache.size (), 1);
}

void readingErrors (MyCache& cache)
{
   for (int ii = 300; ii < 400; ++ ii) {
      try {
         cache.read (ii);
      }
      catch (std::runtime_error&) {
      }
   }
}

BOOST_AUTO_TEST_CASE (errorUploading)
{
   std::vector <std::thread> threads;
   int MaxThread=10;
   
   MyCache cache;

   for (int ii = 0; ii < MaxThread; ++ ii) {
      threads.push_back (std::thread (readingErrors, std::ref (cache)));
   }
   
   for (std::thread& thr : threads)
      thr.join ();
   
   BOOST_REQUIRE_EQUAL (cache.getFaultCounter (), cache.size ());
   BOOST_REQUIRE_EQUAL (cache.getErrorCounter (), MaxThread);
   
   int accessError = 0;
   
   for (auto ii = cache.entry_begin (), maxii = cache.entry_end (); ii != maxii; ++ ii) {
      int key = MyCache::key (ii);
      
      try {
         const MyValue& myValue (MyCache::value (ii));      
         BOOST_REQUIRE_EQUAL (key, myValue.first);
         BOOST_REQUIRE_EQUAL (key * 2, myValue.second);               
      }
      catch (std::runtime_error) {
         BOOST_REQUIRE_EQUAL (key, 333);
         accessError ++;
      }
   }
   
   BOOST_REQUIRE_EQUAL (accessError, 1);
}



