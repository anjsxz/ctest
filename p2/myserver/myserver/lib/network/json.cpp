// Copyright 1985-2012 Luo Guochun, http://imlgc.com
#include "json.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#endif

namespace json
{
    
#define NullPtr                 0
    
    typedef struct _ErrMsg
    {
        int nErrCode;
        const char* szErrMsg;
    }ErrMsg;
    
    static ErrMsg _gErrMsg[] = {
        { E_SUCCESS, "No error" },
        { E_OUTOF_INDEX, "Out of index" },
        { E_NULL_PTR, "Pointer is null" },
        { E_DUPLICATE_KEY, "Duplicate key" },
        { E_BAD_TYPE_CAST, "Bad type cast" },
        { E_KEY_NOT_EXISTS, "Key does not exists"},
        { E_OPEN_FILE_ERROR, "File error"},
        { E_BRACE_MISSING, "Brace missing" },
        { E_UKNOWN_SYNTAX, "Unkown json syntax" },
        { E_PARSE_JASON_VALUE, "Parse json value error"},
        { E_PARSE_JASON_OBJECT, "Parse json object error"},
        { E_PARSE_JASON_ARRAY, "Parse json array error"},
        { E_EMPTY_STRING, "Empty json string"},
        { E_QUOT_MISSING, "Quot \" missing"},
        { E_KEY_MISSING, "Key missing"},
        { E_SENMICOLON_MISSING, "Semicolon missing"},
        { E_VALUE_MISSING, "Value missing"},
        { E_EXTRAL_COMMA, "Unexpected extra comma"},
        { E_COLON_MISSING, "Colon missing"},
        { E_UNKNOWN_VAULE, "Unkown value"},
        { E_STRING_VALUE_ERROR, "String Value error"},
        { E_NOT_JSON_SYNTAX, "Unexpected json syntax"},
        { E_BRACKET_MISSING, "Bracket missing"},
        { E_ENKNOW_NUMBER, "Unknown number"},
        { E_EXTRAL_CHARACTERS, "Unexpect extral characters"}
    };
    
    
    const char* GetErrMsg(int nErrCode)
    {
        const char* szMsg = NullPtr;
        int nLow = 0;
        int nHigh = sizeof(_gErrMsg)/sizeof(_gErrMsg[0]) - 1;
        while(nLow <= nHigh)
        {
            int nMid = (nLow + nHigh) / 2;
            ErrMsg& sErr = _gErrMsg[nMid];
            if(sErr.nErrCode == nErrCode)
            {
                szMsg = sErr.szErrMsg;
                break;
            }
            
            if(sErr.nErrCode < nErrCode)
            {
                nLow = nMid + 1;
            }
            else
            {
                nHigh = nMid - 1;
            }
        }
        
        return szMsg;
    }
    
    enum NumberType
    {
        Int,
        Double,
        Unkown
    };
    
    static NumberType detectNumberType(const char* szStrVal)
    {
        NumberType type = Unkown;
        if (szStrVal != NullPtr && *szStrVal != '\0')
        {
            const char* pStart = szStrVal;
            bool bHasDot = false;
            bool bCanHasDot = false;
            while(*pStart != 0)
            {
                if (*pStart < '0' || *pStart > '9')
                {
                    if (*pStart == '.')
                    {
                        if (bHasDot)
                        {
                            type = Unkown;
                            break;
                        }
                        else
                        {
                            if (bCanHasDot)
                            {
                                bHasDot = true;
                                pStart++;
                                type = Double;
                            }
                            else
                            {
                                type = Unkown;
                            }
                        }
                    }
                    else if(*pStart == '-')
                    {
                        pStart++;
                    }
                    else
                    {
                        //type = Unkown;
                        break;
                    }
                }
                else
                {
                    bCanHasDot = true;
                    pStart++;
                    if (!bHasDot)
                    {
                        type = Int;
                    }
                }
                
            }
        }
        if (type == Int)
        {
            /*if (*szStrVal == '0')
             {
             type = Unkown;
             }*/       
        }
        
        return type;
    }
    
    static long long toInt(const char* szStrVal, bool& bStat)
    {
        int nVal = 0;
        bStat = true;
        if (szStrVal == NullPtr)
        {
            bStat = false;
        }
        else
        {
            int nRet = -1;
            nRet = sscanf(szStrVal, "%d", &nVal);
            if (nRet == -1)
            {
                bStat = false;
            }
        }
        return nVal;
    }
    static double toDouble(const char* szStrVal, bool& bStat)
    {
        double dVal = 0.0;
        bStat = true;
        if (szStrVal == NullPtr)
        {
            bStat = false;
        }
        else
        {
            int nRet = -1;
            nRet = sscanf(szStrVal, "%lf", &dVal);
            if (nRet == -1)
            {
                bStat = false;
            }
        }
        return dVal;
    }
    
