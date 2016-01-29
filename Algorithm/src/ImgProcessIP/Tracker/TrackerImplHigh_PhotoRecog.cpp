#include "TrackerImplHigh.h"


#define CROP_COUNT 1
#define PRETREATMENT_COUNT 4

extern int Dsp_DebugInfo(char* szfmt, ...);

const unsigned int knPlateStrLen = 7;// TODO: Magic Number

__inline int CheckPos(int nIdx, int* rgIdx, int nLen)
{
    int nPos = -1;
    for(int i = 0; i < nLen; i++)
    {
        if(nIdx == rgIdx[i])
        {
            nPos = i;
            break;
        }
    }

    return nPos;
}

HRESULT CTrackerImpl::CalcPlateColor(HV_COMPONENT_IMAGE imgFrame,PBYTE8 pBW, PLATE_TYPE nPlateType,PLATE_COLOR &nColor,int &nAvgY1, int &nAvgY2)
{
    int Y1=0,U1=0,V1=0,nCount1=0;
    int Y2=0,U2=0,V2=0,nCount2=0;
    //�޸ĸ߶Ⱥ������
    int nTop = imgFrame.iHeight/3;
    int nBottom = imgFrame.iHeight - nTop;
    int nLeft = imgFrame.iWidth/6;
    int nRight = imgFrame.iWidth - nLeft;

    RESTRICT_PBYTE8 pYLine = GetHvImageData(&imgFrame, 0) + nTop*imgFrame.iStrideWidth[0];
    RESTRICT_PBYTE8	pULine = GetHvImageData(&imgFrame, 1) + nTop*imgFrame.iStrideWidth[1];
    RESTRICT_PBYTE8 pVLine = GetHvImageData(&imgFrame, 2) + nTop*imgFrame.iStrideWidth[2];
    RESTRICT_PBYTE8 pBinLine = pBW+ nTop*imgFrame.iWidth;
    for(int i=nTop;i<=nBottom;i++,
        pYLine += imgFrame.iStrideWidth[0], 
        pULine += imgFrame.iStrideWidth[1],
        pVLine += imgFrame.iStrideWidth[2],
        pBinLine += imgFrame.iWidth)
    {
        for(int j=nLeft;j<=nRight;j++)
        {
            if(*(pBinLine + j) == 255)
            {
                Y1+= pYLine[j];
                U1+= pULine[j>>1];
                V1+= pVLine[j>>1];
                nCount1++;
            }
            else
            {
                Y2+= pYLine[j];
                U2+= pULine[j>>1];
                V2+= pVLine[j>>1];
                nCount2++;
            }
        }
    }
    if(nCount1== 0)
    {
        nCount1 = 1;
    }
    if(nCount2 == 0)
    {
        nCount2 = 1;
    }

    nAvgY1 = Y1/nCount1;  //�ַ�����
    nAvgY2 = Y2/nCount2;  //��������
    //�õ�YUV��Ϣ��������YUVת����HSLRGB
    BYTE8 RBG1[6],RBG2[6];
    if (nCount1==0) nCount1=1;
    if (nCount2==0) nCount2=1;
    YCbCr2BGR(Y1/nCount1,U1/nCount1,V1/nCount1,&RBG1[2],&RBG1[1],&RBG1[0]);
    YCbCr2BGR(Y2/nCount2,U2/nCount2,V2/nCount2,&RBG2[2],&RBG2[1],&RBG2[0]);
    BGR2HSL(RBG1[2],RBG1[1], RBG1[0], &RBG1[3], &RBG1[4], &RBG1[5]);
    BGR2HSL(RBG2[2],RBG2[1], RBG2[0], &RBG2[3], &RBG2[4], &RBG2[5]);
    int Diff1 = HV_ABS(RBG2[2]-RBG2[1]) + HV_ABS(RBG2[2]-RBG2[0]) + HV_ABS(RBG2[1]-RBG2[0]);
    int Diff2 = HV_ABS(RBG1[2]-RBG1[1]) + HV_ABS(RBG1[2]-RBG1[0]) + HV_ABS(RBG1[1]-RBG1[0]);
    //ȡ��������
    int FontIdx = 0;
    if(nPlateType == PLATE_UNKNOWN|| nPlateType == PLATE_POLICE)
    {
        if( RBG1[5] > RBG2[5] && RBG1[3]>100 && RBG1[3]<190 && RBG1[4]> 30)   //�����ϳ�PLATE_UNKNOWNʱ ����
        {
            FontIdx = 2;
        }
        else
        {
            FontIdx = 1;
        }
    }
    //PLATE_NORMAL�������ơ����ơ�����
    if(nPlateType == PLATE_NORMAL)
    {
        if(RBG2[3]>=100 && RBG2[3]<190 && RBG2[5]>30 && Diff2 > 40 &&    //�о���ʶ�������
            RBG1[5] > RBG2[5] && Diff1>Diff2)   //��������RGB��ֵ<��ɫ��ֵ
        {
            nColor = PC_BLUE;
        }
        else if(RBG2[3]>=60 && RBG2[3]<100 && Diff2>60)
        {
            nColor = PC_GREEN;
        }
        else if((RBG2[3]<10 ||RBG2[3]>230) && RBG2[4]>=80)
        {
            nColor = PC_UNKNOWN;  //��
        }
        else if(RBG2[3]>=15 && RBG2[3]<60 && RBG2[5]<200 && RBG2[5]>60&& Diff2>60)
        {
            nColor = PC_YELLOW;
        }
        else if(Diff2<60)
        {
            nColor = PC_BLACK;
        }
        else
        {
            nColor = PC_UNKNOWN;
        }
    }
    //PLATE_UNKNOWN�������ơ�ǳ���ơ����ơ����ƺ���
    if(nPlateType == PLATE_UNKNOWN|| nPlateType == PLATE_POLICE)
    {
        if(FontIdx == 1)
        {
            if(RBG2[3]>=15 && RBG2[3]<60 && (RBG2[5]<200 || RBG2[4] > 120) && Diff2>60)
            {
                nColor = PC_YELLOW;
            }
            else if(RBG2[3]>=100 && RBG2[3]<190 && Diff2 > 40 && (RBG2[5]< 200 || RBG2[4] > 150) &&
                (RBG1[5] < RBG2[5]) && Diff1<Diff2)
            {
                nColor = PC_LIGHTBLUE;
            }
            else if(RBG2[5]>120 && RBG1[5] < RBG2[5])
            {
                nColor = PC_WHITE;
            }
            else
            {
                nColor = PC_UNKNOWN;
            }
        }
        else
        {
            if(RBG1[3]>=100 && RBG1[3]<190 && RBG1[5]>30 && RBG1[4]>30 && Diff1>60)
            {
                nColor = PC_BLUE;
            }
            else
            {
                nColor = PC_UNKNOWN;
            }
        }
    }	
    //PLATE_DOUBLE_YELLOW�������ơ�����
    if(nPlateType == PLATE_DOUBLE_YELLOW)
    {
        if(RBG2[3]>=15 && RBG2[3]<60 && RBG2[5]<200 && Diff2>30)
        {
            nColor = PC_YELLOW;
        }
        else if(RBG2[5]>120 && RBG1[5] < RBG2[5])
        {
            nColor = PC_WHITE;
        }
        else
        {
            nColor = PC_UNKNOWN;
        }
    }
    if(nColor == 6)
    {
        nColor = PC_BLUE;
    }
    return  S_OK;
}

