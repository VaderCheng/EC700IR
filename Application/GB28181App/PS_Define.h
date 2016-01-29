#ifndef __PS_DEFINE_H__
#define __PS_DEFINE_H__


typedef struct pack_header_tt{
    unsigned char pack_start_code[4];  							//'0x000001BA'
	unsigned char fix_bit:								2;    	//'01'
	unsigned char system_clock_reference_base32_30:		3;		//ϵͳʱ�ӻ����ο�ֵ
	unsigned char marker_bit0:							1;
    unsigned short system_clock_reference_base29_15: 	15;
  	unsigned char marker_bit1:							1;
  	unsigned short system_clock_reference_base14_0:		15;	
  	unsigned char marker_bit2:							1;
  	unsigned short system_clock_reference_extension:	9;		//ϵͳʱ����չ�ο�ֵ
  	unsigned char marker_bit3:							1;
  	unsigned int program_mux_rate:						22;		//��ʾP-STD(������ϵͳĿ�������)���ܴ��ֶ�������ĳ����������ʣ���ÿ��50�ֽ�Ϊ��λ
    unsigned char marker_bit4:							1;
    unsigned char marker_bit5:							1;
    unsigned char reserved:								5;		
    unsigned char pack_stuffing_length:					3;		//�ڴ��ֶ�֮������ֽڵ���Ŀ
/*
	pack_header_tt()
	{
		pack_start_code[0]		= 0x00;
		pack_start_code[1]		= 0x00;
		pack_start_code[2]		= 0x01;
		pack_start_code[3]		= 0xBA;
		fix_bit 		   		= 1;		//01
		marker_bit0		   		= 1;
		marker_bit1		   		= 1;
		marker_bit2		   		= 1;
		marker_bit3 	   		= 1;
		marker_bit4		   		= 1;
		marker_bit5		   		= 1;
		reserved		   		= 0x1f;
		pack_stuffing_length	= 0;
	}*/
}__attribute__((packed)) pack_header_t;  //14

typedef struct system_header_tt
{
    unsigned char system_header_start_code[4]; 			//'0x000001BB'
    unsigned short header_length;            			//��ʾ���ֶ�֮���ϵͳ�ײ����ֽ���
    unsigned char marker_bit0:						1;  //bslbf
    unsigned int rate_bound:						22; //Ϊ���ڵ��ڳ��������������program_mux_rate�ֶ����ֵ������
    unsigned char marker_bit1:						1;  // bslbf
    unsigned char audio_bound:						6;  //Ϊ���ڵ���ͬʱ���н��봦��ĳ���������Ƶ���������Ŀ
    unsigned char fixed_flag:						1;  //"1"��ʾ�̶������ʣ�"0"�ɱ������
    unsigned char CSPS_flag:						1;  // CSPS(ϵͳ����������),"1"��ʾ�����׼����
    unsigned char system_audio_lock_flag:			1;  // ��Ƶ�����ʺ�STD�е�system_clock_frequency�ĳ���������ϵ
    unsigned char system_video_lock_flag:			1;  // ��Ƶͼ�����ʺ�STD�е�system_clock_frequency�ĳ���������ϵ
    unsigned char marker_bit2:						1;             					
    unsigned char video_bound:						5;  // ���ڵ����ڽ��봦����ͬʱ����ĳ������������Ŀ
	unsigned char packet_rate_restriction_flag:		1;
    unsigned char reserved_bits:					7;  //bslbf
	unsigned char video_stream_id;						//0XE0
	unsigned char video_fit_bit:					2;  //11
	unsigned char video_P_STD_buffer_bound_scale:	1;
	unsigned short video_P_STD_buffer_size_bound:	13;
	unsigned char audio_stream_id;						//0XC0
	unsigned char audio_fit_bit:					2;  //11
	unsigned char audio_P_STD_buffer_bound_scale:	1;
	unsigned short audio_P_STD_buffer_size_bound:	13;

    
	/* system_header_tt() */
	/* { */
	/* 	system_header_start_code[0] = 0x00; */
	/* 	system_header_start_code[1] = 0x00; */
	/* 	system_header_start_code[2] = 0x01; */
	/* 	system_header_start_code[3] = 0xBB;		 */
	/* 	header_length[0] 			= 0x00; */
	/* 	header_length[1] 			= 0x0c; */
	/* 	marker_bit1					= 1; */
	/* 	marker_bit2 				= 1; */
	/* 	marker_bit3 				= 1; */
	/* 	packet_rate_restriction_flag= 1; */
	/* 	reserved_bits				= 0x7f; */
	/* 	CSPS_flag					= 1; */
	/* 	fixed_flag					= 1; */
	/* } */
	
}__attribute__((packed)) system_header_t; //18