    ////////////////////////////////////////
    // JsonArray
    JsonArray::JsonArray(int nCap)
    {
        if(nCap > 0)
        {
            _vArray.reserve(nCap);
        }
    }
    JsonArray::JsonArray(const JsonArray& sRight)
    {
        Assign(sRight);
        
    }
    JsonArray::~JsonArray()
    {
        FreeRes();
    }
    void JsonArray::Assign(const JsonArray& sRight)
    {
        _vArray.reserve(sRight._vArray.size());
        for (std::vector<JsonValue*>::const_iterator iter = sRight._vArray.begin();
             iter != sRight._vArray.end(); ++iter)
        {
            JsonValue* pJV = *iter;
            _vArray.push_back(new JsonValue(*pJV));
        }
        
    }
    void JsonArray::FreeRes()
    {
        for(std::vector<JsonValue*>::iterator iter = _vArray.begin();
            iter != _vArray.end(); ++iter)
        {
            delete *iter;
        }
        _vArray.clear();
    }
    JsonArray& JsonArray::operator = (const JsonArray& sRight)
    {
        if (this != &sRight)
        {
            FreeRes();
            Assign(sRight);
        }
        return *this;
    }
    JsonValue& JsonArray::Get(int nIndex) const
    {
        if(nIndex < 0 || nIndex >= (int)(_vArray.size()))
        {
            throw JsonException(GetErrMsg(E_OUTOF_INDEX));
        }
        return *_vArray[nIndex];
    }
    JsonValue& JsonArray::operator[](int nIndex)
    {
        if(nIndex < 0 || nIndex >= (int)(_vArray.size()))
        {
            if (nIndex == (int)(_vArray.size()))
            {
                JsonValue* pJV = new JsonValue;
                _vArray.push_back(pJV);
                return *pJV;
            }
            else
            {
                throw JsonException(GetErrMsg(E_OUTOF_INDEX));
            }
            
        }
        return *_vArray[nIndex];
    }
    const JsonValue& JsonArray::operator[](int nIndex) const
    {
        if(nIndex < 0 || nIndex >= (int)(_vArray.size()))
        {
            throw JsonException(GetErrMsg(E_OUTOF_INDEX));
        }
        return *_vArray[nIndex];
    }
    int JsonArray::Add(JsonValue& sVal)
    {
        JsonValue* pJV = new JsonValue(sVal);
        _vArray.push_back(pJV);
        return E_SUCCESS;
    }
    int JsonArray::GetSize() const
    {
        return _vArray.size();
    }
    int JsonArray::GetCapacity() const
    {
        return _vArray.capacity();
    }
    
    
    ////////////////////////////////////////
    // JsonValue
    JsonValue::JsonValue()
    {
        _nType = 0;
        memset(&_sVal, 0, sizeof(&_sVal));
    }
    JsonValue::JsonValue(bool bVal)
    {
        Assign(bVal);
    }
    JsonValue::JsonValue(const char* szVal)
    {
        Assign(szVal);
    }
    JsonValue::JsonValue(int nVal)
    {
        Assign((long long)nVal);
    }
    JsonValue::JsonValue(long long llVal)
    {
        Assign(llVal);
    }
    JsonValue::JsonValue(float fVal)
    {
        Assign((double)fVal);
    }
    JsonValue::JsonValue(double fVal)
    {
        Assign(fVal);
    }
    JsonValue::JsonValue(const Json& sVal)
    {
        Assign(sVal);
    }
    JsonValue::JsonValue(const JsonArray& sArr)
    {
        Assign(sArr);
    }
    JsonValue::JsonValue(const JsonValue& sRight)
    {
        Assign(sRight);
    }
    JsonValue::~JsonValue()
    {
        FreeRes();
    }
    JsonValue& JsonValue::operator = (const JsonValue& sRight)
    {
        if (this != &sRight)
        {
            FreeRes();
            Assign(sRight);
        }
        return *this;
    }
    
