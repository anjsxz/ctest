
#include "stdafx.h"
#include <zlib.h>
#include <assert.h>
#include <stdlib.h>

#include "ZipUtils.h"



// memory in iPhone is precious
// Should buffer factor be 1.5 instead of 2 ?
#define BUFFER_INC_FACTOR (2)

/*decompress*/
int ZipUtils::ccInflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int *outLength, unsigned int outLenghtHint)
{
    /* ret value */
    int err = Z_OK;
    
    int bufferSize = outLenghtHint;
    *out = new unsigned char[bufferSize];
    
    z_stream d_stream; /* decompression stream */
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    
    d_stream.next_in  = in;
    d_stream.avail_in = inLength;
    d_stream.next_out = *out;
    d_stream.avail_out = bufferSize;
    
    /* window size to hold 256k */
    if( (err = inflateInit2(&d_stream, 15 + 32)) != Z_OK )
        return err;
    
    for (;;)
    {
        err = inflate(&d_stream, Z_NO_FLUSH);
        
        if (err == Z_STREAM_END)
        {
            break;
        }
        
        switch (err)
        {
            case Z_NEED_DICT:
                err = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                inflateEnd(&d_stream);
                return err;
        }
        
        // not enough memory ?
        if (err != Z_STREAM_END)
        {
            unsigned char *tmp = *out;
            
            *out = new unsigned char[bufferSize * 2];
            memcpy(*out,tmp,bufferSize);
            
            delete [] tmp;
            
            
            /* not enough memory, ouch */
            if (! *out )
            {
                inflateEnd(&d_stream);
                return Z_MEM_ERROR;
            }
            
            d_stream.next_out = *out + bufferSize;
            d_stream.avail_out = bufferSize;
            bufferSize *= 2;
        }
    }
    
    *outLength = bufferSize - d_stream.avail_out;
    err = inflateEnd(&d_stream);
    return err;
}

/*compress*/
int ZipUtils::ccDeflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int *outLength,unsigned int outLenghtHint)
{
    /* ret value */
    int err = Z_OK;
    
    int bufferSize = outLenghtHint;
    *out = new unsigned char[bufferSize];
    
    z_stream c_stream;
    c_stream.zalloc = (alloc_func)0;
    c_stream.zfree = (free_func)0;
    c_stream.opaque = (voidpf)0;
    
    c_stream.next_in  = in;
    c_stream.avail_in = inLength;
    c_stream.next_out = *out;
    c_stream.avail_out = bufferSize;
    
    /* window size to hold 256k */
    if( (err = deflateInit(&c_stream, Z_DEFAULT_COMPRESSION)) != Z_OK )
        return err;
    
    for (;;)
    {
        err = deflate(&c_stream, Z_FINISH);
        
        if (err == Z_STREAM_END)
        {
            break;
        }
        
        switch (err)
        {
            case Z_NEED_DICT:
                err = Z_DATA_ERROR;
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                deflateEnd(&c_stream);
                return err;
        }
        
        // not enough memory ?
        if (err != Z_STREAM_END)
        {
            unsigned char *tmp = *out;
            
            *out = new unsigned char[bufferSize * 2];
            memcpy(*out,tmp,bufferSize);
            
            delete [] tmp;
            
            /* not enough memory, ouch */
            if (! *out )
            {
                deflateEnd(&c_stream);
                return Z_MEM_ERROR;
            }
            
            c_stream.next_out = *out + bufferSize;
            c_stream.avail_out = bufferSize;
            bufferSize *= 2;
        }
    }
    
    *outLength = c_stream.total_out;
    err = deflateEnd(&c_stream);
    return err;
}


int ZipUtils::ccDeflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int outLengthHint)
{
    unsigned int outLength = 0;
    int err = ccDeflateMemoryWithHint(in, inLength, out, &outLength, outLengthHint);
    
    if (err != Z_OK || *out == NULL) {
        if (err == Z_MEM_ERROR)
        {
            printf("cocos2d: ZipUtils: Out of memory while decompressing map data!");
        } else
            if (err == Z_VERSION_ERROR)
            {
                printf("cocos2d: ZipUtils: Incompatible zlib version!");
            } else
                if (err == Z_DATA_ERROR)
                {
                    printf("cocos2d: ZipUtils: Incorrect zlib compressed data!");
                }
                else
                {
                    printf("cocos2d: ZipUtils: Unknown error while decompressing map data!");
                }
        
        delete[] *out;
        *out = NULL;
        outLength = 0;
    }
    
    return outLength;
}


int ZipUtils::ccInflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int outLengthHint)
{
    unsigned int outLength = 0;
    int err = ccInflateMemoryWithHint(in, inLength, out, &outLength, outLengthHint);

    if (err != Z_OK || *out == NULL) {
        if (err == Z_MEM_ERROR)
        {
            printf("cocos2d: ZipUtils: Out of memory while decompressing map data!");
        } else 
        if (err == Z_VERSION_ERROR)
        {
            printf("cocos2d: ZipUtils: Incompatible zlib version!");
        } else 
        if (err == Z_DATA_ERROR)
        {
            printf("cocos2d: ZipUtils: Incorrect zlib compressed data!");
        }
        else
        {
            printf("cocos2d: ZipUtils: Unknown error while decompressing map data!");
        }

        delete[] *out;
        *out = NULL;
        outLength = 0;
    }

    return outLength;
}

int ZipUtils::ccInflateMemory(unsigned char *in, unsigned int inLength, unsigned char **out)
{
    // 256k for hint
    return ccInflateMemoryWithHint(in, inLength, out, 256 * 1024);
}

int ZipUtils::ccDeflateMemory(unsigned char *in, unsigned int inLength, unsigned char **out)
{
    // 256k for hint
    return ccDeflateMemoryWithHint(in, inLength, out, 256 * 1024);
}

