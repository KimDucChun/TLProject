#pragma once
#include "../TKObject.h"

enum EFileOpenMode 
{
    efomREAD,
    efomWRITE,
    efomAPPEND,
    efomREADWRITE_EXISTFILE,
    efomREADWRITE_NEWFILE,
    efomREADAFFEND,
    efomREAD_BINARY,
    efomWRITE_BINARY,
    efomAPPEND_BINARY,
    efomREADWRITE_EXISTFILE_BINARY,
    efomREADWRITE_NEWFILE_BINARY,
    efomREADAFFEND_BINARY
};

class TKFile : public TKObject
{
private:
    bool IsEof;

protected:
    enum {  FREAD_BUFFER_SIZE   = 1024  };

    char ReadBuffer[FREAD_BUFFER_SIZE];

    char    FileName[255];
    FILE    *fp;
    
    EFileOpenMode FileOpenMode;
    char    chFileOpenMode[10];

public:
    TKFile(void);
    virtual ~TKFile(void);

    FILE * GetFile(void) { return fp; }

    bool FileOpen(const char *FileName, EFileOpenMode FileOpenMode);
    void FileClose(void);

    bool WriteLine(char *pString);
    bool Write(void *pWriteBuffer, int BufferSize, int RepeatCount);
    //bool WriteFormat(char *Format, ...);

    bool Read(void *pReadBuffer, int BufferSize, int RepeatCount);
    bool ReadLine(char *pString, const int BufferSize);
    bool ReadScanf(const int ParamCount, ...);

    bool Eof(void);
    bool Begin(void);
    bool MoveToNextLine(void);
};