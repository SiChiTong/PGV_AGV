#pragma once

class Parameter_Class
{
public:
	//���峵�����
#define DISTANCE_OF_WHEEL_X_AXES 534.0f	//����������X���ϵľ���(mm)
#define DISTANCE_OF_WHEEL_Y_AXES 534.0f	//ǰ��������Y���ϵľ���(mm)
#define MECANUM_WHEEL_DIAMETER	127.0		//���������ķ��ֱ��(mm)

	//�����˱���������
#define ENCODER_FIX_WHEEL true       //ָʾ�����������ӹ�����һ��
#define REDUCTION_RATIO 30           //���ٱ�
#define ENCODER_RESOLUTION_INIT 2000 //�������ķֱ��ʣ�������

	//������ת��
#define MOTOR_MAX_ROTATIONL_VELOCITY_HARD 3000.0f	//Ӳ������ĵ�����ת��
#define MOTOR_MIN_ROTATIONL_VELOCITY_HARD 0.0f		//Ӳ������ĵ�����ת��
	static const float wheel_max_line_velocity_hard;	//Ӳ�����������������ٶ�(mm/s)
	static const float wheel_min_line_velocity_hard;	//Ӳ�������������С���ٶ�(mm/s)

	static float motor_max_rotationl_velocity_soft;	//������������ת��
	static float motor_min_rotationl_velocity_soft;	//������������ת��

	//���峵������ٶȡ���С�ٶȺ����Ӽ��ٶ�
	static float wheel_max_angular_velocity;	//���������ٶ�(��/s)
	static float wheel_min_angular_velocity;	//������С���ٶ�(��/s)

	static float wheel_max_line_velocity;     //����������ٶ�(mm/s);
	static float wheel_min_line_velocity;      //������С���ٶ�(mm/s);

	static float wheel_acceleration_time;		//���峵�ֵ����Ӽ���ʱ��
	static float wheel_acceleration_line_velocity;		//��������߼��ٶ�(mm/s2)

	static float line_slowest_time;	//������ƶ���ʱ��

	static const float wheel_resolution;	//���ֱַ���

	void Update_Parameter(int num, float para);
	void Init(void);

	enum Parameter_Num
	{
		Motor_Max_Rotationl_Velocity = 0,	//������ת��
		Motor_Min_Rotationl_Velocity,	//�����Сת��
		Wheel_Acceleration_Time,//���峵�ֵ����Ӽ���ʱ��
		Line_Slowest_Time	//������ƶ���ʱ��
	};	//����������
};