    JsonValue& JsonValue::operator = (bool bVal)
    {
        FreeRes();
        Assign(bVal);
        return *this;
    }
    JsonValue& JsonValue::operator = (const char* szVal)
    {
        FreeRes();
        Assign(szVal);
        return *this;
    }
    JsonValue& JsonValue::operator = (int nVal)
    {
        FreeRes();
        Assign((long long)nVal);
        return *this;
    }
    JsonValue& JsonValue::operator = (long long llVal)
    {
        FreeRes();
        Assign(llVal);
        return *this;
    }
    JsonValue& JsonValue::operator = (float fVal)
    {
        FreeRes();
        Assign((double)fVal);
        return *this;
    }
    JsonValue& JsonValue::operator = (double fVal)
    {
        FreeRes();
        Assign(fVal);
        return *this;
    }
    JsonValue& JsonValue::operator = (const Json& sVal)
    {
        FreeRes();
        Assign(sVal);
        return *this;
    }
    JsonValue& JsonValue::operator = (const JsonArray& sArr)
    {
        FreeRes();
        Assign(sArr);
        return *this;
    }
    JsonValue& JsonValue::operator [] (const char* szKey)
    {
        if (szKey == NullPtr)
        {
            throw JsonException(GetErrMsg(E_NULL_PTR));
        }
        if (_nType != ObjectType)
        {
            throw JsonException(GetErrMsg(E_BAD_TYPE_CAST));
        }
        Json* pJS = _sVal._pJsonVal;
        
        JsonValue* pJV = pJS->Get(szKey);
        if (pJV == NullPtr)
        {
            JsonValue jv;
            pJV = pJS->Set(szKey, jv);
        }
        return *pJV;
    }
    const JsonValue& JsonValue::operator [] (const char* szKey) const
    {
        if (szKey == NullPtr)
        {
            throw JsonException(GetErrMsg(E_NULL_PTR));
        }
        if (_nType != ObjectType)
        {
            throw JsonException(GetErrMsg(E_BAD_TYPE_CAST));
        }
        Json* pJS = _sVal._pJsonVal;
        
        JsonValue* pJV = pJS->Get(szKey);
        if (pJV == NullPtr)
        {
            throw JsonException(GetErrMsg(E_KEY_NOT_EXISTS));
        }
        return *pJV;
    }
    void JsonValue::FreeRes()
    {
        switch(_nType)
        {
            case StringType:
                if (_sVal._strVal != NullPtr)
                {
                    delete[] _sVal._strVal;
                    _sVal._strVal = NullPtr;
                }
                break;
            case BoolType:
            case IntegerType:
            case DoubleType:
            case NullType:
                break;
            case ObjectType:
                if (_sVal._pJsonVal != NullPtr)
                {
                    delete _sVal._pJsonVal;
                    _sVal._pJsonVal = NullPtr;
                }
                break;
            case ArrayType:
                if (_sVal._pArrVal != NullPtr)
                {
                    delete _sVal._pArrVal;
                    _sVal._pArrVal = NullPtr;
                }
                break;
        }
        memset(&_sVal, 0, sizeof(_sVal));
    }
    
    void JsonValue::Assign(const JsonValue& sRight)
    {
        _nType = sRight._nType;
        Assign(sRight._sVal);
    }
    void JsonValue::Assign(const JsonVariant& sRight)
    {
        memset(&_sVal, 0, sizeof(_sVal));
        switch(_nType)
        {
            case StringType:
                if (sRight._strVal != NullPtr)
                {
                    int nLen = strlen(sRight._strVal);
                    _sVal._strVal = new char[nLen + 1];
                    memcpy(_sVal._strVal, sRight._strVal, nLen + 1);
                }
                break;
            case BoolType:
            case IntegerType:
            case DoubleType:
            case NullType:
                memcpy(&_sVal, &(sRight), sizeof(_sVal));
                break;
            case ObjectType:
                if (sRight._pJsonVal != NullPtr)
                {
                    _sVal._pJsonVal = new Json(*(sRight._pJsonVal));
                }
                break;
            case ArrayType:
                if (sRight._pArrVal != NullPtr)
                {
                    _sVal._pArrVal = new JsonArray(*(sRight._pArrVal));
                }
                break;
        }
    }
    void JsonValue::Assign(bool bVal)
    {
        _nType = BoolType;
        _sVal._boolVal = bVal;
    }
    void JsonValue::Assign(const char* szVal)
    {
        if(szVal == NullPtr)
        {
            _nType = NullType;
            _sVal._strVal = NullPtr;
        }
        else
        {
            _nType = StringType;
            int nLen = strlen(szVal);
            _sVal._strVal =  new char[nLen + 1];
            memcpy(_sVal._strVal, szVal, nLen + 1);
        }
    }
    void JsonValue::Assign(long long llVal)
    {
        _nType = IntegerType;
        _sVal._llVal = llVal;
    }
    void JsonValue::Assign(double fVal)
    {
        _nType = DoubleType;
        _sVal._fVal = fVal;
    }
    void JsonValue::Assign(const Json& sVal)
    {
        _nType = ObjectType;
        _sVal._pJsonVal = new Json(sVal);
    }
    void JsonValue::Assign(const JsonArray& sArr)
    {
        _nType = ArrayType;
        _sVal._pArrVal = new JsonArray(sArr);
    }
    JsonValue::operator const char*()
    {
        if (_nType != StringType)
        {
            throw JsonException(GetErrMsg(E_BAD_TYPE_CAST));
        }
        return _sVal._strVal;
    }
    JsonValue::operator char*()
    {
        if (_nType != StringType)
        {
            throw JsonException(GetErrMsg(E_BAD_TYPE_CAST));
        }
        return _sVal._strVal;
    }
    JsonValue::operator int()
    {
        if (_nType != IntegerType)
        {
            throw JsonException(GetErrMsg(E_BAD_TYPE_CAST));
        }
        return (int)_sVal._llVal;
    }
    
