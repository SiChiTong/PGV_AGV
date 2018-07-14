#pragma once

#include "./DSP_Lib/arm_math.h"
#include "../HardwareDefine/Version_Boards.h"
class Parameter_Class
{
public:
	static const float wheel_lx_ly_distance ;	//�˶�ѧ��ʽ���(lx+ly)

	static const float wheel_max_line_velocity_hard;	//Ӳ�����������������ٶ�(mm/s)
	static const float wheel_min_line_velocity_hard ;	//Ӳ�������������С���ٶ�(mm/s)

	static float motor_max_rotationl_velocity_soft;	//������������ת��
	static float motor_min_rotationl_velocity_soft;	//������������ת��

	//���峵������ٶȡ���С�ٶȺ����Ӽ��ٶ�
	static float wheel_max_line_velocity;     //����������ٶ�(mm/s);
	static float wheel_min_line_velocity;      //������С���ٶ�(mm/s);

	static float wheel_acceleration_time;		//���峵�ֵ����Ӽ���ʱ��
	static float wheel_acceleration_line_velocity;		//��������߼��ٶ�(mm/s2)

	static float line_slowest_time;	//������ƶ���ʱ��

	static const float wheel_resolution;	//���ֱַ���

	static bool Is_Absolute_Coor;	//ָʾ��ǰ�����Ƿ�Ϊ��������
	static unsigned int AGV_Address_NUM;	//AGV�ĵ�ַ��

	static float movement_threshold;	//�˶���ֵ(mm)
	//static float line_threshold;	//ֱ�߾���岹��ֵ(mm)
	//static float rotate_threshold;	//��ת�ǶȲ岹��ֵ(mm)(��ת��Ϊֱ�߾���)

	enum Parameter_Num
	{
		Parameter_Motor_Max_Rotationl_Velocity = 0,	//������ת��
		Parameter_Motor_Min_Rotationl_Velocity,	//�����Сת��
		Parameter_Wheel_Acceleration_Time,//���峵�ֵ����Ӽ���ʱ��
		Parameter_Line_Slowest_Time,	//������ƶ���ʱ��
		Parameter_Is_Absolute_Coor,	//ָʾ��ǰ�Ƿ��Ǿ�������
		Parameter_AGV_Address_Code,	//AGV�ĵ�ַ��
		Parameter_Movement_Threshold	//�˶���ֵ
	};	//����������

	//void Update_Parameter(int num, float para);
	static void Init_Parameter(void);

};

namespace AGV_State
{
	typedef enum {
		Gcode_Command_IDLE,	//����
		Gcode_Command_BUSY,	//��������æ
		Gcode_Command_OK,		//ָ���������
		Gcode_Command_ERROR	//ָ�����
	} Gcode_Command_State;

	typedef enum
	{
		Movement_Command_IDLE,	//����
		Movement_Command_BUSY,	//��������æ
		Movement_Command_OK		//����������
	}Movement_Command_State;

}