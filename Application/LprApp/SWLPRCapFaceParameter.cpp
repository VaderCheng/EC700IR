#include "SWFC.h"
#include "SWLPRCapFaceParameter.h"

CSWLPRCapFaceParameter::CSWLPRCapFaceParameter()
{
	Get().nWorkModeIndex = PRM_CAP_FACE;
	m_strWorkMode.Format("����");


	//�ȳ�ʼ��ɨ������ÿ��ģʽ�����Լ���ɨ������
	Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft = 0;
	Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop = 15;
	Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight = 100;
	Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom = 55;
	Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum = 5;
	Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum = 9;

	//��ʼ����������
	Get().cTrackerCfgParam.cTrapArea.TopLeftX = Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft;
	Get().cTrackerCfgParam.cTrapArea.TopRightX = Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight;
	Get().cTrackerCfgParam.cTrapArea.BottomLeftX = Get().cTrackerCfgParam.cDetectArea.DetectorAreaLeft;
	Get().cTrackerCfgParam.cTrapArea.BottomRightX = Get().cTrackerCfgParam.cDetectArea.DetectorAreaRight;

	Get().cTrackerCfgParam.cTrapArea.TopLeftY = Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop;
	Get().cTrackerCfgParam.cTrapArea.TopRightY = Get().cTrackerCfgParam.cDetectArea.DetectorAreaTop;
	Get().cTrackerCfgParam.cTrapArea.BottomLeftY = Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom;
	Get().cTrackerCfgParam.cTrapArea.BottomRightY = Get().cTrackerCfgParam.cDetectArea.DetectorAreaBottom;

	Get().cTrackerCfgParam.cTrapArea.nDetectorMinScaleNum = Get().cTrackerCfgParam.cDetectArea.nDetectorMinScaleNum;
	Get().cTrackerCfgParam.cTrapArea.nDetectorMaxScaleNum = Get().cTrackerCfgParam.cDetectArea.nDetectorMaxScaleNum;

	Get().cCamCfgParam.iMaxExposureTime = 4200;
	Get().cCamCfgParam.iMinExposureTime = 0;
	
	//����Ĭ�ϵ���Ƶ��������ϵ��
	Get().cTrackerCfgParam.cScaleSpeed.fltAdjustCoef = 0.96;
}

CSWLPRCapFaceParameter::~CSWLPRCapFaceParameter()
{
}

