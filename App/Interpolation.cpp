#include "Interpolation.h"
#include "../DSP_Lib/arm_math.h"

Interpolation_Class::Interpolation_Parameter_TypedefStructure Interpolation_Class::Interpolation_Parameter;

enum Interpolation_Class::Interpolation_State_Enum Interpolation_Class::Interpolation_State;

float Interpolation_Class::target_distance = 0.0f;
float Interpolation_Class::target_velocity = 0.0f;
float Interpolation_Class::distance = 0.0f;

float Interpolation_Class::threshold = 0.0f;	//�岹��ֵ

//��Ϊͬһʱ��ֻ��ִ��һ���˶�ָ���Ϊ��̬����
int Interpolation_Class::Distance_Symbols = 1; //ָʾ���岹����ķ���

float Interpolation_Class::acc_distance = 0.0f;	//���ٶξ���(mm)
float Interpolation_Class::const_distance = 0.0f;  //���ٶξ���(mm)
float Interpolation_Class::dec_distance = 0.0f;	//���ٶξ���(mm)
float Interpolation_Class::slowly_distance = 0.0f; //���ٶξ���(mm)

float Interpolation_Class::acceleration_time = 0.0f; //���ٶ�ʱ��(s)
float Interpolation_Class::const_time = 0.0f;		 //���ٶ�ʱ��(s)
float Interpolation_Class::deceleration_time = 0.0f; //���ٶ�ʱ��(s)
float Interpolation_Class::slowly_time = 0.0f;		 //���ٶ�ʱ��(s)


bool Interpolation_Class::Init(const float distance, const float threshold)
{
	if (ABS(distance) < threshold)
	{
		return false;	//���岹����С����ֵ���岹ֱ�ӽ���
	}

	Interpolation_Class::threshold = threshold;

	float slowly_distance_temp = \
		Interpolation_Parameter.min_velocity_abs*Interpolation_Parameter.slow_time_abs;	//����������ƶ�����
	//�Ӽ��ٶε����λ�ƣ������ж��Ƿ�������ٶ�
	float distance_acc_dec_temp = \
		(Interpolation_Parameter.max_velocity_abs * Interpolation_Parameter.max_velocity_abs \
			- Interpolation_Parameter.min_velocity_abs * Interpolation_Parameter.min_velocity_abs) \
		/ Interpolation_Parameter.acceleration_abs;

	Distance_Symbols = (distance > 0.0f ? 1 : -1); //���岹��ֵ�ķ���
	Interpolation_Class::distance = distance;
	float input_distance_abs = distance;
	input_distance_abs *= Distance_Symbols;	//�ƶ�����ľ���ֵ

	//��λĿ�������ٶ�
	target_velocity = target_distance = 0.0f;

	//��λ����ֵ
	acceleration_time = const_time = deceleration_time = slowly_time = 0.0f;
	acc_distance = const_distance = dec_distance = slowly_distance = 0.0f;

	input_distance_abs -= slowly_distance_temp;	//�ȼ�ȥ���ٶεľ���

	if (input_distance_abs < 0)	//�޷��������ٶεľ��루��ֻ�����ٶΣ�
	{
		slowly_distance = input_distance_abs + slowly_distance_temp;	//���ٶ��ƶ�����(mm)
		slowly_time = slowly_distance / Interpolation_Parameter.min_velocity_abs;	//���ٶ�ʱ��(s)
		Interpolation_Parameter.max_velocity_abs = Interpolation_Parameter.min_velocity_abs;	//ֻ�������
		return true;
	}
	else if (input_distance_abs < distance_acc_dec_temp)	//���������ٶ�
	{
		//����Ӽ���ʱ��(s)
		Interpolation_Parameter.max_velocity_abs = \
			sqrtf(input_distance_abs * Interpolation_Parameter.acceleration_abs \
				+ Interpolation_Parameter.min_velocity_abs *Interpolation_Parameter.min_velocity_abs);	//��������ٶ�(mm/s)
		acceleration_time = (Interpolation_Parameter.max_velocity_abs - Interpolation_Parameter.min_velocity_abs)\
			/ Interpolation_Parameter.acceleration_abs;	//����Ӽ���ʱ��(s)
		deceleration_time = acceleration_time;
	}
	else//�������ٶ�
	{
		acceleration_time = deceleration_time = \
			(Interpolation_Parameter.max_velocity_abs - Interpolation_Parameter.min_velocity_abs) \
			/ Interpolation_Parameter.acceleration_abs;	//����Ӽ���ʱ��(s)
		const_time = (input_distance_abs - distance_acc_dec_temp) / Interpolation_Parameter.max_velocity_abs;//���ٶ�ʱ��(s)
	}

	//���������岹�����Բ���Ӽ��١�����ʱ�䣬������Ӧ�׶�λ��

	deceleration_time = acceleration_time = \
		(unsigned long)((Interpolation_Parameter.max_velocity_abs - Interpolation_Parameter.min_velocity_abs)\
			/ Interpolation_Parameter.acceleration_abs * 1000.0f) / 1000.0f; //��ȡ�Ӽ���ʱ��(s)��Բ��

	Interpolation_Parameter.max_velocity_abs = \
		Interpolation_Parameter.min_velocity_abs + acceleration_time * Interpolation_Parameter.acceleration_abs;	//��������ٶ�(mm/s)

	const_time = (unsigned long)(const_time*1000.0f) / 1000.0f;		//Բ������ʱ��(s)

	dec_distance = acc_distance = \
		(Interpolation_Parameter.max_velocity_abs + Interpolation_Parameter.min_velocity_abs) * acceleration_time / 2.0f;	//����Ӽ��ٶ�λ��

	const_distance = Interpolation_Parameter.max_velocity_abs * const_time;	//�������ٶ�λ��

	slowly_distance = input_distance_abs + slowly_distance_temp - dec_distance - acc_distance - const_distance;	//����λ��
	slowly_time = slowly_distance / Interpolation_Parameter.min_velocity_abs; //��ȡ�ܵ�����ʱ��(s)

	return true;
}