//define in ducument ITU-T-H.222.0-200605 page.92
typedef struct __AVC_timing_HRD_descriptor
{
	unsigned char	descriptor_tag;
	unsigned char	descriptor_length;
	unsigned char 	hrd_management_valid_flag:		1;
	unsigned char	reserved0:						6;
	unsigned char	picture_and_timing_info_present:1;
	unsigned char	_90kHz_flag:					1;
	unsigned char	reserved1:						7;
	unsigned int	N;
	unsigned int	K;
	unsigned int	num_units_in_tick;
	unsigned char	fixed_frame_rate_flag:			1;
	unsigned char	temporal_poc_flag:				1;
	unsigned char	picture_to_display_conversion_flag:1;
	unsigned char	reserved:						5;
}__attribute__((packed)) AVC_timing_HRD_descriptor_t;

//program_stream_map
typedef struct psm_header_tt
{
    unsigned char packet_start_code_prefix[3];			//'0x000001'
	unsigned char map_stream_id;						//0xBC
    unsigned short program_stream_map_length;			//��ʾ���ֶ�֮�������ӳ����ֽ�����
    unsigned char current_next_indicator:			1;	//"1":�����͵ĳ�����ӳ�䵱ǰ���ã�"0":�����ã���һ������Ϊ��Ч
    unsigned char reserved1:						2;
    unsigned char program_stream_map_version:		5;	//0~31:���������ӳ�䶨���ÿ�α仯�ı����1
    unsigned char reserved2:						7;
    unsigned char marker_bit:						1;
    unsigned short program_stream_info_length;			//���ֶκ��descriptor���ܳ��ȣ���������Ϊ0����ʾû��descriptor
    unsigned short elementary_stream_map_length;		//������ӳ��������ԭʼ����Ϣ�����ֽ�����һ��ԭʼ����Ϣ�ĳ��Ȱ����ĸ��ֽ�
    unsigned char audio_stream_type;
    unsigned char audio_elementary_stream_id;
    unsigned short audio_elementary_stream_info_length;
    unsigned char video_stream_type;					//�����ͣ���H264(0x1B),H265
    unsigned char video_elementary_stream_id;			//��������Ϣ������Ƶ��(0xe0)����Ƶ��(0xC0)
    unsigned short video_elementary_stream_info_length;	//���ֶκ�������(descriptor)���ֽ���
	
    AVC_timing_HRD_descriptor_t AVC_HRD_desc;
    unsigned char CRC_32[4];							//ѭ������У���룬�������ֵ��ô��

	/* psm_header_tt() */
	/* { */
	/* 	promgram_stream_map_start_code[0] = 0x00; */
	/* 	promgram_stream_map_start_code[1] = 0x00; */
	/* 	promgram_stream_map_start_code[2] = 0x01; */
	/* 	promgram_stream_map_start_code[3] = 0xBC; */
	/* 	program_stream_map_length[0]	  = 0x00; */
	/* 	program_stream_map_length[1]      = 0x12;		 */
	/* 	current_next_indicator 			  = 0x1; */
	/* 	reserved1						  = 0x3;	 */
	/* 	program_stream_map_version 		  = 0; */
	/* 	reserved2 						  = 0x7f; */
	/* 	marker_bit 						  = 0x01; */
	/* 	program_stream_info_length[0]	  = 0; */
	/* 	program_stream_info_length[1]	  = 0; */
	/* 	elementary_stream_map_length[0]   = 0; */
	/* 	elementary_stream_map_length[1]   = 0x04; */
	/* 	CRC_32[0]						  = 0x23; */
	/* 	CRC_32[1]						  = 0xb9; */
	/* 	CRC_32[2]						  = 0x0f; */
	/* 	CRC_32[3]						  = 0x3d;		 */
	/* } */
}__attribute__((packed)) psm_header_t; //36

