// Copyright 1985-2012 Luo Guochun, http://imlgc.com
#ifndef X_JSON_H_
#define X_JSON_H_

#include <string>
#include <stdexcept>
#include <vector>

namespace json
{

#define E_SUCCESS               0
#define E_OUTOF_INDEX           100
#define E_NULL_PTR              110
#define E_DUPLICATE_KEY         120
#define E_BAD_TYPE_CAST         130
#define E_KEY_NOT_EXISTS        140
#define E_OPEN_FILE_ERROR       150
#define E_BRACE_MISSING         500
#define E_UKNOWN_SYNTAX         510
#define E_PARSE_JASON_VALUE     520
#define E_PARSE_JASON_OBJECT    530
#define E_PARSE_JASON_ARRAY     540
#define E_EMPTY_STRING          550
#define E_QUOT_MISSING          560
#define E_KEY_MISSING           570
#define E_SENMICOLON_MISSING    580
#define E_VALUE_MISSING         590
#define E_EXTRAL_COMMA          600
#define E_COLON_MISSING         610
#define E_UNKNOWN_VAULE         620
#define E_STRING_VALUE_ERROR    630
#define E_NOT_JSON_SYNTAX       640
#define E_BRACKET_MISSING       650
#define E_ENKNOW_NUMBER         660
#define E_EXTRAL_CHARACTERS     670

#define DEFAULT_ARR_CAPACITY    16
#define FORMAT_SPACE            4

enum
{
    StringType  = 10,
    BoolType    = 20,   // false true null
    IntegerType = 30,
    DoubleType  = 40,
    ObjectType  = 50,
    ArrayType   = 60,
    NullType    = 70
};

const char* GetErrMsg(int nErrCode);

class JsonException
    : public std::exception
{
public:
    JsonException(const char* szMsg)
    {
        if (szMsg)
        {
            _strMsg = szMsg;
        }
    }
    ~JsonException() throw()
    {

    }
    const char* what() const throw()
    {
        return _strMsg.c_str();
    }
private:
    std::string _strMsg;
};

class JsonValue;
class JsonArray
{
public:
    JsonArray(int nCap = DEFAULT_ARR_CAPACITY);
    JsonArray(const JsonArray& sRight);
    ~JsonArray();

    JsonArray& operator = (const JsonArray& sRight);
public:
    JsonValue& Get(int nIndex) const;
    
    JsonValue& operator[](int nIndex);
    const JsonValue& operator[](int nIndex) const;
    
    int Add(JsonValue& sVal);
    
    int GetSize() const;
    int GetCapacity() const;

private:
    void FreeRes();
    void Assign(const JsonArray& sRight);
        
private:
    std::vector<JsonValue*> _vArray;
};

class Json;
class JsonValue
{
    typedef union _JsonVariant
    {
		bool _boolVal;
        char* _strVal;    
        long long _llVal;
        double _fVal;
        Json* _pJsonVal;
        JsonArray* _pArrVal;
    }JsonVariant;

    friend class Json;
public:
    JsonValue();
    JsonValue(bool bVal);
    JsonValue(const char* szVal);
    JsonValue(int nVal);
    JsonValue(long long llVal);
    JsonValue(float fVal);
    JsonValue(double fVal);
    JsonValue(const Json& sVal);
    JsonValue(const JsonArray& sArr);
    JsonValue(const JsonValue& sRight);    
    
    ~JsonValue();

    JsonValue& operator = (const JsonValue& sRight);

    JsonValue& operator = (bool bVal);
    JsonValue& operator = (const char* szVal);
    JsonValue& operator = (int nVal);
    JsonValue& operator = (long long llVal);
    JsonValue& operator = (float fVal);
    JsonValue& operator = (double fVal);
    JsonValue& operator = (const Json& sVal);
    JsonValue& operator = (const JsonArray& sArr);

    JsonValue& operator [] (const char* szKey);
    const JsonValue& operator [] (const char* szKey) const;

public:
    operator const char*();
    operator char*();
    operator int();
    operator long long();
    operator float();
    operator double();
    operator Json();
    operator JsonArray();
	operator bool();
public:
    inline   bool IsEmpty() {return _nType == 0;}
private:
    void FreeRes();
    void Assign(const JsonValue& sRight);
    void Assign(bool bVal);
    void Assign(const char* szVal);
    void Assign(int nVal);
    void Assign(long long llVal);
    void Assign(float fVal);
    void Assign(double fVal);
    void Assign(const Json& sVal);
    void Assign(const JsonArray& sArr);
    void Assign(const JsonVariant& sRight);

private:
    JsonVariant _sVal;
    int _nType;
};

class JsonValueItem
{
    friend class Json;
public:
    JsonValueItem(const char* szKey = 0, JsonValue* pJV = 0);
    JsonValueItem(const JsonValueItem& sRight);
    ~JsonValueItem();
    JsonValueItem& operator = (const JsonValueItem& sRight);
    void FreeRes();
    void Assign(const JsonValueItem& sRight);
    
    int SetKey(const char* szKey);
    std::string GetKey() const;

private:
    //char* _szKey;
    std::string _strKey;
    JsonValue* _pVal;
    JsonValueItem* _pNextVal;
};

class Json
{
public:
    Json();
    Json(const Json& sRight);
    ~Json();
    
    Json& operator = (const Json& sRight);
public:
    static int Parse(Json*& pJson, const char* pBuf);
    static int Load(Json*& pJson, const char* pFilePath);
    int Save(const char* pFilePath);
    
    JsonValue* Set(const char* szKey, JsonValue& sVal);
    JsonValue* Get(const char* szKey) const;
        
    void Dump(std::string& strDump) const;
    void DumpFormat(std::string& strDump, int nSpace = FORMAT_SPACE) const;

    unsigned GetItemSize() const { return _nLen;}

public:
    JsonValue& operator [] (const char* szKey);
    const JsonValue& operator [] (const char* szKey) const;

private:    
    void DumpValueGroup(JsonValueItem* pGroup, std::string& strDump, int nSpace) const;
    void DumpArray(JsonArray* pArr, std::string& strDump, int nSpace) const;
    void DumpValue(JsonValue* pJV, std::string& strDump, int nSpace) const;
    
    void FreeRes();
    void Assign(const Json& sRight);

    static inline void SkipSpaces(const char*& szBuf);
    static inline bool EofBuffer(const char*& szBuf);
    static int ParseKey(const char*& szBuf, const char*& pStart, int& nLen);
    static int ParseJson(Json*& pJS, const char*& szBuf);
    static int ParseJsonValue(JsonValue*& pJV, const char*& szBuf);
    static int ParseJsonArray(JsonArray*& pJArr, const char*& szBuf);
private:
    JsonValueItem* _pGrpVal;
    unsigned int _nLen;
};
}


#endif // X_JSON_H_
