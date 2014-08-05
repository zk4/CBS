#pragma once
//
//
//  Created by ZK on 13-1-4.
//
//

#include <cassert>
#include <cmath>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include<iomanip>


#ifdef WIN32
#include <type_traits>
#include <random>
#else
#include <type_traits>
#endif
extern "C"
{
#ifdef WIN32
#include <Rpc.h>
#pragma comment( lib, "Rpcrt4.lib" )
    //#else
    //#include <uuid/uuid.h>
#endif
}

using namespace std;

// Check windows 检查是否在64位系统下
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif
#ifndef WIN32
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <net/if_dl.h>
#include <ifaddrs.h>
#define IFT_ETHER 0x6
#else

#endif
#include <sstream>


using namespace  std;
#define  __T(s) Tools::itos(s).c_str()
#define  __STR(s) Tools::itos(s)

#define  __FTOS(s,precision) Tools::ftos(s,precision).c_str()
#define  __ITOS(s) Tools::itos(s).c_str()
#define  __STOI(s) Tools::stoi(s)
#define  Z(x) Tools::ZeroMem(x)
class Tools
{
public:

#ifdef WIN32
    static void cprintf ( const char* str, WORD color, ... );
    static  std::string WideByte2UTF8 (const wstring& text);
#endif

    static int    stoi ( string s );   // convert string to  int;
    static string ftos(float i, int precision = 2);   // convert int to string;
    static string itos ( int i ) ;  // convert int to string;
    static string FormatTime (int seconds);
    static const char* FormatTimeYMD(int seconds);
    static bool		FindSensitiveWorlds (const string  str);
    static int		Count1 (int* number_, int bit_offset_, int bit_length);
    static int		RemoveLast1 (int number_);
    static int		Bit01Cycle (int rounds_);
    static int		OffsetEnum2Index (int val_);
    static   int	Index2OffsetEnum (int index);
    static int		Sec2MicroSec (int sec_);
    static int		MicroSec2Sec (int micro_sec_);
    //memory
    static   void*  Memcpy (void*   dest_, const   void*   src_, int  count_);
    static int		RandomBetween (int begin, int end);
    static float	RamdomClamp();
    static unsigned int  Random();
    static bool		CheckOdds (float bet_);
    static void		DUMPMEMORY (const char* buffer, const int size);
    static void		FindAndReplace (std::string& source, const char* find, const char* replace);
    static  string  Ip_Int2String (char ip_[4], int port_);

    static string	GetDeviceId();
    static void		GetMacAddress (char* macAddress, const char* ifName);
    template<typename  T>
    static string ntos ( T i )   // convert number to string
    {
        stringstream s;
        s << i;
        return s.str();
    }

    template<typename T>
    static void ZeroMem ( T& t )
    {
        memset ( &t, 0, sizeof ( T ) );
    }

    template<typename T>
    static void DeletePVector ( T& t )
    {
        for ( size_t i = 0 ; i < t.size() ; ++i ) {
            if ( t[i] ) {
                SAFE_DELETE ( t[i] );
            }
        }
        t.clear();
    }

    template< typename T, size_t N >
    static std::vector<T> MakeVector ( const T ( &data_ ) [N] )
    {
        return std::vector<T> ( data_, data_ + N );
    };

    template<class T>
    static inline T ForceBeyondLevel ( T val_, T level_ )
    {
        return val_ < level_ ? level_ : val_;
    }

    template<class T>
    static inline T Clamp (T& val_, T left_, T right_)
    {
        val_ = val_ < right_ ? val_ : right_;
        val_ = val_ > left_ ? val_ : left_;
        return val_;
    }

    template<typename T>
    static bool IfBetween ( T value , T close_begin_, T close_end_ )
    {
        return ( value >= close_begin_ ) && ( value <= close_end_ ) ;
    }

};