/*
void EnhanceColor(HV_COMPONENT_IMAGE* pImg)
{
    if(!pImg) return;

    RESTRICT_PBYTE8 pbCbBuf = GetHvImageData(pImg, 1);
    int iWidth = pImg->iWidth / 2;
    int iHeight = pImg->iHeight;
    int iStrideWidth = pImg->iStrideWidth[0]/2;

    for(int i = 0; i < iHeight; i++, pbCbBuf += iStrideWidth)
    {
        for(int j = 0; j < iWidth; j++)
        {
            pbCbBuf[j] = MAX_INT(MIN_INT((pbCbBuf[j] - 128) * 2 + 128, 255), 0);
        }
    }
}
*/

HRESULT CTrackerImpl::CropPlateImage(
    const HV_COMPONENT_IMAGE& imgPhoto,
    CRect& rcOriPos,
    PLATE_TYPE nPlateType,
    int nDetConf,
    HV_COMPONENT_IMAGE* rgPlateImage,
    int nCount	//ָ����ȡͼ�Ľ����
    )
{
    int nWidth(rcOriPos.Width());
    int nHeight(rcOriPos.Height());

    CRect rcFull(0,0,imgPhoto.iWidth,imgPhoto.iHeight);

    double nHRatio_Double = 80.0;
    double nHRation_Normal = 180.0;

    for( int i = 0; i < nCount; i++)
    {
        for(int j = 0; j < i; j++)
        {
            nHRatio_Double *= 1.1;
            nHRation_Normal *= 1.1;
        }

        CRect rcCrop = rcOriPos;

        if (( PLATE_DOUBLE_YELLOW == nPlateType )
            || (PLATE_DOUBLE_MOTO == nPlateType))
        {
            rcCrop.left -= int(nWidth * nHRatio_Double) >> 10;
            rcCrop.right += int(nWidth * nHRatio_Double) >> 10;
            rcCrop.top -= int(nHeight * 80) >> 10;
        }
        else
        {
            rcCrop.left -= int(nWidth * nHRation_Normal) >> 10;
            rcCrop.right += int(nWidth * nHRation_Normal) >> 10;
            rcCrop.top -= (nHeight * 80) >> 10;
            rcCrop.bottom += (nHeight * 80) >> 10;
        }
        float fltRatio = 5.2;
        nHeight = rcCrop.Height();
        nWidth = rcCrop.Width(); 
		if(nDetConf < 5)
		{
			fltRatio = 5.5;
		}
        int nWidthTemp = nHeight * fltRatio;
        if(nWidth < nWidthTemp)
        {
            nWidthTemp = (nWidthTemp - nWidth)>>1;
            rcCrop.left -= nWidthTemp;
            rcCrop.right += nWidthTemp;
        }

        nHeight = rcCrop.Height();
        if (nHeight >= MAX_SEG_IMG_HIGH)		//60Ϊ�ָ�ʱ����ͼ���߶�
        {
            rcCrop.top -= (nHeight - MAX_SEG_IMG_HIGH + 1) >> 1;
            rcCrop.bottom += nHeight - MAX_SEG_IMG_HIGH + 1 - ((nHeight - MAX_SEG_IMG_HIGH + 1) >> 1);
        }
        nWidth = rcCrop.Width();
        if (nWidth >= MAX_SEG_IMG_WIDTH)		//400Ϊ�ָ�ʱ����ͼ�����
        {
            rcCrop.left -= ((nWidth - MAX_SEG_IMG_WIDTH + 1) >> 1);
            rcCrop.right += (nWidth - MAX_SEG_IMG_WIDTH + 1 - ((nWidth - MAX_SEG_IMG_WIDTH + 1) >> 1));
        }

        rcCrop.IntersectRect(&rcFull, &rcCrop);
        CropImage(imgPhoto, rcCrop, rgPlateImage + i);
    }

    return S_OK;
}

