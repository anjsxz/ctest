#include "CMemBuffer.h"
#include "stdafx.h"
CMemBuffer::CMemBuffer()
{
    bzero(&m_data,sizeof(DATA));
}

CMemBuffer::~CMemBuffer()
{
    if(m_data.pData)
        free(m_data.pData);
}

LPSTR CMemBuffer::GetBuffer(int nLength)
{
    LPSTR pTmp;
    
    pTmp=(LPSTR)malloc(nLength);
    bzero(pTmp,nLength);
    
    if(m_data.nDataLength>0)
    {    
        
        
        if(m_data.nDataLength<=nLength)
            memcpy(pTmp,m_data.pData,m_data.nDataLength);
        else
            memcpy(pTmp,m_data.pData,nLength);    
        
        if(m_data.pData)
            free(m_data.pData);
        
    }
    
    m_data.pData=pTmp;
    m_data.nDataLength=nLength;
    
    return m_data.pData;
}

void CMemBuffer::ReleaseBuffer()
{
    if(m_data.pData)
        free(m_data.pData);
    m_data.pData=0;
    m_data.nDataLength=0;
}

int  CMemBuffer::Find(char ch)
{
    for(int i=0;i<m_data.nDataLength;i++)
    {
        if(m_data.pData[i]==(BYTE)ch)
            return i;
    }
    
    return -1;
}

bool CMemBuffer::Delete(int nIdx,int nLength)
{
	int nLen=m_data.nDataLength-nIdx;
	if(nIdx<0 || nLength<1 || nLen<nLength)
	{
        //	AddToErrorLog(MEMBUFFERLOG,"CMemBuffer::Remove( nIdx=%d, nLength=%d)",nIdx,nLength);
		return false;
	}
	LPSTR pTmp = (LPSTR)malloc(m_data.nDataLength-nLength);
    
	if(nIdx>0)
	{
		memcpy(pTmp,m_data.pData,nIdx);
	}
	if( (nLen-nLength)>0 )
		memcpy(pTmp+nIdx,m_data.pData+nIdx+nLength,nLen-nLength);
    
	free(m_data.pData);
	m_data.pData=pTmp;  
	m_data.nDataLength -= nLength;
	return true;
}