    JsonValue::operator long long()
    {
        if (_nType != IntegerType)
        {
            throw JsonException(GetErrMsg(E_BAD_TYPE_CAST));
        }
        return _sVal._llVal;
    }
    JsonValue::operator float()
    {
        if (_nType != DoubleType)
        {
            throw JsonException(GetErrMsg(E_BAD_TYPE_CAST));
        }
        return (float)_sVal._fVal;
    }
    JsonValue::operator double()
    {
        if (_nType != DoubleType)
        {
            throw JsonException(GetErrMsg(E_BAD_TYPE_CAST));
        }
        return (float)_sVal._fVal;
    }
    JsonValue::operator bool()
    {
        if (_nType != BoolType)
        {
            throw JsonException(GetErrMsg(E_BAD_TYPE_CAST));
        }
        return (bool)_sVal._boolVal;
    }
    JsonValue::operator Json()
    {
        if (_nType != ObjectType)
        {
            throw JsonException(GetErrMsg(E_BAD_TYPE_CAST));
        }
        return *(_sVal._pJsonVal);
    }
    JsonValue::operator JsonArray()
    {
        if (_nType != ArrayType)
        {
            throw JsonException(GetErrMsg(E_BAD_TYPE_CAST));
        }
        return *(_sVal._pArrVal);
    }
    
    ////////////////////////////////////////
    // JsonValueItem
    JsonValueItem::JsonValueItem(const char* szKey, JsonValue* pJV)
    : _pVal(0)
    , _pNextVal(0)
    {
        if (szKey)
        {
            SetKey(szKey);
        }
        if(pJV)
        {
            _pVal = new JsonValue(*pJV);
        }
    }
    JsonValueItem::JsonValueItem(const JsonValueItem& sRight)
    {
        Assign(sRight);
    }
    JsonValueItem::~JsonValueItem()
    {
        FreeRes();
    }
    JsonValueItem& JsonValueItem::operator = (const JsonValueItem& sRight)
    {
        if (this != &sRight)
        {
            FreeRes();
            Assign(sRight);
        }
        return *this;
    }
    void JsonValueItem::FreeRes()
    {
        if (_pVal)
        {
            delete _pVal;
            _pVal = NullPtr;
        }
        if (_pNextVal)
        {
            delete _pNextVal;
            _pNextVal = NullPtr;
        }
    }
    void JsonValueItem::Assign(const JsonValueItem& sRight)
    {
        if (sRight._pVal != NullPtr)
        {
            _pVal = new JsonValue(*sRight._pVal);
        }
        else
        {
            _pVal = NullPtr;
        }
        if (sRight._pNextVal != NullPtr)
        {
            _pNextVal = new JsonValueItem(*sRight._pNextVal);
        }
        else
        {
            _pNextVal = NullPtr;
        }
        _strKey = sRight._strKey;
    }
    int JsonValueItem::SetKey(const char* szKey)
    {
        if (szKey != NullPtr)
        {
            _strKey = szKey;
        }
        return E_NULL_PTR;
    }
    std::string JsonValueItem::GetKey() const
    {
        return _strKey;
    }
    ////////////////////////////////////////
    // Json
    Json::Json()
    : _pGrpVal(NullPtr)
    , _nLen(0)
    {
    }
    Json::~Json()
    {
        FreeRes();
    }
    Json::Json(const Json& sRight)
    {
        Assign(sRight);
    }
    Json& Json::operator = (const Json& sRight)
    {
        if (this != &sRight)
        {
            FreeRes();
            Assign(sRight);
        }
        return *this;
    }
    int Json::Parse(Json*& pJson, const char* pBuf)
    {
        pJson = NullPtr;
        if (pBuf == NullPtr)
        {
            return E_NULL_PTR;
        }
        
        
        SkipSpaces(pBuf);
        if (!EofBuffer(pBuf))
        {
            int nRet = ParseJson(pJson, pBuf);
            if (nRet == E_SUCCESS)
            {
                SkipSpaces(pBuf);
                if (!EofBuffer(pBuf))
                {
                    if (pJson)
                    {
                        delete pJson;
                        pJson = NullPtr;
                    }
                    
                    nRet = E_EXTRAL_CHARACTERS;
                }
                
            }
            return nRet;
            
        }
        return E_EMPTY_STRING;
    }
    int Json::Load(Json*& pJson, const char* pFilePath)
    {
        pJson = NullPtr;
        if (pFilePath == NullPtr)
        {
            return E_NULL_PTR;
        }
        FILE* pFile = fopen(pFilePath, "r");
        if (pFile == NullPtr)
        {
            return E_OPEN_FILE_ERROR;
        }
        
        std::string str;
        fseek(pFile, 0, SEEK_SET);
        long long lStart = ftell(pFile);
        fseek(pFile, 0, SEEK_END);
        long long lEnd = ftell(pFile);
        str.reserve((unsigned int)(lEnd - lStart));
        fseek(pFile, 0, SEEK_SET);
        int rd = 0;
        char szBuf[513];
        while (true)
        {
            rd = fread(szBuf, 1, 512, pFile);
            szBuf[rd] = 0;
            if (rd <= 0)
            {
                break;
            }
            str += szBuf;
        }
        fclose(pFile);
        
        
        return Parse(pJson, str.c_str());
    }
    
