#pragma once

//���ڴ���˶�ָ��

/*
* �ٶȲ岹
* �������β岹
* ��Ϊ�ĶΣ����ǵ����������С�ٶȣ�
*	�ȼ��ٶ�ta
*	���ٶ�tc
*	�ȼ��ٶ�td
*	�ȵ��ٶ�ts
*/

#include "Position.h"
#include "../macros.h"

class Movement_Class
{
public:
	Movement_Class() :Interpolation_OK(false) {}
	~Movement_Class() = default;
	typedef struct
	{
		float max_velocity_abs; //����ٶ�(mm/ms���/ms)
		float min_velocity_abs; //��С�ٶ�(mm/ms���/ms)
		float acceleration_abs; //���ٶ�(mm/ms2���/ms2)
		float slow_distance_abs;	//��С�ٶ��ƶ���λ��(mm���)
	} Actual_INPUT_TypedefStructure;

	virtual bool Init(const Actual_INPUT_TypedefStructure&Input, float threshold, bool Is_Linear = true) = 0;	//�����˶���������(����)�岹�˶�·��
	bool Get_Expectation(const Coordinate_Class Current_Coor_InWorld);	//���ݵ�ǰ�������Ŀ�����꣬Ŀ���ٶ�
	void Set_Destination(Coordinate_Class &coor) { Destination_Coor_InWorld = coor; }
	void Set_Origin(Coordinate_Class&coor) { Origin_Coor_InWorld = coor; }

	bool Interpolation_OK;	//true��ʾ�岹���
	bool Is_Linear;	//��ʾ��ǰΪֱ���˶�

	static Velocity_Class Target_Velocity_InAGV;	//Ŀ���ٶ�
	static Coordinate_Class Target_Coor_InWorld;	//Ŀ������

protected:

	virtual float Cal_Displacement(const Coordinate_Class Destination_Coor_InOrigin) = 0;	//�����յ���������������е��������岹����
	virtual Velocity_Class& Cal_Velocity(const Coordinate_Class&Destination_Coor_InOrigin, const float velocity) = 0;	//�����յ���������������е����꣬�����ٶȷ����������
	virtual float Cal_Current_Coor_InOrigin(const Coordinate_Class Current_Coor_InOrigin) = 0;	//���ݵ�ǰ���������Դ����ϵ�ϵ�λ��

	Coordinate_Class Destination_Coor_InWorld;	//�յ�����
	Coordinate_Class Origin_Coor_InWorld;	//�������
	Coordinate_Class Destination_Coor_InOrigin;	//�������ϵ�е��յ�����

	Actual_INPUT_TypedefStructure Input_Para;



	//��Ϊͬһʱ��ֻ��ִ��һ���˶�ָ���Ϊ��̬����
	static float X_H_mul_X;	//���ڼ���ͶӰ����
	static int Distance_Symbols; //ָʾ���岹���ݵķ���

	static float acc_distance;	//���ٶξ���(mm)
	static float const_distance;  //���ٶξ���(mm)
	static float dec_distance;	//���ٶξ���(mm)
	static float slowly_distance; //���ٶξ���(mm)

	static float acceleration_time; //���ٶ�ʱ��(ms)
	static float const_time;		 //���ٶ�ʱ��(ms)
	static float deceleration_time; //���ٶ�ʱ��(ms)
	static float slowly_time;		 //���ٶ�ʱ��(ms)
};
