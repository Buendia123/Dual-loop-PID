/*
 * Task_Algorithm.c
 *
 *  Created on: Jan 15, 2025
 *      Author: Lenovo
 */

#include "Task_Algorithm.h"
#include "math.h"


static float target_dirz,target_dirx,target_diry;
static float Encoder_A_correct, Encoder_B_correct;
static float target_yaw;

static float line_KP=5.0,line_KD=130;
static float Balance_KP=13, Balance_KI=0, Balance_KD=140;
static float Velocity_Encode_KP=20,Velocity_Encode_KI=100;
static float Velocity_KP=14,Velocity_KI=4;

static float StriLast,StriBias,StriDiff;
static float ver_last_Bias,ver_D_Bias,ver_Error;
static float Velocity_Bias,Velocity_integral;

static VelocityControlState stateA = {0, 0, 0};
static VelocityControlState stateB = {0, 0, 0};

static int tilt_flag = 0;
int limit=5268;
int StrightControl(float Angle,float Target)
{

	float PWM,Least;
  	Least =Angle-Target;
  	StriBias *=0.8;
  	StriBias += Least*0.2;
  	StriDiff=StriBias-StriLast;
	StriLast=StriBias;
	PWM=StriBias*line_KP+StriDiff*line_KD; //位置控制
	if(buttonResult==0)
	{
    	StriLast=0;
    	StriBias=0;
		StriDiff=0;
	}
    return PWM;

}

float VerticalControl(float Angle,float Target)
{
	float Bias,PWM;
	Bias=Angle-Target;                       //角度偏差
	ver_D_Bias=Bias - ver_last_Bias;                 //两次角度差值
	if(ver_D_Bias>+3)ver_D_Bias=+3;
	if(ver_D_Bias<-3)ver_D_Bias=-3;
	ver_Error+=Bias;
	if(ver_Error>+30)ver_Error=+30;
	if(ver_Error<-30)ver_Error=-30;
	PWM=+Balance_KP*Bias + Balance_KI*ver_Error/10 + ver_D_Bias*Balance_KD;//PID控制累加
	ver_last_Bias=Bias;
	if(buttonResult==0)
	 {
		 ver_last_Bias=0;
		 ver_D_Bias=0;
		 ver_Error=0;//停止时各参数清零
	 }
	return PWM;
}

int VelocityControl(int Encoder)
{
	float PWM,Velocity_Least;

	Velocity_Least =Encoder;
	Velocity_Bias *= 0.8;		                               //一阶低通滤波器
	Velocity_Bias += Velocity_Least*0.2;	                   //一阶低通滤波器
	Velocity_integral +=Encoder;                               //积分出位移

	if(Velocity_integral>+520)
		Velocity_integral=+520; 										//积分限幅
	if(Velocity_integral<-520)
		Velocity_integral=-520; 								//积分限幅
	PWM = Velocity_Bias*Velocity_KP/10 + Velocity_integral*Velocity_KI/100; //PI控制累加
	if(buttonResult==0)
	{
		Velocity_Bias=0;
		Velocity_integral=0;
	}
	return PWM;
}



// 合并后的速度控制函数
int velocity_encode_Control(int encoder, int target, VelocityControlState *encode) {
	if(buttonResult==0) {
        // 如果需要重置，清零状态
    	encode->velocity = 0;
    	encode->bias = 0;
    	encode->Last_bias = 0;
    }

    // 计算偏差
    encode->bias = target - encoder;

    // 增量式PI控制
    encode->velocity += Velocity_Encode_KP * (encode->bias - encode->Last_bias) + encode->bias * Velocity_Encode_KI / 100;

    // 更新上一次的偏差
    encode->Last_bias = encode->bias;

    // 限制速度范围
    if (encode->velocity > +8000) encode->velocity = +8000;
    if (encode->velocity < -8000) encode->velocity = -8000;

    return (int)encode->velocity;
}