    int Json::Save(const char* pFilePath)
    {
        if (pFilePath == NullPtr)
        {
            return E_NULL_PTR;
        }
        FILE* pFile = fopen(pFilePath, "w");
        if (pFile == NullPtr)
        {
            return E_OPEN_FILE_ERROR;
        }
        
        std::string strDump;
        DumpFormat(strDump);
        
        int nLen = strDump.length();
        int wr = 0;
        int i = 0;
        const char* szBuf = strDump.c_str();
        while(nLen > 0)
        {
            wr = fwrite(szBuf + i, 1, nLen >= 512 ? 512 : nLen, pFile);
            i += wr;
            nLen -= wr;
        }
        
        fclose(pFile);
        
        return E_SUCCESS;
    }
    
    JsonValue* Json::Set(const char* szKey, JsonValue& sVal)
    {
        if (szKey == NullPtr)
        {
            throw JsonException(GetErrMsg(E_NULL_PTR));
        }
        JsonValue* pRet = NullPtr;
        if (_pGrpVal == NullPtr)
        {
            _pGrpVal = new JsonValueItem(szKey, &sVal);
            pRet = _pGrpVal->_pVal;
        }
        else
        {
            JsonValueItem* pGrpCur = _pGrpVal;
            JsonValueItem* pGrpPre = NullPtr;
            JsonValue* pJV = NullPtr;
            while(pGrpCur != NullPtr)
            {
                pJV = pGrpCur->_pVal;
                int nCmp = strcmp(szKey, pGrpCur->_strKey.c_str());
                if ( nCmp == 0)
                {
                    return NullPtr;
                }
                else if (nCmp < 0)
                {
                    // pGrpPre ����
                    break;
                }
                else
                {
                    pGrpPre = pGrpCur;
                    pGrpCur = pGrpPre->_pNextVal;
                }
            }
            if (pGrpPre == NullPtr)
            {
                pGrpPre = new JsonValueItem(szKey, &sVal);
                pRet = pGrpPre->_pVal;
                pGrpPre->_pNextVal = _pGrpVal;
                _pGrpVal = pGrpPre;
            }
            else
            {
                pGrpCur = new JsonValueItem(szKey, &sVal);
                pRet = pGrpCur->_pVal;
                pGrpCur->_pNextVal = pGrpPre->_pNextVal;
                pGrpPre->_pNextVal = pGrpCur;
            }
        }
        _nLen++; 
        return pRet;
    }
    JsonValue* Json::Get(const char* szKey) const
    {
        JsonValue* pJV = NullPtr;
        if (szKey)
        {
            JsonValueItem* pJVI = _pGrpVal;
            while(pJVI)
            {
                if (strcmp(szKey, pJVI->_strKey.c_str()) == 0)
                {
                    pJV = pJVI->_pVal;
                    return pJV;
                }
                pJVI = pJVI->_pNextVal;
            }
        }
        return NullPtr;
    }
    
    void Json::Dump(std::string& strDump) const
    {
        DumpValueGroup(_pGrpVal, strDump, 0);
    }
    void Json::DumpFormat(std::string& strDump, int nSpace) const
    {
        DumpValueGroup(_pGrpVal, strDump, nSpace);
    }
    
