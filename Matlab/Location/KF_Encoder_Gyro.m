clc;
clear;
%��һ������Ϊ���������ٶ�
%�ڶ�������Ϊ�����ǽ��ٶ�
%������Ϊ�����ǽǶ�
%������ΪPGV�������Ƕ�
%������Ϊʱ��
data=load('ReceivedTofile-COM4-2018-5-10-20-10-11.txt');

theta_rate_by_encoder=data(:,1)';
theta_rate_by_gyro=data(:,2)';
theta_by_gyro=data(:,3)';
theta_by_pgv=data(:,4)'-data(1,4)';
time=data(:,5)';
theta_rate_target=abs(600*127/4/(534+534)*sin(time / 3.14 * pi));

measure=[theta_by_gyro;theta_rate_by_gyro];%��һ��Ϊ�Ƕȣ��ڶ���Ϊ���ٶ�
x_KF=measure;
A=[1,0;0,0];
B=[0.02;1];
P=[0,0;0,1];    %��ʼ��Э�������
R=[0.2,0;0,0.08];   %�˶�����
Q=[0.1,0;0,0.02];  %��������


theta_by_cal_use_encoder=zeros(1,length(time));
theta_rate_by_cal_pgv=zeros(1,length(time));

for i=2:length(time)
    theta_by_cal_use_encoder(i)= mod(theta_rate_by_encoder(i)*0.02+ theta_by_cal_use_encoder(i-1),360);
    delta_theta=theta_by_pgv(i)-theta_by_pgv(i-1);
    if delta_theta<-180
        delta_theta=delta_theta+360;
    elseif delta_theta>180
        delta_theta=delta_theta-360;
    end
    theta_rate_by_cal_pgv(i)=delta_theta/0.02;
    x_KF(:,i)=A*x_KF(:,i-1)+B*theta_rate_by_encoder(i);%����Ԥ��ֵ
    P=A*P*A'+R;%����Э�������Ԥ��ֵ
    K=P/(P+Q);  %��������
    x_KF(:,i)=x_KF(:,i)+K*(measure(:,i)-x_KF(:,i));%����Ԥ��ֵ
    P=(eye(2)-K)*P; %����Э�������
    
end


% subplot(1, 2, 1);
% plot(time,theta_by_pgv,'b');
% hold on
% plot(time,theta_by_gyro,'o');
% hold on
% plot(time,theta_by_cal_use_encoder,'g')
% hold on
% plot(time,x_KF(1,:),'k');
% xlim([18 30])
% 
% subplot(1, 2, 2);
plot(time,theta_rate_by_gyro,'b');
hold on
plot(time,theta_rate_by_encoder,'r');
hold on
plot(time,theta_rate_target,'g');
hold on
plot(time,x_KF(2,:),'k');
hold on
% plot(time,theta_rate_by_cal_pgv,'c');
xlim([91 95])

min(theta_by_pgv-x_KF(1,:))
