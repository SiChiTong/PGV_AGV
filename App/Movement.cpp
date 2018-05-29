#include "Movement.h"

Movement_Class::Interpolation_Parameter_TypedefStructure Movement_Class::Interpolation_Parameter;

//因为同一时间只会执行一条运动指令，故为静态变量
Velocity_Class Movement_Class::Target_Velocity_InAGV;	//目标速度
Coordinate_Class Movement_Class::Target_Coor_InWorld;	//目标坐标

int Movement_Class::Distance_Symbols = 1; //指示待插补数据的符号

float Movement_Class::X_H_mul_X = 0.0f;	//用于计算投影向量

float Movement_Class::acc_distance = 0.0f;	//加速段距离(mm)
float Movement_Class::const_distance = 0.0f;  //匀速段距离(mm)
float Movement_Class::dec_distance = 0.0f;	//减速段距离(mm)
float Movement_Class::slowly_distance = 0.0f; //慢速段距离(mm)

float Movement_Class::acceleration_time = 0.0f; //加速段时间(s)
float Movement_Class::const_time = 0.0f;		 //匀速段时间(s)
float Movement_Class::deceleration_time = 0.0f; //减速段时间(s)
float Movement_Class::slowly_time = 0.0f;		 //慢速段时间(s)


void Movement_Class::Init_Parameter(void)
{
	Interpolation_Parameter.max_velocity_abs = Parameter_Class::wheel_max_line_velocity;
	Interpolation_Parameter.min_velocity_abs = Parameter_Class::wheel_min_line_velocity;
	Interpolation_Parameter.acceleration_abs = Parameter_Class::wheel_acceleration_line_velocity;
	Interpolation_Parameter.slow_time_abs = Parameter_Class::line_slowest_time;
}

//************************************
// Method:    Cal_Velocity
// FullName:  Movement_Class::Cal_Velocity
// Access:    public 
// Returns:   bool 若插补完成，返回false
// Parameter: const Coordinate_Class Current_Coor_InWorld 当前坐标
// Description: 根据当前坐标，计算期望速度，期望坐标
//************************************
bool Movement_Class::Cal_Velocity(const Coordinate_Class Current_Coor_InWorld)
{
	Coordinate_Class Current_Coor_InOrigin = Current_Coor_InWorld - Origin_Coor_InWorld;	//获取当前坐标在起点坐标系中的坐标
	Coordinate_Class Target_Coor_InOrigin;	//当前坐标向量在终点坐标向量上的投影（即目标坐标）
	Velocity_Class Target_Velocity_InTarget;	//期望坐标坐标系中的速度

	float X_H_mul_y = x_temp_InOrigin*Current_Coor_InOrigin.x_coor \
		+ y_temp_InOrigin*Current_Coor_InOrigin.y_coor \
		+ angle_equivalent_temp_InOrigin*(Current_Coor_InOrigin.angle_rad*Parameter_Class::wheel_lx_ly_distance);

	float k = X_H_mul_y / X_H_mul_X;

	//获取当前向量在终点向量上的投影向量，详情见《矩阵论》
	Target_Coor_InOrigin = Destination_Coor_InOrigin*k;
	//Target_Coor_InOrigin.x_coor = k*Destination_Coor_InOrigin.x_coor;
	//Target_Coor_InOrigin.y_coor = k*Destination_Coor_InOrigin.y_coor;
	//Target_Coor_InOrigin.angle_coor = k*Destination_Coor_InOrigin.angle_coor;
	//Target_Coor_InOrigin.angle_rad = k*Destination_Coor_InOrigin.angle_rad;

	float current_coor = Cal_Current_Coor_InOrigin(Target_Coor_InOrigin)*Distance_Symbols;	//获取在源坐标系上的位移
	float output_velocity = 0.0f;

	Interpolation_State = IS_Interpolating;	//正在插补

	//获取插补速度
	if (current_coor < 0.0f)	//在反方向
	{
		output_velocity = Input_Para.min_velocity_abs * Distance_Symbols;
		Target_Coor_InOrigin.Clear();
	}
	else if (current_coor < acc_distance)//在加速区内
	{
		output_velocity = sqrtf(2 * current_coor * Input_Para.acceleration_abs + Input_Para.min_velocity_abs * Input_Para.min_velocity_abs) * Distance_Symbols;
	}
	else if (current_coor < (acc_distance + const_distance))//在匀速区
	{
		output_velocity = Input_Para.max_velocity_abs * Distance_Symbols;
	}
	else if (current_coor < (acc_distance + const_distance + dec_distance))//在减速区
	{
		output_velocity = sqrtf(Input_Para.max_velocity_abs * Input_Para.max_velocity_abs - 2 * (current_coor - acc_distance - const_distance) * Input_Para.acceleration_abs) * Distance_Symbols;
	}
	else if (current_coor < (acc_distance + const_distance + dec_distance + slowly_distance - threshold))//在慢速区
	{
		output_velocity = Input_Para.min_velocity_abs * Distance_Symbols;
	}
	else    //超出了范围
	{
		output_velocity = 0.0f;
		Target_Coor_InOrigin = Destination_Coor_InOrigin;
		Interpolation_State = IS_Interpolated;
	}

	//分配速度给各个轴
	Target_Velocity_InTarget = Assign_Velocity(Destination_Coor_InOrigin, output_velocity);
	//将起点坐标系中的速度旋转至AGV坐标系
	Target_Velocity_InAGV = Velocity_Class::Absolute_To_Relative(Target_Velocity_InTarget, Target_Velocity_InAGV, Target_Coor_InOrigin);

	//计算期望坐标在世界坐标系上的坐标
	Target_Coor_InWorld = Origin_Coor_InWorld + Target_Coor_InOrigin;
	return (Interpolation_State != IS_Interpolated);	//返回插补结果，若插补完成，返回false
}
