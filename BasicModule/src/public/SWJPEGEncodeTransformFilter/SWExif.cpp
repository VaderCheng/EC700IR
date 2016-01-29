#include "SWFC.h"
#include "SWExif.h"

static INT MotorolaOrder = 0;   // Ϊ��JPEG����һ�£�exifʹ�ô��ģʽ

static VOID Put16u(VOID * Short, WORD PutValue)
{
    if (MotorolaOrder){
        ((BYTE *)Short)[0] = (BYTE)(PutValue>>8);
        ((BYTE *)Short)[1] = (BYTE)PutValue;
    }else{
        ((BYTE *)Short)[0] = (BYTE)PutValue;
        ((BYTE *)Short)[1] = (BYTE)(PutValue>>8);
    }
}

//static INT Get16u(VOID * Short)
//{
//    if (MotorolaOrder){
//        return (((BYTE *)Short)[0] << 8) | ((BYTE *)Short)[1];
//    }else{
//        return (((BYTE *)Short)[1] << 8) | ((BYTE *)Short)[0];
//    }
//}

//static INT Get32s(VOID * Long)
//{
//    if (MotorolaOrder){
//        return  ((( CHAR *)Long)[0] << 24) | (((BYTE *)Long)[1] << 16)
//            | (((BYTE *)Long)[2] << 8 ) | (((BYTE *)Long)[3] << 0 );
//    }else{
//        return  ((( CHAR *)Long)[3] << 24) | (((BYTE *)Long)[2] << 16)
//            | (((BYTE *)Long)[1] << 8 ) | (((BYTE *)Long)[0] << 0 );
//    }
//}

//static unsigned Get32u(VOID * Long)
//{
//    return (unsigned)Get32s(Long) & 0xffffffff;
//}

static VOID Put32u(VOID * Value, DWORD PutValue)
{
    if (MotorolaOrder){
        ((BYTE *)Value)[0] = (BYTE)(PutValue>>24);
        ((BYTE *)Value)[1] = (BYTE)(PutValue>>16);
        ((BYTE *)Value)[2] = (BYTE)(PutValue>>8);
        ((BYTE *)Value)[3] = (BYTE)PutValue;
    }else{
        ((BYTE *)Value)[0] = (BYTE)PutValue;
        ((BYTE *)Value)[1] = (BYTE)(PutValue>>8);
        ((BYTE *)Value)[2] = (BYTE)(PutValue>>16);
        ((BYTE *)Value)[3] = (BYTE)(PutValue>>24);
    }
}

// buffer, Tag, Format, Components, PoINTer of Value
#define MakeIFDEntry(p, tag, type, count, value) \
                {                   \
                    Put16u(p, tag); \
                    Put16u(p+2, type); \
                    Put32u(p+4, count); \
                    Put32u(p+8, value); \
                }
/**
 * @brief  ����exif��Ϣ
 *
 * @param  pExifInfo    ��Ҫ��ӵ�exif��Ϣ�ṹ��ָ��
 *
 * @param  pExifBuffer���洢exif��Ϣ���������ɵ������ṩ
 *
 * @param  iExifLen     exif��Ϣʵ�ʳ���
 *
 * @return ��
 *
 * @note
 *         ������ʵ���ϴ������ǰ�����exif��Ϣ��APP1��(����APP1��־��exif����)��
 *         ֱ�ӽ��ö�д��JPEGͼƬ0xff 0xd8�󼴿ɡ�
 *
 *         �������ο�jheadʵ�֣����޸ģ������exif��׼�Ķ�����
 */