bool Interpolation_Class::Cal_Velocity(float current_distance)
{
	Interpolation_State = IS_Interpolating;	//���ڲ岹

	current_distance *= Distance_Symbols;
	//��ȡ�岹�ٶ�
	if (current_distance < 0.0f)	//�ڷ�����
	{
		target_velocity = Interpolation_Parameter.min_velocity_abs * Distance_Symbols;
		//Target_Coor_InOrigin.Clear();
	}
	else if (current_distance < acc_distance)//�ڼ�������
	{
		target_velocity = sqrtf(2 * current_distance * Interpolation_Parameter.acceleration_abs + Interpolation_Parameter.min_velocity_abs * Interpolation_Parameter.min_velocity_abs) * Distance_Symbols;
	}
	else if (current_distance < (acc_distance + const_distance))//��������
	{
		target_velocity = Interpolation_Parameter.max_velocity_abs * Distance_Symbols;
	}
	else if (current_distance < (acc_distance + const_distance + dec_distance))//�ڼ�����
	{
		target_velocity = sqrtf(Interpolation_Parameter.max_velocity_abs * Interpolation_Parameter.max_velocity_abs - 2 * (current_distance - acc_distance - const_distance) * Interpolation_Parameter.acceleration_abs) * Distance_Symbols;
	}
	else if (current_distance < (acc_distance + const_distance + dec_distance + slowly_distance - threshold))//��������
	{
		target_velocity = Interpolation_Parameter.min_velocity_abs * Distance_Symbols;
		//Target_Coor_InOrigin = Destination_Coor_InOrigin;
	}
	else if (current_distance > (acc_distance + const_distance + dec_distance + slowly_distance + threshold))//��������
	{
		target_velocity = -Interpolation_Parameter.min_velocity_abs * Distance_Symbols;
	}
	else    //����Χ��
	{
		float temp = ABS(acc_distance + const_distance + dec_distance + slowly_distance - current_distance);
		target_velocity = 0.0f;
		target_distance = Interpolation_Class::distance;
		Interpolation_State = IS_Interpolated;
	}

	return (Interpolation_State != IS_Interpolated);	//���ز岹��������岹��ɣ�����false
}
