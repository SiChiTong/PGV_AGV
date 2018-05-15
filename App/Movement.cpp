#include "Movement.h"
#include "../Math/MyMath.h"

#define DISATNCE_DELTA 0.3f		//��ʵ����λ����������λ�Ʋ��0.3fʱ����Ϊ�岹�����

//��Ϊͬһʱ��ֻ��ִ��һ���˶�ָ���Ϊ��̬����
Velocity_Class Movement_Class::Target_Velocity_InAGV;	//Ŀ���ٶ�
Coordinate_Class Movement_Class::Target_Coor_InWorld;	//Ŀ������

float Movement_Class::X_H_mul_X = 0.0f;

int Movement_Class::Distance_Symbols = 1; //ָʾ���岹���ݵķ���

float Movement_Class::acc_distance = 0.0f;	//���ٶξ���(mm)
float Movement_Class::const_distance = 0.0f;  //���ٶξ���(mm)
float Movement_Class::dec_distance = 0.0f;	//���ٶξ���(mm)
float Movement_Class::slowly_distance = 0.0f; //���ٶξ���(mm)

float Movement_Class::acceleration_time = 0.0f; //���ٶ�ʱ��(ms)
float Movement_Class::const_time = 0.0f;		 //���ٶ�ʱ��(ms)
float Movement_Class::deceleration_time = 0.0f; //���ٶ�ʱ��(ms)
float Movement_Class::slowly_time = 0.0f;		 //���ٶ�ʱ��(ms)


//************************************
// Method:    Get_Expectation
// FullName:  Movement_Class::Get_Expectation
// Access:    public 
// Returns:   bool ���岹��ɣ�����false
// Parameter: const Coordinate_Class Current_Coor_InWorld ��ǰ����
// Description: ���ݵ�ǰ���꣬���������ٶȣ���������
//************************************
bool Movement_Class::Get_Expectation(const Coordinate_Class Current_Coor_InWorld)
{
	Coordinate_Class Current_Coor_InOrigin = Current_Coor_InWorld - Origin_Coor_InWorld;	//��ȡ��ǰ�������������ϵ�е�����
	Coordinate_Class Target_Coor_InOrigin;	//��ǰ�����������յ����������ϵ�ͶӰ����Ŀ�����꣩


	float X_H_mul_y = Destination_Coor_InOrigin.x_coor*Current_Coor_InOrigin.x_coor \
		+ Destination_Coor_InOrigin.y_coor*Current_Coor_InOrigin.y_coor \
		+ Destination_Coor_InOrigin.angle_coor*Current_Coor_InOrigin.angle_coor;



	float k = X_H_mul_y / X_H_mul_X;

	//��ȡ��ǰ�������յ������ϵ�ͶӰ����
	Target_Coor_InOrigin.x_coor = k*Destination_Coor_InOrigin.x_coor;
	Target_Coor_InOrigin.y_coor = k*Destination_Coor_InOrigin.y_coor;
	Target_Coor_InOrigin.angle_coor = k*Destination_Coor_InOrigin.angle_coor;



	////���ݵ�ǰ������ֱ�켣��Ŀ������
	//MyMath::Coor coor_temp1, coor_temp2;

	//coor_temp1.x = Current_Coor_InOrigin.x_coor;
	//coor_temp1.y = Current_Coor_InOrigin.y_coor;

	////��ȡб�ʲ����ڵĽ���
	//if (ABS(Destination_Coor_InOrigin.x_coor) < FLOAT_DELTA)
	//{
	//	coor_temp2.y = coor_temp1.y;
	//	coor_temp2.x = Destination_Coor_InOrigin.x_coor;
	//}
	//else
	//{
	//	MyMath::Get_Vertical_Line_Crossover_Point(Destination_Coor_InOrigin.y_coor / Destination_Coor_InOrigin.x_coor, coor_temp1, coor_temp2);
	//}

	//Target_Coor_InOrigin.x_coor = coor_temp2.x;
	//Target_Coor_InOrigin.y_coor = coor_temp2.y;


	//Target_Coor_InOrigin.angle_coor = Current_Coor_InOrigin.angle_coor;


	float current_coor = Cal_Current_Coor_InOrigin(Target_Coor_InOrigin)*Distance_Symbols;	//��ȡ��Դ����ϵ�ϵ�λ��
	float output_velocity = 0.0f;

	Interpolation_OK = false;

	//��ȡ�岹�ٶ�
	if (current_coor < 0.0f)	//�ڷ�����
	{
		output_velocity = Input_Para.min_velocity_abs * Distance_Symbols;
		Target_Coor_InOrigin.Clear();
	}
	else if (current_coor < acc_distance)//�ڼ�������
	{
		output_velocity = sqrtf(2 * current_coor * Input_Para.acceleration_abs + Input_Para.min_velocity_abs * Input_Para.min_velocity_abs) * Distance_Symbols;
	}
	else if (current_coor < (acc_distance + const_distance))//��������
	{
		output_velocity = Input_Para.max_velocity_abs * Distance_Symbols;
	}
	else if (current_coor < (acc_distance + const_distance + dec_distance))//�ڼ�����
	{
		output_velocity = sqrtf(Input_Para.max_velocity_abs * Input_Para.max_velocity_abs - 2 * (current_coor - acc_distance - const_distance) * Input_Para.acceleration_abs) * Distance_Symbols;
	}
	else if (current_coor < (acc_distance + const_distance + dec_distance + slowly_distance - DISATNCE_DELTA))//��������
	{
		output_velocity = Input_Para.min_velocity_abs * Distance_Symbols;
	}
	else
	{
		output_velocity = 0.0f;
		Interpolation_OK = true;
	}

	//����������������������ϵ�ϵ�����
	Target_Coor_InWorld = Origin_Coor_InWorld + Target_Coor_InOrigin;
	//����AGV����ϵ�е������ٶ�
	Target_Velocity_InAGV = Cal_Velocity(Destination_Coor_InOrigin, output_velocity);

	return !Interpolation_OK;	//���ز岹���
}
