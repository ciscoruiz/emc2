
#include <vector>
#include <thread>
#include <algorithm>

#include <time.h>
#include <sys/time.h>

#include <iostream>

#include <future>

typedef std::vector<int> InputValues;

double averageMethod (const InputValues& values)
{
   long result = 0;

   for (int value : values) {
      result += value;
   }

   return result / values.size ();
}

int maxMethod (const InputValues& values)
{
   auto ii =  max_element (values.begin (), values.end ());
   return *ii;
}

int minMethod (const InputValues& values)
{
   auto ii =  min_element (values.begin (), values.end ());
   return *ii;
}

long millisecond () noexcept
{
   struct timeval now;
   gettimeofday (&now, NULL);
   long result = now.tv_sec * 1000;
   return result + now.tv_usec / 1000;
}

void usual (const InputValues& values)
{
   long init = millisecond ();

   double average = averageMethod (values);
   int max = maxMethod (values);
   int min = minMethod (values);

   double result = max - min / average;

   long end = millisecond ();

   std::cout << __func__ << " Result=" << result << " Time=" << end - init << " ms" << std::endl;
}


void averagePromise (std::promise <double>& prm, const InputValues& values)
{
   prm.set_value (averageMethod (values));
}

void maxPromise (std::promise <int>& promise, const InputValues& values)
{
   promise.set_value (maxMethod (values));
}

void minPromise (std::promise <int>& promise, const InputValues& values)
{
   promise.set_value (minMethod (values));
}

void promiseAndFuture (const std::vector<int>& values)
{
   long init = millisecond ();

   std::promise <double> prmAvg;
   std::future <double> futAvg = prmAvg.get_future ();  
   std::thread (averagePromise, std::ref (prmAvg), std::ref (values)).detach ();
     
   std::promise <int> prmMax;
   std::future <int> futMax = prmMax.get_future ();  
   std::thread (maxPromise, std::ref (prmMax), std::ref (values)).detach ();
   
   std::promise <int> prmMin;
   std::future <int> futMin = prmMin.get_future ();  
   std::thread (minPromise, std::ref (prmMin), std::ref (values)).detach ();
   
   double average = futAvg.get ();
   int max = futMax.get ();
   int min = futMin.get ();

   double result = max - min / average;

   long end = millisecond ();

   std::cout << __func__ << " Result=" << result << " Time=" << end - init << " ms" << std::endl;    
}

void usingSync (const std::vector<int>& values)
{
   long init = millisecond ();
   
   std::future <double> futAvg = std::async (std::launch::async, averageMethod, std::ref (values));
   std::future <int> futMax = std::async (std::launch::async, maxMethod, std::ref (values));
   std::future <int> futMin = std::async (std::launch::async, minMethod, std::ref (values));
   
   double average = futAvg.get ();
   int max = futMax.get ();
   int min = futMin.get ();

   double result = max - min / average;

   long end = millisecond ();

   std::cout << __func__ << " Result=" << result << " Time=" << end - init << " ms" << std::endl;    
}

int main ()
{
   InputValues values;

   for (int ii = 0; ii < 10000000; ++ ii) 
      values.push_back (rand ());

   usual (values);
   promiseAndFuture (values);
   usingSync (values);
}