HRESULT CTrackerImpl::PreTreatment( 
    HV_COMPONENT_IMAGE& imgPlate,
    PLATE_TYPE nPlateType,
    PLATE_COLOR nPlateColor,
    BOOL fEnhance,
    CPersistentComponentImage* rgImgRectified,
    int nCount,	 //ָ��Ԥ�������ɵ�ͼ����,ͨ���ı䴹ֱУ���Ƕ�
    CBinaryInfo &BinInfo
    )
{
    if(rgImgRectified == NULL || nCount < 1) return E_INVALIDARG;

    rgImgRectified[0].Assign(imgPlate);


    PLATE_COLOR nColorTemp = PC_UNKNOWN;
    int nPlateAvgY = 0;
    int nPlateAvgY2 = 0;
    CFastMemAlloc FastStack;
    BYTE8 *baTempImg;
    baTempImg = ( BYTE8 * )FastStack.StackAlloc( imgPlate.iWidth * imgPlate.iHeight + 50 );
    if(baTempImg != NULL)
    {
        memset(baTempImg,0,imgPlate.iWidth*imgPlate.iHeight);
        int iIsWhiteChar = ( nPlateColor == PC_BLACK ) || ( nPlateColor == PC_BLUE )|| (nPlateColor == PC_GREEN);
        IMAGE_Gray2BinNew( iIsWhiteChar, baTempImg,  imgPlate ,-1);		
        CalcPlateColor(imgPlate,baTempImg,nPlateType,nColorTemp,nPlateAvgY,nPlateAvgY2);
    }
    CPersistentComponentImage imgEnhance;
    if(fEnhance)
    {
        DWORD32 nMinY(0),nMaxY(255);
        imgEnhance.Assign(imgPlate);
        if(nPlateAvgY < nPlateAvgY2)  //������
        {
            nMinY = MAX_INT(nPlateAvgY - 30,0);
            nMaxY = MIN_INT(nPlateAvgY2 + 50,250);
            PlateEnhance_IR2(&imgEnhance,nMinY,nMaxY);
        }
        else  //������
        {
            nMinY = MAX_INT(nPlateAvgY2 - 50,0);
            nMaxY = MIN_INT(nPlateAvgY + 30,250);
            PlateEnhance_IR2(&imgEnhance,nMinY,nMaxY);
        }

        //PlateEnhance_IR(&imgEnhance);
        imgPlate = (HV_COMPONENT_IMAGE)imgEnhance;
        EnhanceColor(&imgPlate);
    }

    CPersistentComponentImage imgDownSample;
    if(	imgPlate.iWidth > MAX_SEG_IMG_WIDTH ||
        imgPlate.iHeight > MAX_SEG_IMG_HIGH )
    {
        ImageDownSample(imgDownSample, imgPlate);
        imgPlate = (HV_COMPONENT_IMAGE)imgDownSample;
    }

    CPersistentComponentImage imgUpSample;
    if(imgPlate.iHeight < 18 && imgPlate.iHeight > 0 )
    {
        imgUpSample.Create(HV_IMAGE_YUV_422, imgPlate.iWidth * 30 / imgPlate.iHeight, 30);
        PlateResize(imgPlate, imgUpSample);
        imgPlate = imgUpSample;
    }

    CRectifyInfo RectifyInfo;

    CPersistentComponentImage imgRectified;
    imgRectified.Assign(imgPlate);
    RTN_HR_IF_FAILED(m_pProcessPlate->CalcRectifyInfo(
        &imgPlate,
        &imgRectified, 
        &RectifyInfo, 
        &BinInfo, 
        &nPlateType, 
        &nPlateColor
        ));

    if(/*m_fEnableRectify*/1)
    {
        //RectifyInfo.fltVAngle += RectifyInfo.fltHAngle;

        if(RectifyInfo.fltVAngle > 100 ||
            RectifyInfo.fltVAngle < 80)
        {
            RectifyInfo.fltVAngle = (RectifyInfo.fltVAngle - 90)/2 + 90;
        }

        float fltMaxVAngle = (RectifyInfo.fltVAngle - 90);

        if(fltMaxVAngle < 0) 
        {
            fltMaxVAngle = fltMaxVAngle>-5?-5:fltMaxVAngle;
        }
        else
        {
            fltMaxVAngle = fltMaxVAngle<5?5:fltMaxVAngle;
        }


        CRectifyInfo cReRectifyInfo = RectifyInfo;
        for(int i = 0; i < nCount; i++)
        {
            if(fltMaxVAngle < 0)
            {
                cReRectifyInfo.fltVAngle = fltMaxVAngle + 5*i/nCount + 90;
            }
            else
            {
                cReRectifyInfo.fltVAngle = fltMaxVAngle - 5*i/nCount + 90;
            }
            cReRectifyInfo.fltHAngle = RectifyInfo.fltHAngle * (nCount - i) / nCount;

            rgImgRectified[i].Assign(imgPlate);
            RTN_HR_IF_FAILED(m_pProcessPlate->RectifyPlate(
                &imgPlate, &cReRectifyInfo, &rgImgRectified[i], false ));

            //{
            //	static int nCount = 0;
            //	wchar_t szPath[MAX_PATH]={0};
            //	swprintf(szPath, L"d:\\CropRectify\\%08d.bmp",nCount++);
            //	SaveImage(szPath, &rgImgRectified[i]);
            //}
        }
    }
    else
    {
        for(int i = 0; i < nCount; i++)
        {
            rgImgRectified[i].Assign(imgPlate);
        }	
    }

    return S_OK;
}

