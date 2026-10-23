#ifndef __XDequeStream_Old__
#define __XDequeStream_Old__

#include "XStreamBase.h"

class XSTREAM_API XDequeStream : public IXStream
{
private:
    struct XDequeStreamImpl;
    mutable XDequeStreamImpl *m_pStream;

public:
    XDequeStream(void);
    virtual ~XDequeStream(void);

    virtual int Read(void *p, int nCount);
    virtual int Write(const void *p, int nCount);
    virtual int CopyTo(void *p, int nCount) const;
    virtual int RemoveLeft(int nCount);
    virtual int RemoveRight(int nCount);
    virtual void Clear(void);
    virtual int GetLength(void) const;
    virtual int SaveToStream(IXStream *pStream, int nCount) const;
    virtual int LoadFromStream(IXStream *pStream, int nCount);
};

#endif
