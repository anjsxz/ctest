//-------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#ifndef MEMBUFFER_H
#define MEMBUFFER_H



/* data type*/
typedef unsigned long      DWORD;
//typedef int BOOL;
typedef unsigned char      BYTE;
typedef unsigned short     WORD;
typedef float              FLOAT;

typedef char *LPSTR,*PSTR;
typedef const char  *LPCSTR,*PCSTR;

typedef wchar_t *LPWSTR,*PWSTR;
typedef const wchar_t *LPCWSTR,*PCWSTR;

class CMemBuffer
{
public:
    CMemBuffer();
    ~CMemBuffer();
    
public:
    typedef struct tagDATA
    {
        int    nDataLength;
        LPSTR  pData;    
    }DATA;
    
public:
    void                  ReleaseBuffer();
    bool                  Delete(int nIdx,int nLength);
    LPSTR                 GetBuffer(int nLength);
    int                   Find(char ch);
    
    inline  int             GetLength()         {return m_data.nDataLength;}
    inline  operator        LPCSTR() const      {return m_data.pData;}
    inline  char*           GetData()           {return m_data.pData;}
private:    
    DATA    m_data;       
};

#endif