typedef struct pes_header_tt
{
    unsigned char pes_start_code_prefix[3];
	unsigned char stream_id;
    unsigned short PES_packet_length;			//PES������ֽ���,"0":��ʾPES����ĳ��ȼ�û��˵��Ҳû������
	//�����ǿ�ѡ�����ʹ����Ƶ����Ƶ�����д�ѡ��
	unsigned char fix_bit:					2;	//10
	unsigned char PES_scrambling_control:	2;	//PES������Ч���صļ���ģʽ
	unsigned char PES_priority:				1;	//PES������Ч���ص����ȼ�,"1"��"0"���ȼ���
	unsigned char data_alignment_indicator:	1;	//���ݶ���ָʾ��,"0":û�ж��룬"1":����
	unsigned char copyright:				1;	//��Ч���صİ�Ȩ��"1":�а�Ȩ��"0":û�а�Ȩ
    unsigned char original_or_copy:			1;	//PES�������Ч����ʱԭʼ���ݻ��Ǳ��ݣ�"1":ԭʼ "0":����
	unsigned char PTS_DTS_flags:			2;	// "11":PTS DTS "10":PTS  ������Ч
	unsigned char ESCR_flag:				1;	//"1":����ESCR��base��extension�ֶ�
	unsigned char ES_rate_flag:				1;	//"1":����ES_rate
	unsigned char DSM_trick_mode_flag:		1;	//"1":����DSM_trick_mode_flag�ֶ�
	unsigned char additional_copy_info_flag:1;	//"1":����additional_copy_info�ֶ�
    unsigned char PES_CRC_flag:				1;	//"1":����PES�����CRC
    unsigned char PES_extension_flag:		1;
    unsigned char PES_header_data_length;		//���ֶ�֮�����еĳ���
//PTS    
    unsigned char PTS_flags:				4;
    unsigned char PTS32_30:					3;
    unsigned char marker_bit0:				1;
    unsigned short PTS29_15:				15;
	unsigned char marker_bit1:				1;
    unsigned short PTS14_0:					15;
	unsigned char marker_bit2:				1;
    

	/* pes_header_tt() */
	/* { */
	/* 	pes_start_code_prefix[0] 		 = 0x00; */
	/* 	pes_start_code_prefix[1] 		 = 0x00; */
	/* 	pes_start_code_prefix[2] 		 = 0x01; */
	/* 	fix_bit 			  			 = 0x02;					//10 */
	/* 	PES_scrambling_control 			 = 0x00; */
	/* 	PES_priority		   			 = 0x01; */
	/* 	data_alignment_indicator		 = 0x00; */
	/* 	copyright			   			 = 0x00; */
	/* 	original_or_copy				 = 0x01; */
	/* 	PTS_DTS_flags					 = 0x03; */
	/* 	ESCR_flag						 = 0x00; */
	/* 	ES_rate_flag					 = 0x00; */
	/* 	DSM_trick_mode_flag				 = 0x00; */
	/* 	additional_copy_info_flag		 = 0x00; */
	/* 	PES_CRC_flag					 = 0x00; */
	/* 	PES_extension_flag				 = 0x00; */
	/* 	PTS_flags						 = 0x02; */
	/* }  	 */
}__attribute__((packed)) pes_header_t; //6


typedef struct __video_stream_descriptor
{
	unsigned char descriptor_tag;
	unsigned char descriptor_length;					//�����ӵĳ��ȣ��Ӵ��ֶκ�ʼ����
	unsigned char multiple_frame_rate_flag:		1;		//"0":ֻ��һ��֡���ʣ�"1":�����ж���֡����
	unsigned char frame_rate_code:				4;		//multiple_frame_rate_flagΪ"1"ʱ��Ч��
	unsigned char MPEG_1_only_flag:				1;		//"1":��Ƶ���к��� ITU-T rec.H.262|ISO/IEC13818-2������
	unsigned char constrained_parameter_flag:	1;		//MPEG_2_flagΪ"1",��ֵΪ"0"
	unsigned char still_picture_flag:			1;		//"1"��ʾ��Ƶ����ֻ�о�ֹͼ��"0":�ж������߾�ֹͼ��
	unsigned char profile_and_level_indication;
	unsigned char chroma_format:				2;
	unsigned char frame_rate_extension_flag:	1;
	unsigned char reserved:						5;

	/* __video_stream_descriptor() */
	/* { */
	/* 	descriptor_tag = 0x0a;							//	 */
	/* 	descriptor_length = 0x03;						 */
	/* 	multiple_frame_rate_flag = 0; */
	/* 	frame_rate_code	= 0x0c;							//��־����Ϊ"0",���ֵΪ��Ч */
	/* 	MPEG_2_flag = 1; */
	/* 	constrained_parameter_flag = 0;		 */
	/* 	still_picture_flag = 0;		 */
	/* 	profile_and_level_indication = 0x6e; */
	/* 	chroma_format = 0x1;							//01 */
	/* 	frame_rate_extension_flag = 1;		 */
	/* 	reserved = 0x07;								 */
	/* }  */
}__attribute__((packed)) video_stream_descriptor_t;






#endif