HRESULT CTrackerImpl::Vote(
    PlateInfo** rgInfo,
    int nCount,
    PlateInfo* pResult,
    CParamQuanNum* pfltConf
    )
{
    if(  !rgInfo || !pResult || nCount < 1 || !pfltConf) return E_INVALIDARG;

    static int rgIdx[knPlateStrLen][g_cnPlateChars] = {0}; //��λ�ַ������
    static int rgIdxCount[knPlateStrLen][g_cnPlateChars] = {0}; //�ַ��������
    static CParamQuanNum rgIdxConf[knPlateStrLen][g_cnPlateChars] = {0}; //��λ�ַ�������ŶȺ�
    static int rgResultCount[knPlateStrLen] = {0}; //��λ�ַ�����������
    static CParamQuanNum rgColor[PC_COUNT] = {0}; //��ɫ���Ŷ�ͳ��
    static CParamQuanNum rgType[PLATE_TYPE_COUNT] = {0}; //�������Ϳ��Ŷ�ͳ��

    HV_memset(rgIdx, 0, sizeof(rgIdx));
    HV_memset(rgIdxCount, 0, sizeof(rgIdxCount));
    HV_memset(rgIdxConf, 0, sizeof(rgIdxConf));
    HV_memset(rgResultCount, 0, sizeof(rgResultCount));
    HV_memset(rgColor, 0, sizeof(rgColor));
    HV_memset(rgType, 0, sizeof(rgType));

    *pResult = *rgInfo[0];
    *pfltConf = 0.0;

    int nWJCount = 0;

    //ͳ����ɫ�����͵Ŀ��Ŷ�
    float fltMaxConf = -1;
    int nMaxConfID = 0; //��¼���Ŷ���ߵĽ��
    PlateInfo* pCurInfo = NULL;
    for(int i = 0; i < nCount; i++)
    {
        pCurInfo = rgInfo[i];
        rgColor[pCurInfo->color] += pCurInfo->fltTotalConfidence;
        rgType[pCurInfo->nPlateType] += pCurInfo->fltTotalConfidence;

        if(pCurInfo->nPlateType == PLATE_WJ)
        {
            nWJCount++;
        }

        if(fltMaxConf < pCurInfo->fltTotalConfidence.ToFloat())
        {
            fltMaxConf = pCurInfo->fltTotalConfidence.ToFloat();
            nMaxConfID = i;
        }
    }

    //��ɫͶƱ
    PLATE_COLOR nColor = rgInfo[0]->color;
    CParamQuanNum fltConf = 0.0;
    for(int i = 0; i < PC_COUNT; i++)
    {
        if(fltConf < rgColor[i])
        {
            fltConf = rgColor[i];
            nColor = (PLATE_COLOR)i;
        }
    }

    //��������ͶƱ
    PLATE_TYPE nType = rgInfo[0]->nPlateType;
    if(nWJCount >= 2)
    {
        nType = PLATE_WJ;
    }
    else
    {	
        fltConf = 0.0;
        for(int i = 0; i < PLATE_TYPE_COUNT; i++)
        {
            if(fltConf < rgType[i])
            {
                fltConf = rgType[i];
                nType = (PLATE_TYPE)i;
            }
        }
    }

    //�ַ�ͶƱ
    int nPos;
    for(int i = 0; i < nCount; i++)
    {
        if(rgInfo[i]->nPlateType != nType) continue;

        pCurInfo = rgInfo[i];

        for(int  j = 0; j < knPlateStrLen; j++) //ͳ�Ƹ�λ���
        {
            nPos = CheckPos(pCurInfo->rgbContent[j], rgIdx[j], rgResultCount[j]);
            if(nPos == -1)
            {
                nPos = rgResultCount[j];
                rgIdx[j][nPos] = pCurInfo->rgbContent[j];
                //				rgIdxCount[j][nPos] = 1; //�������������Ϊ0
                //				rgIdxConf[j][nPos] = pCurInfo->rgfltConfidence[j];
                rgResultCount[j]++;
            }

            rgIdxConf[j][nPos] += pCurInfo->rgfltConfidence[j];
            rgIdxCount[j][nPos]++;

            if(i == nMaxConfID) //���ܿ��Ŷ���ߵĽ������Ȩ��
            {
                rgIdxConf[j][nPos] += pCurInfo->rgfltConfidence[j];
                rgIdxCount[j][nPos]++;
            }
        }
    }

    int rgContent[knPlateStrLen] = {0};
    CParamQuanNum rgConf[knPlateStrLen] = {0};
    CParamQuanNum nMaxConf;

    //��λ���⴦��,ͶƱ�Խ����������
    //rgContent[0] = rgIdx[0][0];
    //rgConf[0] = rgIdxConf[0][0];
    //int nMaxIdxCount = rgIdxCount[0][0];
    //for(int i = 1; i < rgResultCount[0]; i++)
    //{
    //	if(nMaxIdxCount > rgIdxCount[0][i]) continue;

    //	//�������һ���򿴿��Ŷ�
    //	if(nMaxIdxCount == rgIdxCount[0][i] &&
    //	   rgConf[0] > rgIdxConf[0][i])
    //	{
    //		continue;
    //	}
    //	
    //	nMaxIdxCount = rgIdxCount[0][i];
    //	rgContent[0] = rgIdx[0][i];
    //	rgConf[0] = rgIdxConf[0][i];
    //}

    for(int i = 0; i < knPlateStrLen; i++)
    {
        rgContent[i] = rgIdx[i][0];
        rgConf[i] = rgIdxConf[i][0].ToFloat()/rgIdxCount[i][0];
        nMaxConf = rgIdxConf[i][0];
        for(int j = 1; j < rgResultCount[i];j++)
        {
            if(nMaxConf < rgIdxConf[i][j])
            {
                nMaxConf = rgIdxConf[i][j];
                rgContent[i] = rgIdx[i][j];
                rgConf[i] = rgIdxConf[i][j].ToFloat()/rgIdxCount[i][j];
            }
        }
    }
    ////////////////////////////////////////////  ���Ŷ��޸�begin
    int nMaxIdx = 0;
    for (int i = 0; i < rgResultCount[0]; ++i)
    {
        if(rgContent[0] == rgIdx[0][i])
        {
            nMaxIdx = rgIdxCount[0][i];		// ���������ŶȺ��ֵĸ���
            break;
        }
    }
    // ������ͬ���ֵĸ��������ۿ��Ŷ�
    rgConf[0] -= rgConf[0].ToFloat() * (nCount + 1 - nMaxIdx) / 10.0f;		//  (1 - n/5) * 1/2
    ///////////////////////////////////////////////  end


    //����ͶƱ���
    for(int i = 0; i < knPlateStrLen; i++)
    {
        pResult->rgbContent[i] = rgContent[i];
        pResult->rgfltConfidence[i] = rgConf[i];
    }

    //������Ŷ�
    pResult->fltTotalConfidence = 1.0;
    for(int i = 1; i < knPlateStrLen; i++)
    {
        pResult->fltTotalConfidence *= pResult->rgfltConfidence[i];
    }

    pResult->color = nColor;
    pResult->nPlateType = nType;

    *pfltConf = pResult->fltTotalConfidence;

    return S_OK;
}

