#ifndef CSWIMAGEQUALITYSET_H
#define CSWIMAGEQUALITYSET_H
#include "SWObject.h"


class CSWImageQualitySet: public CSWObject
{
CLASSINFO(CSWImageQualitySet,CSWObject)
    enum{ GAMMA_ROW = 8 , GAMMA_COL = 2};
    INT m_arrGamma[GAMMA_ROW][GAMMA_COL];

public:
    // MODE_CLOSE-�ر�ɫ�ʵ������ܣ�MODE_CAPTURE-������ץ��֡��MODE_VIDEO_CAPUTRUE-����������֡
    enum{ MODE_CLOSE = 1, MODE_CAPTURE , MODE_VIDEO_CAPUTRUE};
public:
    CSWImageQualitySet();
    virtual ~CSWImageQualitySet();

    // ����GAMMAֵswpa_camera_imgproc_set_gamma
    VOID SetGamma( INT *pData );

    /**
    * @brief ����ɫ�ʲ���\n
    *
    * @param [in] mode ����ģʽ MODE_CLOSE-�ر�ɫ�ʵ������ܣ�MODE_CAPTURE-������ץ��֡��
    * MODE_VIDEO_CAPUTRUE-����������֡
    * @note .
    * @see .
    */
    INT SetColorParamMode( INT nMode );
    /**
    * @brief ����ɫ�ʲ���\n
    *
    * @param [in] contrast_value �Աȶ�ֵ����Χ��[-100~100]��
    * Ĭ��ֵ��0(��ֵ���Ӧ�ò㣬����õ���������FPGA�Ĵ�����Ĭ��ֵ)
    * @retval 0 : ִ�гɹ�
    * @retval -1 : ִ��ʧ��
    * @note .
    * @see .
    */
    INT SetImageContrast( INT nContrast);
    /**
    * @brief ����ɫ�ʲ���\n
    *
    * @param [in] saturation_value ���Ͷ�ֵ����Χ��[-100~100]��
    * Ĭ��ֵ��0(��ֵ���Ӧ�ò㣬����õ���r/g/b������FPGA�Ĵ�����Ĭ��ֵ)
    * @retval 0 : ִ�гɹ�
    * @retval -1 : ִ��ʧ��
    * @note .
    * @see .
    */
     INT SetImageSaturation( INT nSaturation);
    /**
    * @brief ����ɫ�ʲ���\n
    *
    * @param [in] color_temperature_value ɫ��ֵ
    * @retval 0 : ִ�гɹ�
    * @retval -1 : ִ��ʧ��
    * @note .
    * @see .
    */
     INT SetImageTemperature( INT nTemperature);
    /**
    * @brief ����ɫ�ʲ���\n
    *
    * @param [in] hue_value ɫ��ֵ��ɫ�ʡ�ɫ�ࣩ
    * @retval 0 : ִ�гɹ�
    * @retval -1 : ִ��ʧ��
    * @note .
    * @see .
    */
     INT SetImageHue( INT nHue );
     // ��ȡ��ز���
     INT GetColorParam( INT &nMode , INT &nContrast
                       ,  INT &nSaturation , INT &nTemperature , INT &nHue );

    /**
    * @brief �����񻯹���ģʽ
    *
    * @param [in] mode ����ģʽ MODE_CLOSE-�ر��񻯵������ܣ�MODE_CAPTURE-������ץ��֡��
    * MODE_VIDEO_CAPUTRUE-����������֡
    * @note .
    * @see .
    */
    INT SetSharpenMode( INT nMode );

    /**
    * @brief ��������ز���\n
    *
    *
    * @param [in] mode ����ģʽ 1-�ر��񻯹��ܣ�2-������ץ��֡��3-����������֡
    * @param [in] threshold ��ֵ ��Χ1-3
    * @retval 0 : ִ�гɹ�
    * @retval -1 : ִ��ʧ��
    * @note
    * @see .
    */
    INT SetSharpenParam( INT threshold );
    /**
    * @brief ��ȡ����ز���\n
    *
    *
    * @param [out] mode ����ģʽ 1-�ر��񻯹��ܣ�2-������ץ��֡��3-����������֡
    * @param [out] threshold ��ֵ ��Χ0-127
    * @retval 0 : ִ�гɹ�
    * @retval -1 : ִ��ʧ��
    * @note
    * @see .
    */
    INT GetSharpenParam( INT &nMode , INT &threshold );

    /**
    * @brief ���ó�����ز���\n
    *
    *
    * @param [in] mode ����ģʽ MODE_CLOSE-�ر��񻯵������ܣ�MODE_CAPTURE-������ץ��֡��
    * MODE_VIDEO_CAPUTRUE-����������֡
    * @retval 0 : ִ�гɹ�
    * @retval -1 : ִ��ʧ��
    * @note
    * @see .
    */
    INT SetHazeRemovalMode( INT nMode );
    /**
    * @brief ���ó�����ز���\n
    *
    * @param [in] threshold ��ֵ ��Χ0-10
    */
    INT SetHazeRemovalParam( INT threshold );

protected:

private:
    // ģʽ,Ĭ��ֵΪMODE_VIDEO_CAPUTRUE
    INT m_nMode;
    // ��ʹ��ģʽ
    INT m_nSharpenEnMode;
    INT m_nSharpenThreshold;
    //����ʹ��ģʽ
    INT m_nHazeMode ;
    // ����ֵ
    INT m_nHazeThreshold;
    // �Աȶ�
    INT m_nContrast;
    // ���Ͷ�
    INT m_nSaturation;
    // ɫ��ֵ
    INT m_nTemperature;
    //ɫ��ֵ
    INT m_nHue;
};

#endif // CSWIMAGEQUALITYSET_H