void PosControl(void)
{
	float vertical_control_value = 0;
	float velocity_control_value = 0;
	float velocity_ring_encode_A,velocity_ring_encode_B;
	ZeroDiff = ang.acc->Pitch - ZeroPoint;
	if (buttonResult == 1)
	{
		if (ZeroDiff<30 && ZeroDiff >(-30))
		{
			if (state == front && state == rear)
			{
				target_dirz = fabsf(StrightControl(ang.acc->Yaw,target_yaw));//front rear
			}
			else
				target_yaw = ang.acc->Yaw;
			target_dirz = (target_dirz > 60) ? 60 : target_dirz;
			target_dirz = (target_dirz < -60) ? -60 : target_dirz;

			Encoder_A_correct = (+target_dirx) + target_dirz;
			Encoder_B_correct = (+target_dirx) - target_dirz;
			vertical_control_value = -VerticalControl(ang.acc->Pitch, ZeroPoint);//left right
			velocity_control_value = +VelocityControl(motorval.encodeA - Encoder_A_correct + motorval.encodeB - Encoder_B_correct);
			velocity_control_value = (velocity_control_value>180)?180:velocity_control_value;
			velocity_control_value = (velocity_control_value<-180)?(-180):velocity_control_value;
			velocity_ring_encode_A = vertical_control_value + velocity_control_value + Encoder_A_correct;
			velocity_ring_encode_B = vertical_control_value + velocity_control_value + Encoder_B_correct;

			 Motor_A = velocity_encode_Control((+motorval.encodeA), velocity_ring_encode_A, &stateA);
			 Motor_B = velocity_encode_Control((+motorval.encodeB), velocity_ring_encode_B, &stateB);
			 tilt_flag = 0;
		}
		else
			tilt_flag++;
		if (tilt_flag>50)
		{
			state=stop;
			Motor_A = 0;
			Motor_B = 0;
		}
	}
	else if(buttonResult == 0)
	{
		Motor_A = 0;
		Motor_B = 0;
	}

}

void direction_control(void)
{
	switch (state) {
	    case front:
	        target_dirx = (target_dirx < control_speed) ? target_dirx + 1 : control_speed;
	        break;

	    case rear:
	        target_dirx = (target_dirx > (-control_speed)) ? target_dirx - 1 : (-control_speed);
	        break;

	    case left:
	        target_dirx = (target_dirx > 0) ? target_dirx - 0.5 : 0;
	        target_dirx = (target_dirx < 0) ? target_dirx + 0.5 : 0;
	        if (fabsf(target_dirx) < 1 && fabsf(target_diry) < 1 && (target_dirz > (-control_speed))) {
	            target_dirz -= 0.1;
	        }
	        break;

	    case right:
	        target_dirx = (target_dirx > 0) ? target_dirx + 0.5 : 0;
	        target_dirx = (target_dirx < 0) ? target_dirx - 0.5 : 0;
	        if (fabsf(target_dirx) < 1 && fabsf(target_diry) < 1 && (target_dirz < control_speed)) {
	            target_dirz += 0.1;
	        }
	        break;

	    case lefront:
	        target_dirx = (target_dirx < control_speed) ? target_dirx + 0.5 : control_speed;
	        target_dirz = (target_dirz > (-control_speed)) ? target_dirz - 0.2 : (-control_speed);
	        break;

	    case rifront:
	        target_dirx = (target_dirx < control_speed) ? target_dirx + 0.5 : control_speed;
	        target_dirz = (target_dirz < (control_speed)) ? target_dirz + 0.2 : control_speed;
	        break;

	    case rirear:
	        target_dirx = (target_dirx > (-control_speed)) ? target_dirx - 0.5 : (-control_speed);
	        target_dirz = (target_dirz > (-control_speed)) ? target_dirz - 0.2 : (-control_speed);
	        break;

	    case lerear:
	        target_dirx = (target_dirx > (-control_speed)) ? target_dirx - 0.5 : (-control_speed);
	        target_dirz = (target_dirz > (-control_speed)) ? target_dirz - 0.2 : (-control_speed);
	        break;

	    case stop:
	    	target_yaw = ang.acc->Yaw;
	        break;
		target_dirx = (target_dirx> 50)? 50:target_dirx;
		target_dirx = (target_dirx< -50)? -50:target_dirx;
		target_dirz = (target_dirz<(-control_speed)/10)?((-control_speed)/10):target_dirz;
		target_dirz = (target_dirz>(control_speed)/10)?((control_speed)/10):target_dirz;

	}
}

void Set_Pwm(int motor_A,int motor_B)
{
	if(motor_A<0)
	{
		encodeA_PWMA=7199;
		encodeA_PWMB=7199 + motor_A;
	}

	else{
		encodeA_PWMB=7199;
		encodeA_PWMA=7199 - motor_A;
	}
	if(motor_B<0){
		encodeB_PWMA=7199;
		encodeB_PWMB=7199 + motor_B;
	}
	else{
		encodeB_PWMB=7199;
		encodeB_PWMA=7199 - motor_B;
	}
}

//button push 1: self balance
//button push 2: ultrasonic tracking
void vControlTask(void *pvParameters) {

	direction_control();
	PosControl();

	if(Motor_A>+limit)Motor_A=+limit;
	if(Motor_A<-limit)Motor_A=-limit;
	if(Motor_B>+limit)Motor_B=+limit;
	if(Motor_B<-limit)Motor_B=-limit;

	if(buttonResult==1)
		Set_Pwm(Motor_A,Motor_B);
	else
	{
	 Set_Pwm(0,0);
	 Motor_A=0;
	 Motor_B=0;
	 target_yaw = ang.acc->Yaw;
	 target_dirx=0;
	}
}
