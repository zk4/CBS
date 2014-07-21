#pragma  once
#include <cassert>
 
template <typename T> class Singleton
{
        static T* s_Singleton;

    public:
        Singleton ( void )
        {
            assert ( !s_Singleton );
   
            long long offset = ( long long ) ( T* ) 1 - ( long long ) ( Singleton <T>* ) ( T* ) 1;
            s_Singleton = ( T* ) ( ( long long ) this + offset );
          
        }
      
        ~Singleton ( void )
		{
			assert(s_Singleton != NULL);  s_Singleton = 0;
		}

        static T* GetSingleton ( void )

		{
            if ( !s_Singleton )
            {
                return new T;
            }
            else
            {
                return ( s_Singleton );
            }
        }

        static bool IsNull ( void )
        {
            return ( s_Singleton );
        }
};

template <typename T> T* Singleton <T>::s_Singleton = 0;