#ifndef __SWCAMERA_CONTROL_PARAMETER_H__
#define __SWCAMERA_CONTROL_PARAMETER_H__
typedef struct _CameraParam_info
{
    INT iDiffCorEnable; 				/**< AGC������У��ʹ�� */
    INT iCapiffCorEnable;               /**< ץ��AGC������У��ʹ�� */
    INT iAGCEnable;                     /**< AGCʹ�� */
    INT iAWBEnable;                     /**< AWBʹ�� */

    INT iAGCTh;                         /**< AGC�������� */
    INT iAGCShutterLOri;                /**< AGC���ŵ������� */
    INT iAGCShutterHOri;                /**< AGC���ŵ������� */
    INT iAGCGainLOri;                   /**< AGC����������� */
    INT iAGCGainHOri;                   /**< AGC����������� */
	INT rgiAGCZone[16];					/**< AGC������� */
	INT iAGCScene;						/**< AGC�龰ģʽ */

    INT iGainR;                         /**< R���� */
    INT iGainG;                         /**< G���� */
    INT iGainB;                         /**< B���� */
    INT iGain;                          /**< ������� */
    INT iShutter;                       /**< ����ʱ�� */

	// zhaopy ������������ʹ���������ץ��������ò���Ч��
	// ��������Ҫ���Ӷ�Ӧ�����
	INT iCaptureAutoParamEnable;		/**< ʹ��ץ���Զ����� */
	INT iCaptureRGBEnable;				/**< ʹ��ץ��RGB�������� */
	INT iCaptureShutterEnable;			/**< ʹ��ץ�Ķ�������ʱ�� */
	INT iCaptureGainEnable;				/**< ʹ��ץ�Ķ������� */
	INT iCaptureSharpenEnable;          /**< ʹ��ץ�Ķ������ */

	INT iEnableCaptureDay;              /**< ����ʹ��ץ�Ŀ��� */
	INT iEnableCaptureParamDay;         /**< ����ʹ�ܶ���ץ�Ĳ������� */
    INT iCaptureGainR;                  /**< ץ��R���� */
    INT iCaptureGainG;                  /**< ץ��G���� */
    INT iCaptureGainB;                  /**< ץ��B���� */
    INT iCaptureGain;                   /**< ץ��������� */
    INT iCaptureShutter;                /**< ץ�Ŀ���ʱ�� */
    INT iCaptureSharpenThreshold;       /**< ץ�����ֵ */

    // DC��Ȧ
    INT iEnableDCAperture;              /**< ʹ��DC��Ȧ 0: ��ʹ��  1��ʹ�� */
    // ͼ����ǿ��
    INT iEnableSharpen;                 /**< ʹ���񻯿��� 0: ��ʹ��  1��ʹ�� */
    INT iSharpenCapture;                /**< ��ץ��ͼ��1����ץ��ͼ 0��������ͼ��*/
    INT iSharpenThreshold;              /**< �񻯷�ֵ[0~100] */

	BOOL fColorMatrixEnable;			/**< ʹ��ͼ����ǿ���ܣ�0Ϊ�رգ�1Ϊ�� */
    INT iColorMatrixMode;				/**< ʹ��ͼ����ǿ���ܣ�1 Ϊ�ر�ʹ�ܣ�2Ϊʹ��ץ�ģ�3Ϊʹ������֡ */
    INT iContrastValue;                 /**< �Աȶ�ֵ[-100~100] */
    INT iSaturationValue;               /**< ���Ͷ�ֵ[-100~100] */
    INT iTemThreshold;              	/**< ɫ��5������[32,40,52,55,60,70] */
    INT iHueThreshold;              	/**< ɫ��[-180~180] */

    INT iHazeRemovalMode;				/**< ʹ��ͼ������ܣ�1 Ϊ�ر�ʹ�ܣ�2Ϊʹ��ץ�ģ�3Ϊʹ������֡ */
    INT iHazeRemovalThreshold;			/**< ͼ�����ֵ[0~10] */


   	// zhaopy
	// ���²���ֻ��Ҫ��ʼ��ʱ���ü��ɡ�
	INT iFlashRateSynSignalEnable;      /**< Ƶ��ͬ���ź�ʹ�� */
	INT iFlashRateMultiplication;		    /**< Ƶ������Ƿ�Ƶ��1����Ƶ��� */
	INT iFlashRatePolarity;				     /**< Ƶ��������ԡ�1������0����*/
	INT iFlashRatePulseWidth;			    /**< Ƶ���������100 �� 20000. ΢��*/
	INT iFlashRateOutputType;			    /**< Ƶ��������͡�1����������ƽ��, 0:OC(����) */
	INT iCaptureSynSignalEnable;      /**< ץ��ͬ���ź�ʹ�� */
	INT iCaptureSynPolarity;			   	/**< ץ��������ԡ�1������0����*/
	INT iCaptureSynPulseWidth;				/**< ץ���������100 �� 20000. ΢��*/
	INT iCaptureSynOutputType;				/**< ץ��������͡�1����������ƽ��, 0:OC(����) */
	INT iCaptureSynCoupling;					/**< Ƶ����ץ���Ƿ���ϡ� 1����ϣ�0������ϡ�*/

	// ���̵���ز�����
	//
	BOOL fRedLightEnable;		//��ƼӺ쿪��
	SW_RECT rgRedLightRect[8];					/**<��ƼӺ����� */
	INT  nRedLightCount;
	INT iHThreshold;			// ����ƵƼӺ��йأ�HSL�ռ䣬ɫ����ֵ����Χ0-184320������Ϊ1/512�ȣ�����ﷶΧ��0-360��
	INT iLThreshold;			//����ƵƼӺ��йأ�HSL�ռ䣬������ֵ����Χ0-16383
	INT iSThreshold;			// ����ƵƼӺ��йأ�HSL�ռ䣬���Ͷ���ֵ����Χ0-16383
	INT iColorFactor;			//����ƵƼӺ��йأ�ɫ��˷����ӣ���Χ0-511
	INT iLumLThreshold;		//����׵ƼӺ��йأ�������ֵ����Χ0-16383
	INT iLumLFactor;			// ����׵ƼӺ��йأ����ȳ˷����ӣ���Χ0-511
	INT iLumHFactor;			// ����׵ƼӺ��йأ�ɫ��˷����ӣ���Χ0-511

	// ����ͬ������
	INT iExternalSyncMode;		// 0-������;1-�����ڲ�����ͬ����2-�����ⲿ����ͬ����Ŀǰ����ӵڶ�·ץ���źŽӿڽ���ͬ���źţ�
	INT iSyncRelay;				// ��ͬ���ź��ӳ٣���Χ0-20000����λΪ1us.

	// Ӳ��������
	INT iCaptureEdgeOne;			// ��һ·Ӳ����ץ���صĹ���ģʽ��0-�ⲿ�½��ش�����1-�ⲿ�����ش�����2-�������½��ض�������3-������
	INT iCaptureEdgeTwo;			// �ڶ�·Ӳ����ץ���صĹ���ģʽ��0-�ⲿ�½��ش�����1-�ⲿ�����ش�����2-�������½��ض�������3-������
	INT iFlashDifferentLaneExt;     // �Ƿ�ֳ�����0-���ֳ�������·Ӳ��������ʹ��������ƹ�����1-�ֳ�������һ·Ӳ����ֻ��ʹ��һ·����ƹ������ڶ�·Ӳ����ֻ��ʹ�ڶ�·����ƹ���

	// �������
	BOOL fEnableTriggerOut; // �������ʹ�ܿ���
	INT nTriggerOutNormalStatus; // ���������̬����(����)
	INT nCaptureSynOutputType;		//����ץ���������

    //wdrǿ��ֵ
    INT iWDRStrength;
    //����
    BOOL fTNFEnable;
    BOOL fSNFEnable;
    INT iDenoiseState;
	//�ڰ�ͼ����
	BOOL fGrayImageEnable;	//�ڰ�ͼ����
	//CVBS
    INT iCVBSMode;			//CVBS ģʽ

    INT iFilterSwitchState;	//�˹�Ƭ

    INT iSaturationEnable;//���Ͷ�ʹ��
    INT iContrastEnalbe;//�Աȶ�ʹ��
    INT iGammaValue;
    INT iGammaEnable;
    INT iEdgeValue;//ͼ���Ե��ǿ

	INT iLEDPolarity;	//����ƿ��Ƽ���
	INT iLEDOutputType;		//�������
	INT iLEDPluseWidth;  //�������
	INT iALMPolarity;				/**< ����������ԡ�1������0����*/
	INT iALMOutputType;				/**< ����������͡�1����������ƽ��, 0:OC(����)*/
	INT iFlashEnable;		//�����ʹ��
	INT iFlashPolarity;		//����Ƽ��� 1:�� 0����
    INT iFlashOutputType;	//������������ 1��OC���� 0��������ƽ
    INT iFlashCoupling;		//������������  1����� 0�������
	INT iFlashPluseWidth;	//�������� 0��100
	
	INT iColorGradation;			/**<ɫ�� 0 �� 0~255��1��16~235 */
	INT iF1OutputType;				/**< F1������͡�1:���������ź�;0:���˻�����ź�*/

	INT iCameraWorkMode;					/**<����ģʽ 0-�շ�վ/����� 1-����·ģʽ 6-���Ӿ��� 7-ͣ����*/
//  INT iSharpenEnable;//��ʹ��

	INT iIris;	/**< ��Ȧ�趨ֵ��ֻ��ľ����Ч */
	INT iAutoFocus;	/**< �Զ��۽�ֵ��ֻ��ľ����Ч */
	INT iAWBMode;                     /**< AWBģʽ������ľ�Ƕ�ʹ�õ� */
	INT iAEMode;                     /**< AEģʽ ��ֻ��ľ����Ч*/
	INT iWDREnable;                     /**< WDR���� ��ֻ��ľ����Ч*/
	INT iNRLevel;                     /**< ����ȼ���ֻ��ľ����Ч*/
}CAMERAPARAM_INFO,*LPCAMERAPARAM_INFO;
#endif
