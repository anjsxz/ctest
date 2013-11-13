
#ifndef __SUPPORT_ZIPUTILS_H__
#define __SUPPORT_ZIPUTILS_H__


    /* XXX: pragma pack ??? */
    /** @struct CCZHeader
    */
    struct CCZHeader {
        unsigned char            sig[4];                // signature. Should be 'CCZ!' 4 bytes
        unsigned short            compression_type;    // should 0
        unsigned short            version;            // should be 2 (although version type==1 is also supported)
        unsigned int             reserved;            // Reserved for users.
        unsigned int            len;                // size of the uncompressed file
    };

    enum {
        CCZ_COMPRESSION_ZLIB,                // zlib format.
        CCZ_COMPRESSION_BZIP2,                // bzip2 format (not supported yet)
        CCZ_COMPRESSION_GZIP,                // gzip format (not supported yet)
        CCZ_COMPRESSION_NONE,                // plain (not supported yet)
    };

    class ZipUtils
    {
    public:
        /** 
        * Inflates either zlib or gzip deflated memory. The inflated memory is
        * expected to be freed by the caller.
        *
        * It will allocate 256k for the destination buffer. If it is not enough it will multiply the previous buffer size per 2, until there is enough memory.
        * @returns the length of the deflated buffer
        *
        @since v0.8.1
        */
        static int ccInflateMemory(unsigned char *in, unsigned int inLength, unsigned char **out);
        static int ccDeflateMemory(unsigned char *in, unsigned int inLength, unsigned char **out);


        /** 
        * Inflates either zlib or gzip deflated memory. The inflated memory is
        * expected to be freed by the caller.
        *
        * outLenghtHint is assumed to be the needed room to allocate the inflated buffer.
        *
        * @returns the length of the deflated buffer
        *
        @since v1.0.0
        */
        static int ccInflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int outLenghtHint);
        
        static int ccDeflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int outLenghtHint);

        /** inflates a GZip file into memory
        *
        * @returns the length of the deflated buffer
        *
        * @since v0.99.5
        */
        static int ccInflateGZipFile(const char *filename, unsigned char **out);

        /** inflates a CCZ file into memory
        *
        * @returns the length of the deflated buffer
        *
        * @since v0.99.5
        */
        static int ccInflateCCZFile(const char *filename, unsigned char **out);

    private:
        static int ccInflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int *outLength, 
            unsigned int outLenghtHint);
        
        static int ccDeflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int *outLength,
                                           unsigned int outLenghtHint);
    };


#endif // __PLATFORM_WOPHONE_ZIPUTILS_H__