VOID CreateExif(HvExifInfo* pExifInfo, CHAR* pExifBuffer, INT& iExifLen)
{
    CHAR* p = pExifBuffer;

    WORD NumEntries;
    INT DataWriteIndex;
    INT DateIndex;
    INT DirIndex;
    INT DirContinuation;

    memcpy(p+4, "Exif\0\0II",8);   // ��Ϊ��ˣ���JPEG��ʽһ��
    Put16u(p+12, 0x2a);

    DataWriteIndex = 18;
    Put32u(p+14, DataWriteIndex - 10); // first IFD offset.  Means start 16 bytes in.

    /////////////////////////////////////////////////////////////// IFD0
    NumEntries = 4;         // IFD����Ŀ
    DirIndex = DataWriteIndex;
    DataWriteIndex += 2 + NumEntries*12 + 4;

    Put16u(p+DirIndex, NumEntries); // Number of entries
    DirIndex += 2;

    // IFD Enitries
    // camera Make
    MakeIFDEntry(p+DirIndex, TAG_MAKE, FMT_STRING, 32, DataWriteIndex - 10);
    DirIndex += 12;

    strncpy(p+DataWriteIndex, pExifInfo->szCameraMake, 32);
    DataWriteIndex += 32;

    // camera Model
    MakeIFDEntry(p+DirIndex, TAG_MODEL, FMT_STRING, 32, DataWriteIndex - 10);
    DirIndex += 12;

    strncpy(p+DataWriteIndex, pExifInfo->szCameraModel, 32);
    DataWriteIndex += 32;

    // Date/time entry
    MakeIFDEntry(p+DirIndex, TAG_DATETIME, FMT_STRING, 20, DataWriteIndex - 10);
    DirIndex += 12;

    DateIndex = DataWriteIndex;

    strncpy(p+DataWriteIndex, pExifInfo->szDateTime, 20);
    DataWriteIndex += 20;

    // Link to exif dir entry
    MakeIFDEntry(p+DirIndex, TAG_EXIF_OFFSET, FMT_ULONG, 1, DataWriteIndex - 10);
    DirIndex += 12;

    // End of directory - contains optional link to continued directory.
    DirContinuation = DirIndex;

    /////////////////////////////////////////////////////////////// Exif IFD

    NumEntries = 9;         // IFD����Ŀ
    DirIndex = DataWriteIndex;
    DataWriteIndex += 2 + NumEntries*12 + 4;

    Put16u(p+DirIndex, NumEntries); // Number of entries
    DirIndex += 2;

    //1.ExposureTime
    MakeIFDEntry(p+DirIndex, TAG_EXPOSURETIME, FMT_URATIONAL, 1, DataWriteIndex - 10);
    DirIndex += 12;

    Put32u(p+DataWriteIndex, pExifInfo->iExposureTime); // exposure time
    DataWriteIndex += 4;
    Put32u(p+DataWriteIndex, 1000000);  // us
    DataWriteIndex += 4;

    //gain
    MakeIFDEntry(p+DirIndex, TAG_GAIN_CONTROL, FMT_USHORT, 1, pExifInfo->iGain);
    DirIndex += 12;

    //2.contrast
    MakeIFDEntry(p+DirIndex, TAG_CONTRAST, FMT_USHORT, 1, pExifInfo->iContrast);
    DirIndex += 12;

    //3.saturation
    MakeIFDEntry(p+DirIndex, TAG_SATURATION, FMT_USHORT, 1, pExifInfo->iSaturation);
    DirIndex += 12;

    //4.sharpness
    MakeIFDEntry(p+DirIndex, TAG_SHARPNESS, FMT_USHORT, 1, pExifInfo->iSharpness);
    DirIndex += 12;

    MakeIFDEntry(p+DirIndex, TAG_WHITEBALANCE, FMT_USHORT, 1, !pExifInfo->iAutoWB);
    DirIndex += 12;

    //uniqe id
    CSWString strRGB;
    strRGB.Format("Gain:%d (r:%d,g:%d,b:%d)", 
		pExifInfo->iGain,pExifInfo->iContrast, pExifInfo->iSaturation, pExifInfo->iSharpness);
    MakeIFDEntry(p+DirIndex, TAG_IMAGE_UNIQUE_ID, FMT_STRING, strRGB.Length() + 1, DataWriteIndex - 10);
    DirIndex += 12;

    memcpy(p+DataWriteIndex, (LPCSTR)strRGB, strRGB.Length() + 1);
    DataWriteIndex += strRGB.Length() + 1;

    //5.Original date/time entry
    MakeIFDEntry(p+DirIndex, TAG_DATETIME_ORIGINAL, FMT_STRING, 20, DataWriteIndex - 10);
    DirIndex += 12;

    memcpy(p+DataWriteIndex, p+DateIndex, 20);
    DataWriteIndex += 20;
    
    //6.user comment
    MakeIFDEntry(p+DirIndex, TAG_USERCOMMENT, FMT_STRING, 32, DataWriteIndex - 10);
    DirIndex += 12;

    CHAR comment[32] = {0};
    sprintf(comment, "gain: %d filter: %d", pExifInfo->iGain, pExifInfo->iFilter);
    strncpy(p+DataWriteIndex, comment, 32);
    DataWriteIndex += 32;

    // End of directory - contains optional link to continued directory.
    Put32u(p+DirIndex, 0);

    //Continuation which links to this directory;
    Put32u(p+DirContinuation, DataWriteIndex - 10);

    /////////////////////////////////////////////////////////////// IFD1
    NumEntries = 2;     // IFD����Ŀ
    DirIndex = DataWriteIndex;
    DataWriteIndex += 2 + NumEntries*12 + 4;

    Put16u(p+DirIndex, NumEntries); // Number of entries
    DirIndex += 2;
    // thumbnail offset
    MakeIFDEntry(p+DirIndex, TAG_THUMBNAIL_OFFSET, FMT_ULONG, 1, DataWriteIndex - 10);
    DirIndex += 12;

    // thumbnail length
    MakeIFDEntry(p+DirIndex, TAG_THUMBNAIL_LENGTH, FMT_ULONG, 1, DataWriteIndex - 10);
    DirIndex += 12;

    // End of directory - contains optional link to continued directory.
    Put32u(p+DirIndex, 0);

    ///////////////////// APP1 Marker and exif length
    p[0] = (BYTE)0xFF;
    p[1] = (BYTE)0xE1;
    p[2] = (BYTE)((DataWriteIndex-2) >> 8);
    p[3] = (BYTE)(DataWriteIndex-2);

    iExifLen = DataWriteIndex;
}
