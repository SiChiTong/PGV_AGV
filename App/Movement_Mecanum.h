#pragma once
#include "Movement.h"
#include "../parameter_define.h"

class Movement_Mecanum_Class:public Movement_Class
{
public:
	bool Init(const Actual_INPUT_TypedefStructure&Input, float threshold,bool Is_Linear = true) override;
private:
	float Cal_Displacement(const Coordinate_Class Destination_Coor_InOrigin) override;	//�����յ���������������е��������岹����
	Velocity_Class &Cal_Velocity(const Coordinate_Class&Destination_Coor_InOrigin, const float velocity) override;	//�����յ���������������е����꣬�����ٶȷ����������
	float Cal_Current_Coor_InOrigin(const Coordinate_Class Current_Coor_InOrigin) override;	//���ݵ�ǰ���������Դ����ϵ�ϵ�λ��

	float x_temp_InOrigin;	//�յ��������������ϵ�ϵ�x��ƫ��
	float y_temp_InOrigin;	//�յ��������������ϵ�ϵ�y��ƫ��
	float angle_equivalent_temp_InOrigin;	//�յ��������������ϵ�ϵĽǶ����ƫ�ƣ���ЧΪ��ƫ�ƣ�
	float distance_InOrigin;	//�յ��������������ľ������ֵ

};