    void Json::DumpValueGroup(JsonValueItem* pGroup, std::string& strDump, int nSpace) const
    {
        strDump += "{";
        if (nSpace > 0)
        {
            strDump += "\n";
        }
        JsonValueItem* pGrp = pGroup;
        while(pGrp)
        {
            JsonValue* pJV = pGrp->_pVal;
            for (int i=0; i<nSpace; i++)
            {
                strDump += " ";
            }
            strDump += "\"";
            strDump += pGrp->_strKey;
            strDump += "\" : ";
            
            DumpValue(pJV, strDump, nSpace);
            
            pGrp = pGrp->_pNextVal;
            if (pGrp != NullPtr)
            {
                if (nSpace > 0)
                {
                    strDump += ",\n";
                }
                else
                {
                    strDump += ",";
                }
                
            }
        }
        if (nSpace > 0)
        {
            strDump += "\n";
            for (int i=0; i<nSpace - FORMAT_SPACE; i++)
            {
                strDump += " ";
            }
        }
        
        strDump += "}";
    }
    void Json::DumpArray(JsonArray* pArr, std::string& strDump, int nSpace) const
    {
        if (pArr)
        {
            strDump += "[";
            if (nSpace > 0)
            {
                strDump += "\n";
            }
            int nSize = pArr->GetSize() - 1;
            for(int i=0; i <= nSize; i++)
            {
                JsonValue& pJV = pArr->Get(i);
                for (int j=0; j<nSpace; j++)
                {
                    strDump += " ";
                }
                DumpValue(&pJV, strDump, nSpace);
                if (i != nSize)
                {
                    if (nSpace > 0)
                    {
                        strDump += ",\n";
                    }
                    else
                    {
                        strDump += ",";
                    }
                }
                
            }
            if (nSpace > 0)
            {
                strDump += "\n";
                for (int i=0; i<nSpace - FORMAT_SPACE; i++)
                {
                    strDump += " ";
                }
            }        
            strDump += "]";
        }
    }
    void Json::DumpValue(JsonValue* pJV, std::string& strDump, int nSpace) const
    {
        if (pJV)
        {
            switch(pJV->_nType)
            {
                case StringType:
                    if (pJV->_sVal._strVal != NullPtr)
                    {
                        strDump += "\"";
                        strDump += pJV->_sVal._strVal;
                        strDump += "\"";
                    }
                    else
                    {
                        strDump += "\"\"";
                    }
                    break;
                case BoolType:
                    if (pJV->_sVal._boolVal)
                    {
                        strDump += "true";
                    }
                    else
                    {
                        strDump += "false";
                    }
                    break;
                case IntegerType:
                {
                    char szBuf[20] = {0};
                    sprintf(szBuf, "%lld", pJV->_sVal._llVal);
                    strDump += szBuf;
                }
                    break;
                case DoubleType:
                {
                    char szBuf[20] = {0};
                    sprintf(szBuf, "%lf", pJV->_sVal._fVal);
                    strDump += szBuf;
                }
                    break;
                case NullType:
                    strDump += "null";
                    break;
                case ObjectType:
                    if (pJV->_sVal._pJsonVal != NullPtr)
                    {
                        pJV->_sVal._pJsonVal->DumpValueGroup(
                                                             pJV->_sVal._pJsonVal->_pGrpVal, strDump, nSpace >0 ? nSpace + FORMAT_SPACE : nSpace);
                    }
                    else
                    {
                        strDump += "{}";
                    }
                    break;
                case ArrayType:
                    if (pJV->_sVal._pArrVal != NullPtr)
                    {
                        DumpArray(pJV->_sVal._pArrVal, strDump, nSpace >0 ? nSpace + FORMAT_SPACE : nSpace);
                    }
                    else
                    {
                        strDump += "[]";
                    }
                    break;
            }
        }
    }
    
    void Json::FreeRes()
    {
        if (_pGrpVal)
        {
            delete _pGrpVal;
            _pGrpVal = NullPtr;
        }
    }
    void Json::Assign(const Json& sRight)
    {
        if (sRight._pGrpVal)
        {
            _pGrpVal = new JsonValueItem(*sRight._pGrpVal);
        }
        else
        {
            _pGrpVal = NullPtr;
        }
        _nLen = sRight._nLen;
    }
    JsonValue& Json::operator [] (const char* szKey)
    {
        if (szKey == NullPtr)
        {
            throw JsonException(GetErrMsg(E_NULL_PTR));
        }
        JsonValue* pJV = Get(szKey);
        if (pJV == NullPtr)
        {
            JsonValue jv;
            pJV = Set(szKey, jv);
        }
        return *pJV;
    }
    const JsonValue& Json::operator [] (const char* szKey) const
    {
        if (szKey == NullPtr)
        {
            throw JsonException(GetErrMsg(E_NULL_PTR));
        }
        JsonValue* pJV = Get(szKey);
        if (pJV == NullPtr)
        {
            throw JsonException(GetErrMsg(E_KEY_NOT_EXISTS));
        }
        return *pJV;
    }
    
