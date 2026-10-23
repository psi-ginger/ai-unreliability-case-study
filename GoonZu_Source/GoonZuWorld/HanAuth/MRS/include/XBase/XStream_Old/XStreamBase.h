#ifndef __XStreamBase_Old__
#define __XStreamBase_Old__

#ifdef _UNIX_
    #define XSTREAM_API
#else
#ifdef XSTREAM_EXPORTS
    #define XSTREAM_API __declspec(dllexport)
#else
    #define XSTREAM_API __declspec(dllimport)
#endif
#endif

class XSTREAM_API IXStream
{
public:
    virtual ~IXStream(void) {}

    virtual int Read(void *p, int nCount) = 0;
    virtual int Write(const void *p, int nCount) = 0;
    virtual int CopyTo(void *p, int nCount) const = 0;
    virtual int RemoveLeft(int nCount) = 0;
    virtual int RemoveRight(int nCount) = 0;
    virtual void Clear(void) = 0;
    virtual int GetLength(void) const = 0;
    virtual int SaveToStream(IXStream *pStream, int nCount) const = 0;
    virtual int LoadFromStream(IXStream *pStream, int nCount) = 0;
};

#endif
