#ifndef SINGLETON_H_INCLUDED
#define SINGLETON_H_INCLUDED

namespace Util
{

/**
 * Singleton class
 **/ 
template<class A>
class Singleton
{
   private:
   
   protected:
      Singleton() = default;

   public:
      Singleton(const Singleton&)            = delete;
      Singleton(Singleton&&)                 = delete;
      Singleton& operator=(const Singleton&) = delete;
      Singleton& operator=(Singleton&&)      = delete;
      
      static A& instance()
      {
         static A a; 
         return a;
      }
};

} /* namespace Util */

#endif /* SINGLETON_H_INCLUDED */