    void Json::SkipSpaces(const char*& szBuf)
    {
        while(EofBuffer(szBuf) == false)
        {
            if (*szBuf == ' ' ||
                *szBuf == '\t' ||
                *szBuf == '\r' ||
                *szBuf == '\n')
            {
                szBuf++;
            }
            else
            {
                bool bSkipCmt = false;
                if (*szBuf == '/')
                {
                    if (*(szBuf + 1) && *(szBuf + 1) == '/')
                    {
                        bSkipCmt = true;
                    }
                }
                else
                {
                    if (*szBuf == '#')
                    {
                        bSkipCmt = true;
                    }
                }
                if (bSkipCmt)
                {
                    while(EofBuffer(szBuf) == false)
                    {
                        szBuf++;
                        if(*szBuf == '\n')
                        {
                            szBuf++;
                            break;
                        }
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
    bool Json::EofBuffer(const char*& szBuf)
    {
        if (szBuf == NullPtr || *szBuf == '\0')
        {
            return true;
        }
        return false;
    }
    int Json::ParseJson(Json*& pJS, const char*& szBuf)
    {
        if (szBuf == NullPtr)
        {
            return E_NULL_PTR;
        }
        if (*szBuf != '{')
        {
            return E_BRACE_MISSING;
        }
        
        int nRet = E_NOT_JSON_SYNTAX;
        bool nComma = false;
        bool bEndBrace = false;
        bool bHasVal = false;
        szBuf++;
        SkipSpaces(szBuf);
        while(!EofBuffer(szBuf))
        {
            nRet = E_SUCCESS;
            if (*szBuf == '\"')
            {
                int nStrLen = 0;
                const char* pStart = NullPtr;
                nRet = ParseKey(szBuf, pStart, nStrLen);
                if ( nRet != E_SUCCESS)
                {
                    break;
                }
                if (nStrLen == 0)
                {
                    nRet = E_KEY_MISSING;
                    break;
                }
                SkipSpaces(szBuf);
                if (EofBuffer(szBuf))
                {
                    nRet = E_COLON_MISSING;
                    break;
                }
                
                if (*szBuf != ':')
                {
                    nRet = E_COLON_MISSING;
                    break;
                }
                
                szBuf++;
                SkipSpaces(szBuf);
                if (EofBuffer(szBuf))
                {
                    nRet = E_KEY_MISSING;
                    break;
                }
                
                JsonValue* pJV = NullPtr;
                nRet = ParseJsonValue(pJV, szBuf);
                if (nRet != E_SUCCESS)
                {
                    nRet = E_PARSE_JASON_VALUE;
                    break;
                }
                nComma = false;
                if (pJS == NullPtr)
                {
                    pJS = new Json;                       
                }
                std::string strKey(pStart, nStrLen);
                if (!pJS->Set(strKey.c_str(), *pJV))
                {
                    if(pJV) delete pJV;
                    nRet = E_DUPLICATE_KEY;
                    break;
                }
                bHasVal = true;
                if(pJV) delete pJV;
            }
            else if (*szBuf == ',')
            {
                if (bHasVal == true)
                {
                    nComma = true;
                    szBuf++;
                }
                else
                {
                    nRet = E_EXTRAL_COMMA;
                    break;
                }
                
            }
            else if (*szBuf == '}')
            {
                if (nComma)
                {
                    nRet = E_EXTRAL_COMMA;
                }
                else
                {
                    bEndBrace = true;
                    szBuf++;
                    SkipSpaces(szBuf);
                }            
                break;
            }
            else
            {
                nRet = E_KEY_MISSING;
                break;
            }
            //      szBuf++;
            SkipSpaces(szBuf);
        }
        if (!bEndBrace)
        {
            nRet = E_PARSE_JASON_OBJECT;
        }
        if(nRet != E_SUCCESS)
        {
            if (pJS != NullPtr)
            {
                delete pJS;
                pJS = NullPtr;
            }
        }
        return nRet;
    }
    int Json::ParseKey(const char*& szBuf, const char*& pStart, int& nLen)
    {
        nLen = 0;
        if (szBuf == NullPtr)
        {
            return E_NULL_PTR;
        }
        if (*szBuf != '\"')
        {
            return E_QUOT_MISSING;
        }
        szBuf++;
        pStart = szBuf;
        const char* pEnd = NullPtr;
        while(!EofBuffer(szBuf))
        {
            if (*szBuf == '\"')
            {
                pEnd = szBuf;
                szBuf++;
                break;
            }
            szBuf++;
        }
        if (pEnd == NullPtr)
        {
            return E_KEY_MISSING;
        }
        nLen = (int)(pEnd - pStart);
        return E_SUCCESS;
    }
    int Json::ParseJsonValue(JsonValue*& pJV, const char*& szBuf)
    {
        if (szBuf == NullPtr)
        {
            return E_NULL_PTR;
        }
        int nRet = E_SUCCESS;
        if (!EofBuffer(szBuf))
        {
            if (*szBuf == '{')
            {
                Json* pJS = NullPtr;
                nRet = ParseJson(pJS, szBuf);
                if (nRet != E_SUCCESS)
                {
                    nRet = E_PARSE_JASON_OBJECT;
                }
                else
                {
                    if (pJV == NullPtr)
                    {
                        pJV = new JsonValue(*pJS);
                        
                        if(pJS) delete pJS;
                    }
                }
                
            }
            else if (*szBuf == '[')
            {
                JsonArray* pJArr = NullPtr;
                nRet = ParseJsonArray(pJArr, szBuf);
                if (nRet != E_SUCCESS)
                {
                    nRet = E_PARSE_JASON_ARRAY;
                }
                else
                {
                    if (pJV == NullPtr)
                    {
                        pJV = new JsonValue(*pJArr);
                        
                        if(pJArr) delete pJArr;
                    }
                }
                
            }
            else if (*szBuf == 'n')
            {
                // null
                if (strncmp(szBuf, "null", 4) == 0)
                {
                    if (pJV == NullPtr)
                    {
                        pJV = new JsonValue((char*)0);
                    }
                    szBuf += 4;
                }
                else
                {
                    nRet = E_UNKNOWN_VAULE;
                }
            }
            else if (*szBuf == 'f')
            {
                // false
                if (strncmp(szBuf, "false", 5) == 0)
                {
                    if (pJV == NullPtr)
                    {
                        pJV = new JsonValue(false);
                    }
                    szBuf += 5;
                }
                else
                {
                    nRet = E_UNKNOWN_VAULE;
                }
            }
            else if (*szBuf == 't')
            {
                // true
                if (strncmp(szBuf, "true", 4) == 0)
                {
                    if (pJV == NullPtr)
                    {
                        pJV = new JsonValue(true);
                    }
                    szBuf += 4;
                }
                else
                {
                    nRet = E_UNKNOWN_VAULE;
                }
            }
            else if (*szBuf == '\"')
            {
                int nStrLen = 0;
                const char* pStart = NullPtr;
                nRet = ParseKey(szBuf, pStart, nStrLen);
                if(nRet != E_SUCCESS)
                {
                    nRet = E_STRING_VALUE_ERROR;
                }
                else
                {
                    std::string strVal(pStart, nStrLen);
                    if (pJV == NullPtr)
                    {
                        pJV = new JsonValue(strVal.c_str());
                    }
                }
            }
            else
            {
                // integer&double
                const char* pStart = szBuf;
                while(!EofBuffer(szBuf))
                {
                    if (*szBuf == ' ' ||
                        *szBuf == '\t' ||
                        *szBuf == ',' ||
                        *szBuf == ']' ||
                        *szBuf == '}' ||
                        *szBuf == '\r' ||
                        *szBuf == '\n')
                    {
                        if (*szBuf != ']' &&
                            *szBuf != '}')
                        {
                            szBuf++;
                        }
                        
                        break;
                    }
                    else
                    {
                        szBuf++;
                    }
                }
                
                int sub = szBuf - pStart;
                /*if(sub == 1)
                 sub ++;*/
                //std::string str(pStart, sub - 1);
                std::string str(pStart,sub);
                //std::string str(pStart, szBuf - pStart - 1);
                NumberType type = detectNumberType(str.c_str());
                switch (type)
                {
                    case Unkown:
                        nRet = E_ENKNOW_NUMBER;
                        break;
                    case Int:
                    {
                        bool bStat = false;
                        long long llv = toInt(str.c_str(), bStat);
                        if (bStat)
                        {
                            pJV = new JsonValue(llv);
                        }
                    }
                        break;
                    case Double:
                    {
                        bool bStat = false;
                        double dv = toDouble(str.c_str(), bStat);
                        if (bStat)
                        {
                            pJV = new JsonValue(dv);
                        }
                    }
                        break;
                }
                
            }
            if(nRet != E_SUCCESS)
            {
                if (pJV != NullPtr)
                {
                    delete pJV;
                    pJV = NullPtr;
                }
            }
        }
        else
        {
            nRet = E_VALUE_MISSING;
        }
        
        
        return nRet;
    }
    int Json::ParseJsonArray(JsonArray*& pJArr, const char*& szBuf)
    {
        if (szBuf == NullPtr)
        {
            return E_NULL_PTR;
        }
        if (*szBuf != '[')
        {
            return E_BRACKET_MISSING;
        }
        int nRet = E_SUCCESS;
        bool nComma = false;
        bool bEndBrace = false;
        bool bHasVal = false;
        szBuf++;
        SkipSpaces(szBuf);
        while (!EofBuffer(szBuf))
        {
            if (*szBuf == ',')
            {
                if(bHasVal)
                {
                    szBuf++;
                    nComma = true;
                }
                else
                {
                    nRet = E_EXTRAL_COMMA;
                    break;
                }
            }
            else if (*szBuf == ']')
            {
                if (nComma)
                {
                    nRet = E_EXTRAL_COMMA;
                }
                else
                {
                    bEndBrace = true;
                    szBuf++;
                }
                break;
            }
            else if (*szBuf == '[')
            {
                JsonArray* pNewJArr = NullPtr;
                nRet = ParseJsonArray(pNewJArr, szBuf);
                if(nRet != E_SUCCESS)
                {
                    break;
                }
                if (pJArr == NullPtr)
                {
                    pJArr = new JsonArray;
                }
                JsonValue jv(*pNewJArr);
                pJArr->Add(jv);
                
                if(pNewJArr) delete pNewJArr;
                
                bHasVal = true;
                nComma = false;
            }
            else
            {
                JsonValue* pNewJV = NullPtr;
                nRet = ParseJsonValue(pNewJV, szBuf);
                if(nRet != E_SUCCESS)
                {
                    break;
                }
                if (pJArr == NullPtr)
                {
                    pJArr = new JsonArray;
                }
                pJArr->Add(*pNewJV);
                
                if(pNewJV) delete pNewJV;
                bHasVal = true;
                nComma = false;
            }
            SkipSpaces(szBuf);
        }
        if (!bEndBrace)
        {
            nRet = E_BRACKET_MISSING;
        }
        if (nRet != E_SUCCESS)
        {
            if (pJArr != NullPtr)
            {
                delete pJArr;
            }
        }
        else
        {
            if (pJArr == NullPtr)
            {
                pJArr = new JsonArray;
            }
        }
        return nRet;
    }
    
}


#ifdef _MSC_VER
#pragma warning(pop)
#endif