HRESULT CTrackerImpl::RecogSnapImg(
	PROCESS_ONE_FRAME_PARAM* pParam,
	PROCESS_ONE_FRAME_DATA* pProcessData,
	PROCESS_ONE_FRAME_RESPOND* pProcessRespond,
	HV_COMPONENT_IMAGE& imgFrame,
	CRect& rcRecogArae,
	int nDetMinScale,
	int nDetMaxScale
	)
{
	DWORD32 dwBegin = GetSystemTick();

	CRect rcCrop;
	rcCrop.left = (long)(imgFrame.iWidth * rcRecogArae.left/100.0);
	rcCrop.top = (long)(imgFrame.iHeight * rcRecogArae.top/100.0);
	rcCrop.right = (long)(imgFrame.iWidth * rcRecogArae.right/100.0);
	rcCrop.bottom = (long)(imgFrame.iHeight * rcRecogArae.bottom/100.0);

	//TODO: ��ȡ�������
	HV_COMPONENT_IMAGE imgCrop;
	CropImage(imgFrame, rcCrop, &imgCrop);

	MULTI_ROI MultiROI=
	{
		ARRSIZE(m_rgTempPlatePos),
			m_rgTempPlatePos
	};

	CItgArea* pItgArea = m_cItgArea.IsCalcItg()?&m_cItgArea:NULL;

	int iAvgY = 0;
	if( S_OK != m_pPlateDetector->ScanPlatesComponentImage(
		ALL_DETECTOR_PHOTO,
		&imgCrop,
		&MultiROI,
		nDetMinScale,
		nDetMaxScale,
		TRUE,
		0,
		FALSE,
		&iAvgY,//m_iAvgY,
		pItgArea
		))
	{
		return E_FAIL;
	}

	//�������λ��Ϊ��������
	for(int j = 0; j < MultiROI.dwNumROI; j++)
	{
		RECTA* pROI = MultiROI.rcROI + j;
		pROI->left += rcCrop.left;
		pROI->top += rcCrop.top;
		pROI->right += rcCrop.left;
		pROI->bottom += rcCrop.top;
	}

	pProcessRespond->iFrameAvgY = iAvgY;//m_iAvgY;
	int nInfoCount = 0;
	PlateInfo* aInfo[10] ={0};
	CRect rcMax(0,0,imgFrame.iWidth,imgFrame.iHeight);
	for(int i = 0; i < MIN_INT(MultiROI.dwNumROI,10); i++)
	{
		RECTA *pROI = MultiROI.rcROI + i;
		CRect rcCurPlate(*pROI);
		if (!rcCurPlate.IntersectRect(&rcCurPlate, &rcMax)) continue;

		int nPlateVar = pROI->nVariance;

		HV_COMPONENT_IMAGE rgCrop[CROP_COUNT];
		CPersistentComponentImage rgTempPlate[CROP_COUNT];
		CPersistentComponentImage rgPlate[CROP_COUNT * PRETREATMENT_COUNT];

		//ȷ��ͶƱ��
		int nCropCount = CROP_COUNT;
		int nPlateCount = PRETREATMENT_COUNT;
		int nAllCount = nCropCount * nPlateCount;

		CropPlateImage(imgFrame, rcCurPlate, pROI->nPlateType,pROI->nConf, rgCrop, nCropCount);

		BOOL fEnhance = FALSE;
		CBinaryInfo BinaryInfo;

		for(int j = 0; j < nCropCount; j++)
		{
			// ��ʽת��
			if (rgCrop[j].nImgType != HV_IMAGE_YUV_422 && rgCrop[j].nImgType != HV_IMAGE_BT1120 && rgCrop[j].nImgType != HV_IMAGE_BT1120_FIELD)
			{
				rgTempPlate[j].Create(HV_IMAGE_YUV_422, rgCrop[j].iWidth, rgCrop[j].iHeight);
				rgTempPlate[j].Convert(rgCrop[j]);
				rgCrop[j] = (HV_COMPONENT_IMAGE)rgTempPlate[j];
			}
			PreTreatment(
				rgCrop[j],
				pROI->nPlateType,
				pROI->color,
				fEnhance,
				rgPlate + j * nPlateCount,
				nPlateCount,
				BinaryInfo
				);
		}

		GetInflateRect(
			rcCurPlate,			// in, out.
			pROI->color,
			pROI->nPlateType,
			//pROI->nConf,
			200		//���ڴ˿�ȵ����������߷�Χ
			);
		rcCurPlate.IntersectRect(&rcMax,&rcCurPlate);

		PlateInfo cInfo;
		cInfo.rcPos = rcCurPlate;
		cInfo.color = pROI->color;
		cInfo.nPlateType = pROI->nPlateType;
		cInfo.iLastDetWidth = pROI->right-pROI->left;
		cInfo.nVariance = nPlateVar;


		int nPosX = rcCurPlate.CenterPoint().x;
		int nPosY = rcCurPlate.CenterPoint().y;

		int nBottomLine = imgFrame.iHeight * 2 / 3;

		// �޸�5�ֿ��
		int iBottomWidthReal = m_iBottomWidth * imgFrame.iWidth / 1800;	// ��ͼ����Ϊ�ο� 200w 1600 300w 2000
		int iMiddleWidthReal = m_iMiddleWidth * imgFrame.iWidth / 1800;
		int nFiveCharWidth = (nPosY > nBottomLine)?iBottomWidthReal:iMiddleWidthReal;

		PlateInfo* aInfo_1[128] = {0};
		PlateInfo* aInfo_2[128] = {0};
		int nInfoCount_1 = 0;
		int nInfoCount_2 = 0;

		CPersistentComponentImage imgRectifyPlate;
		for(int j = 0; j < nAllCount; j++)
		{
			PlateInfo* pInfo = new PlateInfo;
			if(pInfo == NULL) continue;

			*pInfo = cInfo;
            Dsp_DebugInfo("Rec %d(%d) %d(%d)\n", i, MIN_INT(MultiROI.dwNumROI,10), j, nAllCount);
			if(S_OK != RecognizePlate(rgPlate[j], *pInfo, imgRectifyPlate))
			{
				SAFE_DELETE(pInfo);
				continue;
			}
			aInfo_1[nInfoCount_1++] = pInfo;
		}

		//ͶƱ
		PlateInfo* pResult_1 = new PlateInfo;
		CParamQuanNum fltConf1 = 0.0;
		if(S_OK != Vote(aInfo_1, nInfoCount_1,pResult_1,&fltConf1))
		{
			SAFE_DELETE(pResult_1);
		}

		//�Ƿ�ı��ַ���ɫ��ʶ��
		BOOL fReRecog = TRUE;
		if( pResult_1 != NULL )
		{
			if( pROI->nPlateType == PLATE_DOUBLE_YELLOW ||
				pResult_1->nPlateType == PLATE_WJ )	//˫���ƺ�WJ�Ʋ���ʶ��
			{
				fReRecog = FALSE;
			}
			else if( pROI->nPlateType == PLATE_NORMAL && //���Ŷȸߵ����Ʋ���ʶ��
				 pResult_1->fltTotalConfidence > 0.25f)
			{
				fReRecog = FALSE;
			}
			else if(pResult_1->fltTotalConfidence > 0.35f)
			{
				fReRecog = FALSE;
			}

			//SaveDebugText("C:\\Conf.log","Conf=%8.3f",pResult_1->fltTotalConfidence.ToFloat());
		}

		if( fReRecog )
		{
			if(pROI->nPlateType == PLATE_UNKNOWN)
			{
				cInfo.rcPos = rcCurPlate;
				cInfo.color = PC_BLUE;
				cInfo.nPlateType = PLATE_NORMAL;
				cInfo.iLastDetWidth = pROI->right-pROI->left;
			}
			else if(pROI->nPlateType == PLATE_NORMAL)
			{
				cInfo.rcPos = rcCurPlate;
				cInfo.color = PC_YELLOW;
				cInfo.nPlateType = PLATE_UNKNOWN;
				cInfo.iLastDetWidth = pROI->right-pROI->left;
			}

			for(int j = 0; j < nAllCount; j++)
			{
				PlateInfo* pInfo = new PlateInfo;
				if(pInfo == NULL) continue;

				*pInfo = cInfo;
                Dsp_DebugInfo("Rec2 %d(%d) %d(%d)\n", i, MIN_INT(MultiROI.dwNumROI,10), j, nAllCount);
				if(S_OK != RecognizePlate(rgPlate[j],  *pInfo, imgRectifyPlate))
				{
					SAFE_DELETE(pInfo);
					continue;
				}
				aInfo_2[nInfoCount_2++] = pInfo;
			}
		}

		PlateInfo* pResult_2 = new PlateInfo;
		CParamQuanNum fltConf2 = 0.0;
		if(pResult_2)
		{
			if(S_OK != Vote(aInfo_2, nInfoCount_2,pResult_2,&fltConf2))
			{
				SAFE_DELETE(pResult_2);
			}
		}

		PlateInfo* pResult = pResult_1;
		if(pResult_2)
		{
			if(	pResult_1 == NULL )
			{
				pResult = pResult_2;
			}
			else if(pResult_2->fltTotalConfidence.ToFloat() > (1.5 * pResult_1->fltTotalConfidence.ToFloat()))
			{
				pResult = pResult_2;
			}

			if(pResult == pResult_2)
			{
				SAFE_DELETE(pResult_1);
			}
			else
			{
				SAFE_DELETE(pResult_2);
			}
		}

		if(pResult)
		{
			pResult->fValid = TRUE;
			aInfo[nInfoCount++] = pResult;
		}

		//����
		for(int j = 0; j < nInfoCount_1; j++)
		{
			SAFE_DELETE(aInfo_1[j]);
		}

		for(int j = 0; j < nInfoCount_2; j++)
		{
			SAFE_DELETE(aInfo_2[j]);
		}
	}

	//��ͬһ��λ�õĶ��trackֻ����һ��
	for (int i = 0; i < nInfoCount - 1; i++)
	{
		for (int j = i + 1; j < nInfoCount; j++)
		{
			CRect rect1(aInfo[i]->rcPos);
			CRect rect2(aInfo[j]->rcPos);
			rect1.InflateRect(
				(rect1.Width() * (m_pParam->g_fltTrackInflateX).ToInt()),
				(rect1.Height() * (m_pParam->g_fltTrackInflateY).ToInt())
				);
			rect2.InflateRect(
				(rect2.Width() * (m_pParam->g_fltTrackInflateX).ToInt()),
				(rect2.Height() * (m_pParam->g_fltTrackInflateY).ToInt())
				);
			if (rect1.IntersectsWith(rect2))
			{
				if (aInfo[i]->fltTotalConfidence > aInfo[j]->fltTotalConfidence)
				{
					aInfo[j]->fValid = false;
				}
				else
				{
					aInfo[i]->fValid = false;
				}
			}
		}
	}

	// ���տ��Ŷ�����
	PlateInfo* tmpInfo;
	for (int i = 0; i < nInfoCount - 1; i++)
	{
		for (int j = i + 1; j < nInfoCount; j++)
		{
			BOOL fChange = FALSE;
			if(/*m_nFirstPlatePos*/ 0 == 1)
			{
				CRect rcCur = aInfo[i]->rcPos;
				CRect rcDest = aInfo[j]->rcPos;
				if(rcCur.CenterPoint().y < rcDest.CenterPoint().y)
				{
					fChange = TRUE;
				}
			}
			else if(aInfo[i]->fltTotalConfidence < aInfo[j]->fltTotalConfidence)
			{
				fChange = TRUE;
			}

			if (fChange)
			{
				tmpInfo = aInfo[i];
				aInfo[i] = aInfo[j];
				aInfo[j] = tmpInfo;
			}
		}
	}

	//int nOutPutCount = 0;
	for (int i = 0; i < nInfoCount; i++)
	{
		if(aInfo[i]->fValid != TRUE) continue;

		for ( int j = 0; j < 7; j++)
		{
			if(aInfo[i]->rgbContent[j] == 0)
			{
				aInfo[i]->rgbContent[j] = 1;
			}
		}

		//---------------------------------------------------------------------------------------------
		// ����������֤
		// ��Ը������͵ĳ��ƽ��й����飬����ԤͶƱ���
		//---------------------------------------------------------------------------------------------
		switch(aInfo[i]->nPlateType)
		{
		case PLATE_NORMAL:				// ���ƻ����
			if ((aInfo[i]->rgbContent[0] == 57) && (aInfo[i]->rgbContent[1] == 36))
			{
				// ǰ��λΪ����Z��
				if ((aInfo[i]->rgbContent[6] != 114) && (aInfo[i]->rgbContent[6] != 115))		// �۰���β
				{
					aInfo[i]->rgbContent[6] = 115;
				}
				aInfo[i]->color = PC_BLACK;			// ǿ����Ϊ��ɫ
			}
			break;
		case PLATE_POLICE:				// �����ƻ��������
			if (IsMilitaryAlpha(aInfo[i]->rgbContent[0]) || IsMilitaryChn(aInfo[i]->rgbContent[0]) || ((aInfo[i]->rgbContent[0] == 38) && (aInfo[i]->rgbContent[1] == 32)))
			{
				aInfo[i]->color = PC_WHITE;			// ǿ����Ϊ��ɫ
				break;
			}

			// (��ͷ���Ѿ�û�о����ˣ�ֻ���ǻ��ƺ͹�������)
			if (aInfo[i]->rgbContent[6] == 37)		// ���һλΪ������
			{
				if (aInfo[i]->rgbContent[0] != 58)	// ����һλ�ǡ���
				{
					// ��׼���ƻ�ǳ����
					if (aInfo[i]->color == PC_WHITE)
					{
						aInfo[i]->color = PC_YELLOW;			// ǿ����Ϊ��ɫ,Ҳ������ǳ����
					}
				}
				else
				{
					// ��������
					aInfo[i]->color = PC_WHITE;			// ǿ����Ϊ��ɫ
					aInfo[i]->nPlateType = PLATE_POLICE2;
				}
			}
			else
			{
				// ��׼���ƻ�ǳ����
				if (aInfo[i]->color == PC_WHITE)
				{
					aInfo[i]->color = PC_YELLOW;			// ǿ����Ϊ��ɫ,Ҳ������ǳ����
				}
			}
			break;
		case PLATE_POLICE2:							// ��׼����
			aInfo[i]->rgbContent[6] = 37;					// ǿ����Ϊ����
			aInfo[i]->color = PC_WHITE;					// ǿ����Ϊ��ɫ
			break;
		case PLATE_DOUBLE_YELLOW:					// ˫����
			if (IsMilitaryAlpha(aInfo[i]->rgbContent[0]) || IsMilitaryChn(aInfo[i]->rgbContent[0]) || ((aInfo[i]->rgbContent[0] == 38) && (aInfo[i]->rgbContent[1] == 32)))
			{
				aInfo[i]->color = PC_WHITE;				// ǿ����Ϊ��ɫ
			}
			else
			{
				// ˫����
				aInfo[i]->color = PC_YELLOW;			// ǿ����Ϊ��ɫ
			}
			break;
		default:
			break;
		}

                PlateInfo& cInfo = *aInfo[i];

		// ʶ������ɫ ���ϲŽ��г�����ɫʶ����滻
		cInfo.nCarColour = CC_UNKNOWN;
		RecognizeCarColour(imgCrop, cInfo, iAvgY);

		PROCESS_IMAGE_CORE_RESULT* pCurResult = &(pProcessRespond->cTrigEvent.rgCarLeftCoreInfo[pProcessRespond->cTrigEvent.iCarLeftCount]);
		//�س���Сͼ, �㳵������
		HiVideo::CRect& rect = cInfo.rcPos;
		HV_COMPONENT_IMAGE imgBestPlate;
		CropImage(imgFrame, rect, &imgBestPlate);
		CPersistentComponentImage imgYUVPlate;
		if (imgBestPlate.nImgType != HV_IMAGE_YUV_422)
		{
			imgYUVPlate.Create(HV_IMAGE_YUV_422, imgBestPlate.iWidth, imgBestPlate.iHeight);
			imgYUVPlate.Convert(imgBestPlate);
			imgBestPlate = imgYUVPlate;
		}

		for (int i = 0; i < 3; i++)
		{
			PBYTE8 pSrc = GetHvImageData(&imgBestPlate, i);
			PBYTE8 pDes = GetHvImageData(&pProcessData->rghvImageSmall[pProcessRespond->cTrigEvent.iCarLeftCount], i);
			if (pSrc && pDes)
			{
				memcpy(pDes, pSrc, imgBestPlate.iStrideWidth[i] * imgBestPlate.iHeight);
			}
		}

		pCurResult->nPlateWidth = imgBestPlate.iWidth;
		pCurResult->nPlateHeight = imgBestPlate.iHeight;

		BYTE8 rgbResizeImg[BINARY_IMAGE_WIDTH * BINARY_IMAGE_HEIGHT];
		//HiVideo::CRect *prcPlatePos;
		//prcPlatePos = &rcCurPlate;

		int iWidth = BINARY_IMAGE_WIDTH;
		int iHeight = BINARY_IMAGE_HEIGHT;
		int iStride = BINARY_IMAGE_WIDTH;

		// ֻ������Ĳ��ֶ�ֵ��ͼ
		ScaleGrey(rgbResizeImg, iWidth, iHeight, iStride,
			GetHvImageData(&imgBestPlate, 0),
			imgBestPlate.iWidth,
			imgBestPlate.iHeight,
			imgBestPlate.iStrideWidth[0]
			);

		// �����ֵ����ֵ
		HiVideo::CRect rcArea(0, 0, iWidth, iHeight);
		int iThre = CalcBinaryThreByCenter(
			rgbResizeImg,
			iWidth,
			iHeight,
			iStride,
			&rcArea);

		// ���ݳ�����ɫ���ö�ֵ������
		BOOL fIsWhiteChar = FALSE;
		if ((cInfo.color == PC_BLUE) ||
			(cInfo.color == PC_BLACK)	||
			(cInfo.color == PC_GREEN))
		{
			fIsWhiteChar = TRUE;
		}
		GrayToBinByThre(
			fIsWhiteChar,
			rgbResizeImg,
			iWidth,
			iHeight,
			iStride,
			iThre,
			GetHvImageData(&pProcessData->rghvImageBin[pProcessRespond->cTrigEvent.iCarLeftCount], 0)
			);

		memcpy(pCurResult->rgbContent, cInfo.rgbContent, 8);
		pCurResult->nType = cInfo.nPlateType;
		pCurResult->iAvgY = iAvgY;//m_iAvgY;
		pCurResult->iCarAvgY = cInfo.nAvgY;
		pCurResult->nColor = cInfo.color;
		pCurResult->nCarColor = cInfo.nCarColour;
		pCurResult->rcBestPlatePos = cInfo.rcPos;
		pCurResult->fltAverageConfidence = cInfo.fltTotalConfidence.ToFloat();
		pCurResult->fltFirstAverageConfidence = cInfo.rgfltConfidence[0].ToFloat();
		pCurResult->nRoadNo = MatchRoad(cInfo.rcPos.CenterPoint().x, cInfo.rcPos.CenterPoint().y);
		pCurResult->nFirstFrameTime = pParam->dwImageTime;
		pCurResult->nCarArriveTime  = pParam->dwImageTime;
		pProcessRespond->cTrigEvent.dwEventId |= EVENT_CARLEFT;
		pProcessRespond->cTrigEvent.iCarLeftCount++;
	}

	pProcessRespond->cTrigEvent.dwEventId |= EVENT_FRAME_RECOED;

	return S_OK;
}
