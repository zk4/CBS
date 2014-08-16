
#include "Tools.h"

#ifdef TALKINGDATA
#include "TDCCTalkingDataGA.h"
#include "TDCCAccount.h"
#endif
string Tools::GetDeviceId()
{
#ifdef TALKINGDATA
    return TDCCTalkingDataGA::getDeviceId();
#endif
    return "911112311";
}

#ifdef WIN32
void Tools::GetMacAddress(char* macAddress, const char* ifName)
{
    UUID uuid;
    UuidCreateSequential(&uuid);      // Ask OS to create UUID
    const char* mac = "44-01-2E-44-32-A0";
    memmove(macAddress, mac, strlen(mac));
}
#else
void  Tools::GetMacAddress(char* macAddress, const char* ifName)
{
    int  success;
    struct ifaddrs* addrs;
    struct ifaddrs* cursor;
    const struct sockaddr_dl* dlAddr;
    const unsigned char* base;
    int i;
    success = getifaddrs(&addrs) == 0;

    if (success) {
        cursor = addrs;

        while (cursor != 0) {
            if ((cursor->ifa_addr->sa_family == AF_LINK)
                    && (((const struct sockaddr_dl*) cursor->ifa_addr)->sdl_type == IFT_ETHER) && strcmp(ifName,  cursor->ifa_name) == 0) {
                dlAddr = (const struct sockaddr_dl*) cursor->ifa_addr;
                base = (const unsigned char*) &dlAddr->sdl_data[dlAddr->sdl_nlen];
                strcpy(macAddress, "");

                for (i = 0; i < dlAddr->sdl_alen; i++) {
                    if (i != 0) {
                        strcat(macAddress, ":");
                    }

                    char partialAddr[3];
                    sprintf(partialAddr, "%02X", base[i]);
                    strcat(macAddress, partialAddr);
                }
            }

            cursor = cursor->ifa_next;
        }

        freeifaddrs(addrs);
    }

//	return macAddress;
}

#endif

void Tools::DUMPMEMORY(const char* buffer, const int size)
{
    for (int i = 0; i < size; ++i) {
        printf("%2d,", buffer[i]);

        if (i % 8 == 0) {
            printf("\n");
        }
    }
}

bool Tools::CheckOdds(float bet_)
{
    float systemodds = RandomBetween(0, 100) / 100.0f;
    //cout << "system odds:" << systemodds << "  me :" << bet_ << endl;
    return systemodds < bet_;
}

unsigned int Tools::Random()
{
#if defined(WIN32)
    static  std::random_device rd;
    std::mt19937 mt(rd());
    return mt();
    /*srand(time(NULL));
    return  rand();*/
#else
    return (arc4random() % ((unsigned) RAND_MAX + 1));
#endif
}

int Tools::RandomBetween(int begin, int end)
{
    if (begin == end) {
        return begin;
    }

    assert(begin < end);
    return begin + (Tools::Random() % (end - begin));
}

void* Tools::Memcpy(void* dest_, const void* src_, int count_)
{
    char*   dest  = (char*) dest_;
    const   char*   src  = (const   char*) src_;
    int  count  = (count_  +   7)  /   8 ;

    switch (count_  &   7) {
    case   0 :
        do {
            * dest ++   =   * src ++ ;

        case   7 :
            * dest ++   =   * src ++ ;

        case   6 :
            * dest ++   =   * src ++ ;

        case   5 :
            * dest ++   =   * src ++ ;

        case   4 :
            * dest ++   =   * src ++ ;

        case   3 :
            * dest ++   =   * src ++ ;

        case   2 :
            * dest ++   =   * src ++ ;

        case   1 :
            * dest ++   =   * src ++ ;
        } while (-- count  >   0);
    }

    return  dest_;
}

int Tools::Sec2MicroSec(int sec_)
{
    assert(sec_ >= 0);
    return sec_ * 1000;
}

int Tools::MicroSec2Sec(int micro_sec_)
{
    // NXASSERT(micro_sec_ >= 0)
    return micro_sec_ / 1000;
}

int Tools::OffsetEnum2Index(int val_)
{
    // NXASSERT ( ( val_ & Bit01Cycle ( sizeof ( int ) * 8 / 2 ) ) > 0 )
    if (((val_ - 1) & val_) != 0) {
        assert(0);
    }

    int index = 0;

    while (val_ >>= 1) {
        index++;
    }

    return index;
}

