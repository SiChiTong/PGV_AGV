#pragma once
#include "Wireless_Serial.h"
#include "../HALayer/IO.h"

class AS62_Class final :public Wireless_Serial_Class
{
public:
	AS62_Class() :Wireless_Serial_Class(), M0(Wireless_M0_Port, Wireless_M0_Pin), M1(Wireless_M1_Port, Wireless_M1_Pin) {}
	~AS62_Class() = default;

	void Init(uint32_t baudrate); //���ݲ����ʳ�ʼ������,����M0�ź�M1��

	uint32_t Return_Baudrate(void) override;	//���ش��ڲ�����
	void Mode(WF_Mode mode, bool value) override;

private:

	bool Analyze_Para_Data(const char*rx) override;	//������������
	void Code_Para_Data(void) override;	//�Բ������б���,����
	void Send_Read_CMD(void) override;	//���Ͷ�����ָ��

	IO_Class M0;  //AS62��M0��
	IO_Class M1; //AS62��M1��
};
