#include "Movement.h"

#define INTER_FLOAT_DELTA 0.001 //�岹�õĸ�������������������ֵ�ľ���ֵС�ڸ���������Ϊ��������һ��

//��Ϊͬһʱ��ֻ��ִ��һ���˶�ָ���Ϊ��̬����
Velocity_Class Movement_Class::Target_Velocity_InAGV;	//Ŀ���ٶ�
Coordinate_Class Movement_Class::Target_Coor_InWorld;	//Ŀ������

int Movement_Class::Distance_Symbols = 1; //ָʾ���岹���ݵķ���

float Movement_Class::acc_distance = 0.0f;	//���ٶξ���(mm)
float Movement_Class::const_distance = 0.0f;  //���ٶξ���(mm)
float Movement_Class::dec_distance = 0.0f;	//���ٶξ���(mm)
float Movement_Class::slowly_distance = 0.0f; //���ٶξ���(mm)

float Movement_Class::acceleration_time = 0.0f; //���ٶ�ʱ��(ms)
float Movement_Class::const_time = 0.0f;		 //���ٶ�ʱ��(ms)
float Movement_Class::deceleration_time = 0.0f; //���ٶ�ʱ��(ms)
float Movement_Class::slowly_time = 0.0f;		 //���ٶ�ʱ��(ms)


void Movement_Class::Init(const Actual_INPUT_TypedefStructure & Input)
{
	float distance_temp = (Input.max_velocity_abs * Input.max_velocity_abs - Input.min_velocity_abs * Input.min_velocity_abs) / Input.acceleration_abs;	//���������ٶ���Сλ��,��Сλ�Ƶľ���
	float input_distance = Cal_Displacement(Origin_Coor_InWorld, Destination_Coor_InWorld);	//�����ƶ�����
	Actual_INPUT_TypedefStructure Input_Para = Input;

	if (ABS(Input.min_velocity_abs) < INTER_FLOAT_DELTA) //��С�ٶ�Ϊ0
	{
		Input_Para.slow_distance_abs = 0.0f;	//ʵ����С�ٶ�λ��Ϊ0
	}

	Distance_Symbols = (input_distance > 0.0f ? 1 : -1); //���岹��ֵ�ķ���

	input_distance *= Distance_Symbols;

	//��λ����ֵ
	acceleration_time = 0.0f;
	const_time = 0.0f;
	deceleration_time = 0.0f;
	slowly_time = 0.0f;

	acc_distance = 0.0f;
	const_distance = 0.0f;
	dec_distance = 0.0f;
	slowly_distance = 0.0f;

	input_distance -= Input_Para.slow_distance_abs; //��ȥ���������ȵ��ٶεľ���

	if (input_distance < 0)	//ֻ�������λ��
	{
		slowly_time = (input_distance + Input_Para.slow_distance_abs) / Input_Para.min_velocity_abs;	//���������ʱ��
		Input_Para.max_velocity_abs = Input.min_velocity_abs;	//����ٶ�Ϊ��С�ٶ�
	}
	else if (input_distance < distance_temp)	//���������ٶ�
	{
		Input_Para.max_velocity_abs = sqrtf(input_distance * Input.acceleration_abs + Input.min_velocity_abs * Input.min_velocity_abs);
		acceleration_time = (Input_Para.max_velocity_abs - Input_Para.min_velocity_abs) / Input_Para.acceleration_abs;
		deceleration_time = acceleration_time;
	}
	else//�������ٶ�
	{
		acceleration_time = deceleration_time = (Input.max_velocity_abs - Input.min_velocity_abs) / Input.acceleration_abs;
		const_time = (input_distance - distance_temp) / Input.max_velocity_abs;
	}

	deceleration_time = acceleration_time = (long)((Input_Para.max_velocity_abs - Input_Para.min_velocity_abs) / Input_Para.acceleration_abs * 100.0f) / 100.0f; //��ȡ�Ӽ���ʱ��(ms)��Բ��
	Input_Para.max_velocity_abs = Input_Para.min_velocity_abs +acceleration_time * Input_Para.acceleration_abs;	//��������ٶ�
	const_time = (long)(const_time*100.0f) / 100.0f;		//Բ������ʱ��

	dec_distance = acc_distance = (Input_Para.max_velocity_abs + Input_Para.min_velocity_abs) * acceleration_time / 2.0f;	//����Ӽ��ٶ�λ��
	const_distance = Input_Para.max_velocity_abs * const_time;	//�������ٶ�λ��
	slowly_distance = input_distance - dec_distance * 2 - const_distance;	//����λ��
																					//slowly_distance = Input_Para.min_velocity_abs * Result.slowly_time + Input.slow_distance_abs;
	slowly_time = (long)(slowly_distance / Input_Para.min_velocity_abs * 100.0f) / 100.0f; //��ȡ�ܵ�����ʱ�䣬Բ��

}

bool Movement_Class::Get_Expectation(const Coordinate_Class Current_Coor_InWorld)
{
	Target_Coor_InWorld = Current_Coor_InWorld;	//�����Ŀ������
	target_coor = current_coor;
	current_coor *= Distance_Symbols;

	if (current_coor < 0.0f)	//�ڷ�����
	{
		output_velocity = Input_Para.min_velocity_abs * Distance_Symbols;
		target_coor = 0.0f;
	}
	else if (current_coor < acc_distance)//�ڼ�������
	{
		output_velocity = sqrtf(2 * ABS(current_coor) * Input_Para.acceleration_abs + Input_Para.min_velocity_abs * Input_Para.min_velocity_abs) * Distance_Symbols;
		//target_coor = (current_coor)* Distance_Symbols;
	}
	else if (current_coor < (acc_distance + const_distance))//��������
	{
		output_velocity = Input_Para.max_velocity_abs * Distance_Symbols;
		//target_coor = (current_coor )* Distance_Symbols;
	}
	else if (current_coor < (acc_distance + const_distance + dec_distance))//�ڼ�����
	{
		output_velocity = sqrtf(Input_Para.max_velocity_abs * Input_Para.max_velocity_abs - 2 * ABS(current_coor - acc_distance - const_distance) * Input_Para.acceleration_abs) * Distance_Symbols;
		//target_coor = (current_coor )* Distance_Symbols;
	}
	else if (current_coor < (acc_distance + const_distance + dec_distance + slowly_distance - DISATNCE_DELTA))//��������
	{
		output_velocity = Input_Para.min_velocity_abs * Distance_Symbols;
		//target_coor = (current_coor)* Distance_Symbols;
	}
	else
	{
		output_velocity = 0.0f;
		//target_coor = (current_coor)* Distance_Symbols;
		return false;
	}
	return true;
}