int Tools::Count1(int* number_, int bit_offset_, int bit_length)
{
    int count = 0;
    int mask = 1 << (sizeof(int) * 8 - 1);

    while (bit_length > 0) {
        if ((* (number_ + bit_offset_)) & mask) {
            count++;
        }

        bit_length--;
    }

    return count;
}

std::string Tools::Ip_Int2String(char ip_[4], int port_)
{
    struct IP {
        unsigned char a;
        unsigned char b;
        unsigned char c;
        unsigned char d;
    };
    //        char a=ip_[0];
    //
    //          NTOHL(*(int*)ip_);
    char str_trunk_ip_port[64] = {0};
    sprintf(str_trunk_ip_port, "%d.%d.%d.%d:%d", (unsigned char) ip_[0], (unsigned char) ip_[1], (unsigned char) ip_[2], (unsigned char) ip_[3], port_);
    return string(str_trunk_ip_port);
}

void Tools::FindAndReplace(std::string& source, const char* find, const char* replace)
{
    //ASSERT(find != NULL);
    //ASSERT(replace != NULL);
    size_t findLen = strlen(find);
    size_t replaceLen = strlen(replace);
    size_t pos = 0;

    //search for the next occurrence of find within source
    while ((pos = source.find(find, pos)) != std::string::npos) {
        //replace the found string with the replacement
        source.replace(pos, findLen, replace);
        //the next line keeps you from searching your replace string,
        //so your could replace "hello" with "hello world"
        //and not have it blow chunks.
        pos += replaceLen;
    }
}
bool Tools::FindSensitiveWorlds(const string str)
{
    const char* sensitiveWords[] = {"尖阁列岛", "钓鱼岛"};
    size_t pos = 0;

    for (auto s : sensitiveWords) {
        if ((pos = str.find(s, 0)) != std::string::npos) {
            return true;
        }
    }

    return  false;
}

std::string Tools::FormatTime(int seconds)
{
    string s;
    static char time_buffer[32];
    sprintf(time_buffer, "%02d:%02d:%02d", seconds / (3600), seconds % (3600) / 60, seconds % 60);
    s = time_buffer;
    return s;
}
const char* Tools::FormatTimeYMD(int seconds)
{
    time_t t = seconds;
    static char buffer[64];
    struct tm * timeinfo = localtime(&t);
    strftime(buffer, 64, "%Y-%m-%d", timeinfo);
    return buffer;

}

float Tools::RamdomClamp()
{
    return RandomBetween(0, 100) / 100.f;
}

int Tools::Index2OffsetEnum(int index)
{
    return (1 << index);
}

int Tools::Bit01Cycle(int rounds_)
{
    assert(rounds_ > 0 && rounds_ <= sizeof(int)* 8 / 2);
    int value = 1;

    while (rounds_) {
        (value <<= 2) |= 1;
        rounds_--;
    }

    return value;
}

int Tools::RemoveLast1(int number_)
{
    return number_ &= (number_ - 1);
}

int Tools::stoi(string s)  // convert string to int
{
    int num = -1;
    stringstream ss(s);
    ss >> num;
    return num;
}

std::string Tools::ftos(float i,int precision )  // convert int to string
{
    stringstream s;
    s << setiosflags(ios::fixed) << setprecision(precision) << i;
    return s.str();
}

std::string Tools::itos(int i)  // convert int to string
{
    stringstream s;
    s << i;
    return s.str();
}
#ifdef WIN32

std::string Tools::WideByte2UTF8(const wstring& text)
{
    int asciisize = ::WideCharToMultiByte(CP_UTF8, 0, text.c_str(), text.size(), NULL, 0, NULL, NULL);

    if (asciisize == ERROR_NO_UNICODE_TRANSLATION ||
            asciisize == 0) {
        return string();
    }

    char* resultstring = new char[asciisize];
    int convresult = ::WideCharToMultiByte(CP_UTF8, 0, text.c_str(), text.size(), resultstring, asciisize, NULL, NULL);

    if (convresult != asciisize) {
        return string();
    }

    std::string buffer(resultstring, convresult);
    delete[] resultstring;
    return buffer;
}
#endif // WIN32

#ifdef WIN32

void Tools::cprintf(const char* str, WORD color, ...)
{
    HANDLE handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
    WORD colorOld;      //用来保留原来的文字的颜色
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(handle, &csbi);
    colorOld = csbi.wAttributes;
    SetConsoleTextAttribute(handle, color);
    cout << str;
    SetConsoleTextAttribute(handle, colorOld);
}

#endif // WIN32



