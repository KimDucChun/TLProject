#include "./tkfile.h"

TKFile::TKFile(void)
:fp(NULL)
,IsEof(false)
{
}

TKFile::~TKFile(void)
{
    this->FileClose();
}

bool TKFile::FileOpen(const char *FileName, EFileOpenMode FileOpenMode)
{
    strcpy(this->FileName, FileName);
    this->FileOpenMode = FileOpenMode;

    switch (FileOpenMode)
    {
        case efomREAD:
            strcpy(chFileOpenMode, "rt");
            break;
        case efomWRITE:
            strcpy(chFileOpenMode, "wt");
            break;
        case efomAPPEND:
            strcpy(chFileOpenMode, "at");
            break;
        case efomREADWRITE_EXISTFILE:
            strcpy(chFileOpenMode, "r+t");
            break;
        case efomREADWRITE_NEWFILE:
            strcpy(chFileOpenMode, "w+t");
            break;
        case efomREADAFFEND:
            strcpy(chFileOpenMode, "a+t");
            break;
        case efomREAD_BINARY:
            strcpy(chFileOpenMode, "rb");
            break;
        case efomWRITE_BINARY:
            strcpy(chFileOpenMode, "wb");
            break;
        case efomAPPEND_BINARY:
            strcpy(chFileOpenMode, "ab");
            break;
        case efomREADWRITE_EXISTFILE_BINARY:
            strcpy(chFileOpenMode, "r+b");
            break;
        case efomREADWRITE_NEWFILE_BINARY:
            strcpy(chFileOpenMode, "w+b");
            break;
        case efomREADAFFEND_BINARY:
            strcpy(chFileOpenMode, "a+b");
            break;
    }

    fp = fopen(FileName, chFileOpenMode);
    if ( !fp )
    {
        MessageBox(0, FileName, "File Open Error", MB_ICONERROR);
    }
    _ASSERT( fp );

    if (fp == NULL)
    {
        MessageBox(0, "File Not Found", "Error", MB_ICONERROR);  // MB_ICONERROR
        return false;
    }
    IsEof = false;

    return true;
}

void TKFile::FileClose(void)
{
    if (fp != NULL)
    {
        fclose(fp);
        fp = NULL;
    }
}

bool TKFile::WriteLine(char *pString)
{
    fprintf(fp, "%s\r\n", pString);
    return true;
}

bool TKFile::Write(void *pWriteBuffer, int BufferSize, int RepeatCount)
{
    if (BufferSize > 0)
    {
        if ( !fwrite(pWriteBuffer, BufferSize, RepeatCount, fp) )
        {
            return false;
        }
    }
    return true;
}

//bool TKFile::WriteFormat(char *Format, ...)
//{
    //va_list ap;
    //void *pArg;
    //int sLen = strlen(Format);

    ////%c int or wint_t When used with printf functions, specifies a single-byte character; when used with wprintf functions, specifies a wide character. 
    ////%C int or wint_t When used with printf functions, specifies a wide character; when used with wprintf functions, specifies a single-byte character. 
    ////%d int Signed decimal integer. 
    ////%i int Signed decimal integer. 
    ////%o int Unsigned octal integer. 
    ////%u int Unsigned decimal integer. 
    ////%x int Unsigned hexadecimal integer, using "abcdef." 
    ////%X int Unsigned hexadecimal integer, using "ABCDEF." 
    ////%e double Signed value having the form [ ? ]d.dddd e [sign]ddd where d is a single decimal digit, dddd is one or more decimal digits, ddd is exactly three decimal digits, and sign is + or ?. 
    ////%E double Identical to the e format except that E rather than e introduces the exponent. 
    ////%f double Signed value having the form [ ? ]dddd.dddd, where dddd is one or more decimal digits. The number of digits before the decimal point depends on the magnitude of the number, and the number of digits after the decimal point depends on the requested precision. 
    ////%g double Signed value printed in f or e format, whichever is more compact for the given value and precision. The e format is used only when the exponent of the value is less than ?4 or greater than or equal to the precision argument. Trailing zeros are truncated, and the decimal point appears only if one or more digits follow it. 
    ////%G double Identical to the g format, except that E, rather than e, introduces the exponent (where appropriate). 
    ////%n Pointer to integer  Number of characters successfully written so far to the stream or buffer; this value is stored in the integer whose address is given as the argument. 
    ////%p Pointer to void Prints the address of the argument in hexadecimal digits. 
    ////%s String When used with printf functions, specifies a single-byte?character string; when used with wprintf functions, specifies a wide-character string. Characters are printed up to the first null character or until the precision value is reached. 
    ////%S String When used with printf functions, specifies a wide-character string; when used with wprintf functions, specifies a single-byte?character string. Characters are printed up to the first null character or until the precision value is reached.

    //for ( int i = 0 ; i < sLen ; ++i )
    //{
    //    if ("%")
    //    {


    //    }

    //    printf

    //}

    //va_start(ap, Format);
    //pArg = va_arg(ap, void* );
    //fprintf(fp, Format, pArgString, );

    //return true;
//}

bool TKFile::Read(void *pReadBuffer, int BufferSize, int RepeatCount)
{
    if (BufferSize > 0)
    {
        memset(pReadBuffer, 0, BufferSize);
        if (fread(pReadBuffer, BufferSize, RepeatCount, fp) == 0)
        {
            OutputDebugString("Read Size Wrong\n");
            return false;
        }
    }
    return true;
}

bool TKFile::ReadLine(char *pString, const int BufferSize)
{
    if (fgets(pString, BufferSize, fp) == NULL)
    {
        IsEof = (feof(fp) != 0);
    }
    if (pString[strlen(pString)-1] == 0x0A) // 캐리지 리턴 문자를 초기화 한다.
    {
        pString[strlen(pString)-1] = 0;
    }

    return true;
}

bool TKFile::ReadScanf(const int ParamCount, ...)
{
    va_list lStrings;
    char *pArgString;

    va_start(lStrings, ParamCount);
    for (int i = 0 ; i < ParamCount ; ++i ) 
    {
        pArgString = va_arg(lStrings, char* );
        pArgString[0] = 0x00;
        
        fscanf(fp, "%s", pArgString);
        IsEof = (feof(fp) != 0);
    }
    va_end(lStrings);

    return true;
}

bool TKFile::Eof(void)
{
    return IsEof;
}

bool TKFile::Begin(void)
{
    fseek(fp, 0, SEEK_SET);
    return true;
}

bool TKFile::MoveToNextLine(void)
{
    if (fgets(ReadBuffer, FREAD_BUFFER_SIZE, fp) == NULL)
    {
        IsEof = (feof(fp) != 0);
    }

    return true;
}

