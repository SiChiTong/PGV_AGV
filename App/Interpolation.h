#pragma once

/*
* ·���岹�㷨
* �������β岹
* ��Ϊ�ĶΣ����ǵ����������С�ٶ�������
*	�ȼ��ٶ�ta
*	���ٶ�tc
*	�ȼ��ٶ�td
*	�ȵ��ٶ�ts
*/

//�岹ʱ���ȼ�¼���岹����ķ���
#include "../macros.h"

class Interpolation_Class
{
public:
	Interpolation_Class() = default;
	~Interpolation_Class() = default;

	typedef struct
	{
		float max_velocity_abs; //����ٶ�(mm/s)
		float min_velocity_abs; //��С�ٶ�(mm/s)
		float acceleration_abs; //���ٶ�(mm/s2)
		float slow_time_abs;	//��С�ٶ��ƶ�ʱ��(s)
	} Interpolation_Parameter_TypedefStructure;	//�岹�����ṹ�嶨��

	enum Interpolation_State_Enum
	{
		NO_Interpolation,  //δ�岹
		IS_Interpolating, //���ڲ岹
		IS_Interpolated   //�岹���
	};	//�岹״̬

	enum Interpolation_State_Enum Interpolation_State;

	//ʹ��ǰ����²岹�����Ͳ岹��ֵ
	bool Init(const float distance) { return Init(distance, Interpolation_Class::threshold); }	//���ݲ岹�����Ͳ岹����岹·��
	bool Init(const float distance, const float threshold);	//���ݴ��岹����Ͳ岹��ֵ��·���岹
	bool Cal_Velocity(float current_distance);	//���ݵ�ǰ�ƶ����������������������ٶȣ����ؼ�����

	void Update_Interpolation_Parameter(const Interpolation_Parameter_TypedefStructure& Input_Para) {
		Interpolation_Parameter = Input_Para;
	}	//���²岹����

	static float target_distance;
	static float target_velocity;

	static Interpolation_Parameter_TypedefStructure Interpolation_Parameter;	//�岹�����������ݴ�
	static float threshold;	//�岹��ֵ

private:
	static float distance;
	//��Ϊͬһʱ��ֻ��ִ��һ���˶�ָ���Ϊ��̬����
	static int Distance_Symbols; //ָʾ���岹����ķ���

	static float acc_distance;	//���ٶξ���(mm)
	static float const_distance;  //���ٶξ���(mm)
	static float dec_distance;	//���ٶξ���(mm)
	static float slowly_distance; //���ٶξ���(mm)

	static float acceleration_time; //���ٶ�ʱ��(s)
	static float const_time;		 //���ٶ�ʱ��(s)
	static float deceleration_time; //���ٶ�ʱ��(s)
	static float slowly_time;		 //���ٶ�ʱ��(s)
};
