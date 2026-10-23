#ifndef __XMemoryStream_Old__
#define __XMemoryStream_Old__

#include "XStreamBase.h"
#include <XBase/XSystem.h>

class XSTREAM_API XMemoryStream : public IXStream
{
private:
    XMemory m_Stream;

public:
    XMemoryStream(void);
    virtual ~XMemoryStream(void);

    virtual int Read(void *p, int nCount);
    virtual int Write(const void *p, int nCount);
    virtual int CopyTo(void *p, int nCount) const;
    virtual int RemoveLeft(int nCount);
    virtual int RemoveRight(int nCount);
    virtual void Clear(void);
    virtual int GetLength(void) const;
    virtual int SaveToStream(IXStream *pStream, int nCount) const;
    virtual int LoadFromStream(IXStream *pStream, int nCount);

    void Attach(const XMemory &Memory);
    XMemory Detach(void);
    XMemory GetXMemory(void) const;
};

#endif