HRESULT CSWLPRCapFaceParameter::InitTracker(VOID)
{
	if(S_OK == CSWLPRParameter::InitTracker())
	{
    GetEnum("\\Tracker\\VideoDet"
    	, "ShadowDetMode"
    	, &Get().cTrackerCfgParam.cVideoDet.nShadowDetMode
    	, Get().cTrackerCfgParam.cVideoDet.nShadowDetMode
    	, "0:��ʹ����Ӱ���;1:ʹ����Ӱ���"
    	, "��Ӱ���ģʽ"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\Tracker\\VideoDet"
    	, "ShadowDetEffect"
    	, &Get().cTrackerCfgParam.cVideoDet.nShadowDetEffect
    	, Get().cTrackerCfgParam.cVideoDet.nShadowDetEffect
    	, 0
    	, 100
    	, "��Ӱ���ǿ���̶�ֵ"
    	, "0:ʹ��ȱʡֵ(����ȷ), 1:��Ӱ���ǿ���̶���С, 100:��Ӱ���ǿ���̶����"
    	, CUSTOM_LEVEL
    );
    
    GetEnum("\\Tracker\\VideoDet"
    	, "DiffType"
    	, &Get().cTrackerCfgParam.cVideoDet.nDiffType
    	, Get().cTrackerCfgParam.cVideoDet.nDiffType
    	, "0:֡����ȡ;1:֡���sobelǰ��"
    	, "ǰ����ȡ��ʽ"
    	, "ֵΪ0:֡����ȡ ֵΪ1:֡���sobelǰ������Ը���·����������������"
    	, CUSTOM_LEVEL
    );
    
    GetEnum("\\Tracker\\VideoDet"
    	, "MedFilter"
    	, &Get().cTrackerCfgParam.cVideoDet.nMedFilter
    	, Get().cTrackerCfgParam.cVideoDet.nMedFilter
    	, "0:�ر�;1:��"
    	, "���ƶ�������"
    	, ""
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\Tracker\\VideoDet"
    	, "MedFilterUpLine"
    	, &Get().cTrackerCfgParam.cVideoDet.nMedFilterUpLine
    	, Get().cTrackerCfgParam.cVideoDet.nMedFilterUpLine
    	, 0
    	, 100
    	, "���ƶ�����������������"
    	, "ֵΪ0:��Ļ���Ϸ� ֵΪ100:��Ļ���·�"
    	, CUSTOM_LEVEL
    );
    
    GetInt("\\Tracker\\VideoDet"
    	, "MedFilterDownLine"
    	, &Get().cTrackerCfgParam.cVideoDet.nMedFilterDownLine
    	, Get().cTrackerCfgParam.cVideoDet.nMedFilterDownLine
    	, 0
    	, 100
    	, "���ƶ�����������������"
    	, "ֵΪ0:��Ļ���Ϸ� ֵΪ100:��Ļ���·�"
    	, CUSTOM_LEVEL
    );
    
   	GetEnum("\\Tracker\\Recognition"
   		, "EnableRecogCarColor"
   		, &Get().cTrackerCfgParam.fEnableRecgCarColor
   		, Get().cTrackerCfgParam.fEnableRecgCarColor
   		, "0:��;1:��"
   		, "������ɫʶ�𿪹�"
   		, ""
   		, CUSTOM_LEVEL
    );
    
    GetEnum("\\Tracker\\Recognition"
    	, "RecogFace"
    	, &Get().cTrackerCfgParam.fEnableDetFace
    	, Get().cTrackerCfgParam.fEnableDetFace
    	, "0:��;1:��"
    	, "������⿪��"
    	, ""
    	, CUSTOM_LEVEL
    );
    //��ǰֻҪ��������ʶ�𣬾ͱ���򿪳�����ɫʶ��,TODO
    if(Get().cTrackerCfgParam.fEnableDetFace)
    {
    	Get().cTrackerCfgParam.fEnableRecgCarColor = TRUE;
    }
    
    GetInt("\\Tracker\\Recognition"
    	, "FaceMin"
    	, &Get().cTrackerCfgParam.nMinFaceScale
    	, Get().cTrackerCfgParam.nMinFaceScale
    	, 0
    	, 20
    	, "��С�������(��ͼƬ��Ȱٷֱȼ���)"
    	, ""
    	, CUSTOM_LEVEL
    );
    GetInt("\\Tracker\\Recognition"
    	, "FaceMax"
    	, &Get().cTrackerCfgParam.nMaxFaceScale
    	, Get().cTrackerCfgParam.nMaxFaceScale
    	, 0
    	, 20
    	, "����������(��ͼƬ��Ȱٷֱȼ���)"
    	, ""
    	, CUSTOM_LEVEL
    );
    if (Get().cTrackerCfgParam.nMinFaceScale > Get().cTrackerCfgParam.nMaxFaceScale)
    {
        Get().cTrackerCfgParam.nMinFaceScale = 2;
        Get().cTrackerCfgParam.nMaxFaceScale = 20;
        
        UpdateInt("\\Tracker\\Recognition"
        	, "FaceMin"
        	, Get().cTrackerCfgParam.nMinFaceScale
        );  
        
        UpdateInt("\\Tracker\\Recognition"
        	, "FaceMax"
        	, Get().cTrackerCfgParam.nMinFaceScale
        );
    }

	//������ɫʶ�����	
	GetInt("\\Tracker\\ProcessPlate"
        , "EnableColorReRecog"
        , &Get().cTrackerCfgParam.nEnableColorReRecog
        , Get().cTrackerCfgParam.nEnableColorReRecog
        , 0
        , 7
        , "�ڰ׻���ʶ��"
        , ""
        , CUSTOM_LEVEL
        );

	GetInt("\\Tracker\\ProcessPlate"
        , "nBColorReThr"
        , &Get().cTrackerCfgParam.nBColorReThr
        , Get().cTrackerCfgParam.nBColorReThr
        , -60
        , 60
        , "����ʶ����ֵ"
        , ""
        , PROJECT_LEVEL
        );

	GetInt("\\Tracker\\ProcessPlate"
        , "nWColorReThr"
        , &Get().cTrackerCfgParam.nWColorReThr
        , Get().cTrackerCfgParam.nWColorReThr
        , 0
        , 240
        , "����ʶ����ֵ"
        , ""
        , PROJECT_LEVEL
        );

	GetInt("\\Tracker\\ProcessPlate"
        , "nWGColorReThr"
        , &Get().cTrackerCfgParam.nWGColorReThr
        , Get().cTrackerCfgParam.nWGColorReThr
        , 0
        , 240
        , "�Ұ���ʶ����ֵ"
        , ""
        , PROJECT_LEVEL
        );

	GetInt("\\Tracker\\ProcessPlate"
        , "EnableColorThr"
        , &Get().cTrackerCfgParam.fEnableCorlorThr
        , Get().cTrackerCfgParam.fEnableCorlorThr
        , 0
        , 1
        , "�Һڿ��ƿ���"
        , ""
        , CUSTOM_LEVEL
        );

	GetInt("\\Tracker\\ProcessPlate"
        , "nBGColorThr"
        , &Get().cTrackerCfgParam.nBGColorThr
        , Get().cTrackerCfgParam.nBGColorThr
        , 0
        , 240
        , "�Һڿ�����ֵ"
        , ""
        , PROJECT_LEVEL
        );

	GetInt("\\Tracker\\ProcessPlate"
        , "nWGColorThr"
        , &Get().cTrackerCfgParam.nWGColorThr
        , Get().cTrackerCfgParam.nWGColorThr
        , 0
        , 240
        , "�Ұ׿�����ֵ"
        , ""
        , PROJECT_LEVEL
        );

	
		return S_OK;
	}
	return E_FAIL;
}
	
HRESULT CSWLPRCapFaceParameter::InitHvDsp(VOID)
{
	if(S_OK == CSWLPRParameter::InitHvDsp())
	{
		GetEnum("\\HvDsp\\Misc"
		 	, "OutputCarSize"
		 	, &Get().cTrackerCfgParam.fOutputCarSize
		 	, Get().cTrackerCfgParam.fOutputCarSize
		 	, "0:�����;1:���"
  	  , "��������ߴ�"
  	  , ""
  	  , PROJECT_LEVEL
  	);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CSWLPRCapFaceParameter::InitCamApp(VOID)
{
	if(S_OK == CSWLPRParameter::InitCamApp())
	{
		GetEnum("\\HvDsp\\Camera\\Ctrl"
			, "EnableAutoCaptureParam"
			, &Get().cCamCfgParam.iCaptureAutoParamEnable
			, Get().cCamCfgParam.iCaptureAutoParamEnable
			, "0:�ر�;1:��"
			, "ץ�Ķ��������Զ�����"
			, "0:�ر�;1:��"
			, CUSTOM_LEVEL
			);
		GetInt("\\HvDsp\\Camera\\Ctrl"
			, "CaptureDayShutterMax"
			, &Get().cCamCfgParam.iCaptureDayShutterMax
			, Get().cCamCfgParam.iCaptureDayShutterMax
			, 400
			, 3000
			, "ץ�İ���������"
			, ""
			, CUSTOM_LEVEL
			);
		GetInt("\\HvDsp\\Camera\\Ctrl"
			, "CaptureDayGainMax"
			, &Get().cCamCfgParam.iCaptureDayGainMax
			, Get().cCamCfgParam.iCaptureDayGainMax
			, 100
			, 280
			, "ץ�İ����������"
			, ""
			, CUSTOM_LEVEL
			);
		GetInt("\\HvDsp\\Camera\\Ctrl"
			, "CaptureNightShutterMax"
			, &Get().cCamCfgParam.iCaptureNightShutterMax
			, Get().cCamCfgParam.iCaptureNightShutterMax
			, 400
			, 3000
			, "ץ������������"
			, ""
			, CUSTOM_LEVEL
			);
		GetInt("\\HvDsp\\Camera\\Ctrl"
			, "CaptureNightGainMax"
			, &Get().cCamCfgParam.iCaptureNightGainMax
			, Get().cCamCfgParam.iCaptureNightGainMax
			, 100
			, 280
			, "ץ�������������"
			, ""
			, CUSTOM_LEVEL
			);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CSWLPRCapFaceParameter::InitIPTCtrl(VOID)
{
    if (S_OK == CSWLPRParameter::InitIPTCtrl())
    {		//ȥ����Ӻ��̵Ƶ��豸
    		CSWString strCOMM;
				for(int i = 0; i < 2; i++)
				{	
					strCOMM.Format("\\Device[�ⲿ�豸]\\COM%d[�˿�%02d]", i, i);
					GetEnum((LPCSTR)strCOMM
  				  	, "DeviceType"
  				  	, &Get().cDevParam[i].iDeviceType
  				  	, 0
  				  	, "0:��;1:�����״�;2:�ս�������;4:����ά���״�;5:�Ĵ������״�"
  				  	, "�ⲿ�豸����"
  				  	, ""
  				  	, CUSTOM_LEVEL
  				);
  			}
        return S_OK;
    }
    return E_FAIL ;
}


