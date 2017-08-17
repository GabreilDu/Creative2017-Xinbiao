#define __WIFI_C_
#include "includes.h"
#include "math.h"


int g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
int g_remote_frame_cnt = 0;
int g_start_all=0;
int have_responsed;
int order_received; 
WORD response_data=0x0;
int Car_Waitfororder=0;
int Light_Status=0;//默认红灯
int Traffic_Jam=0;//默认畅通
int End=0;
BYTE remote_frame_data[REMOTE_FRAME_LENGTH];
BYTE remote_frame_data_send[REMOTE_FRAME_LENGTH];
BYTE g_device_NO = WIFI_ADDRESS_CAR_1;
BYTE g_device_NO_Hex;/* 设备号 即WiFi地址 */
BYTE des;
WORD cmd_WIFI;
WORD sending_data;
BYTE waiting_for_response=0;
int  lost_data=0;
extern int place[4];
extern int used;
extern int Emergency;
extern int Hold_a;
extern int bz;
extern int velocity;
int sending_waiter=0;
//********************2017赛季参数******************************************
//********************dby 修改*********************************************
int right_turn=0;
int left_turn=0;
int straight_drive=0;
int car_stop=0;
int car_go_back=0;
int speed_change=0;
int car_turn_around=0;
int uturn=0;
int target_lost=0;
int message_received=0;
int target_access=0;
int target_near=0;
int barrier_left_detected=0;
int barrier_right_detected=0;
int stuck1=0,stuck2=0;//1避障过程   2绕圈
int barrier_offset=0;
double light_offset=0;
int yanshi=0;
int count=0;
int message_num1=0;
int test_pid=0;
int test_helm=0;
int bz=0;
int leftbackturn=0,rightbackturn=0;
unsigned long angle=0;
unsigned long Distanz=0;
int x,y,z=0;
int light_goout=0;

BYTE light_distance[4]={0,0,0,0};//收到WiFi距离记录
BYTE light_angle[4]={0,0,0,0};//收到WiFi角度记录
BYTE light_angle1[10]={0,0,0,0,0,0,0,0,0,0};//收到WiFi角度记录1
BYTE light_distance1[10]={0,0,0,0,0,0,0,0,0,0};//收到WiFi距离记录1
extern get_ss;

short high8;
short low8;
short high9;
short low9;
short steer_rate=0;
short angle_rate=0;
short speed_number=0;
/*-----------------------------------------------------------------------*/
/* 执行远程命令                                                          */
/*-----------------------------------------------------------------------*/
void execute_remote_cmd(const BYTE *data)
{
	return;
}
int change_hex_into_dec(BYTE rev)
{
	int o=rev;
	if(rev>0x7F)
	{
		o=o-256;
		
	}
	return o;
}
/*-----------------------------------------------------------------------*/
/* 接受远程数据帧                                                        */
/* 第二版                                                                */
/* 修改地址位定义:源地址(1B)+目的地址(1B)                                */
/* AA BB 源地址(1B) 目的地址(1B) 长度(1B) 数据体 校验字(1B)              */
/* 未改变原数据帧的大部分定义及长度                                      */
/*-----------------------------------------------------------------------*/
int rev_remote_frame_2(BYTE rev)
{
	if (g_remote_frame_cnt == 0)	//接收帧头
	{
		if (rev == 0xAA)
		{
			remote_frame_data[g_remote_frame_cnt++] = 0xAA;
		}
	}
	else if (g_remote_frame_cnt == 1)	//接收帧头
	{
		if (rev == 0xBB)
		{
			remote_frame_data[g_remote_frame_cnt++] = 0xBB;
		}
		else
		{
			g_remote_frame_cnt = 0;
		}
	}
	else if (g_remote_frame_cnt == 2)	//接收源地址
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
	}
	else if (g_remote_frame_cnt == 3)	//接收目的地址
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
		if (rev != g_device_NO && rev != 0xFF && rev != 0xEE)
		{
			g_remote_frame_cnt = 0;	//不是发给本机的
		}
	}
	else if (g_remote_frame_cnt == 4)	//接收长度
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
		if (rev+6>REMOTE_FRAME_LENGTH)	//判断是否会导致缓冲区溢出
		{
			g_remote_frame_cnt = 0;
		}
	}
	else if (g_remote_frame_cnt>4 && g_remote_frame_cnt<=remote_frame_data[4]+4)	//接收数据区
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
	}
	else if (g_remote_frame_cnt==remote_frame_data[4]+4+1)	//接收校验字节	
	{
		BYTE sum;
		remote_frame_data[g_remote_frame_cnt++] = rev;
		sum = check_sum((const BYTE *)(remote_frame_data+2), (WORD)(remote_frame_data[4]+3));
		if (sum != remote_frame_data[remote_frame_data[4]+5])
		{
			g_remote_frame_cnt = 0;	//CheckSum Fail
		}
		else
		{
			g_remote_frame_cnt = 0;
			g_remote_frame_state = REMOTE_FRAME_STATE_OK;	//CheckSum Success
		}
	}
	return g_remote_frame_state;
}


/*-----------------------------------------------------------------------*/
/* 生成并发送远程命令                                                    */
/* 第二版                                                                */
/* 修改地址位定义:源地址(1B)+目的地址(1B)                                */
/* AA BB 源地址(1B) 目的地址(1B) 长度(1B) 数据体 校验字(1B)              */
/* 未改变原数据帧的大部分定义及长度                                      */
/* 参数 : scr 源地址                                                     */
/*        des 目标地址                                                   */
/*        cmd WiFi命令字                                                 */
/*        length data长度                                                */
/*        data发出的数据体，接在cmd后                                    */
/*-----------------------------------------------------------------------*/
void generate_remote_frame_2(BYTE scr, BYTE des, WORD cmd, BYTE length, const BYTE data[])
{
	WORD i = 0, j = 0;
	remote_frame_data_send[i++] = 0xAA;
	remote_frame_data_send[i++] = 0xBB;
	remote_frame_data_send[i++] = scr;
	remote_frame_data_send[i++] = des;
	remote_frame_data_send[i++] = length+2;
	remote_frame_data_send[i++] = (BYTE)(cmd>>8);
	remote_frame_data_send[i++] = (BYTE)cmd;
	for (j = 0; j < length; j++)
	{
		remote_frame_data_send[i++] = data[j];
	}
	remote_frame_data_send[i++] = check_sum(remote_frame_data_send+2, i-2);
	for (; i < REMOTE_FRAME_LENGTH; i++)	/* 清空未使用区域 */
	{
		remote_frame_data_send[i] = 0x00;
	}
	serial_port_0_TX_array(remote_frame_data_send, REMOTE_FRAME_LENGTH);
}


/*-----------------------------------------------------------------------*/
/* 发送赛场网络控制命令                                                          */
/* 依赖generate_remote_frame_2()                                          */
/*-----------------------------------------------------------------------*/
void send_net_cmd(BYTE des, WORD net_cmd)
{
	generate_remote_frame_2(g_device_NO, des, WIFI_CMD_NET, sizeof(net_cmd), (const BYTE *)(&net_cmd));
}


/*-----------------------------------------------------------------------*/
/* 异或校验                                                                            */
/*-----------------------------------------------------------------------*/
BYTE check_sum(const BYTE *data, WORD length)
{
	int i;
	BYTE res = 0;
	
	for (i=0; i<length; i++)
	{
		res ^= data[i];
	}
	
	return res;
}


/*-----------------------------------------------------------------------*/
/* 报告在线                                                                            */
/*-----------------------------------------------------------------------*/
void report_online(void)
{
return;
}


/*-----------------------------------------------------------------------*/
/* 发送远程请求的数据                                                             */
/*-----------------------------------------------------------------------*/
void send_remote_request_data(void)
{
	return;
}
// 以下部分非必须程序，用于2016赛季应答机制//
//*********************************************************************************
//  打包程序 ，用来使程序可读性更好
//*********************************************************************************
void sending_service_package (BYTE toward, WORD cmd, WORD data)
{  
	des=toward;
	cmd_WIFI=cmd;
	sending_data=data;
	main_wifi_sender();
}

//*********************************************************************************
//  主发送程序                 输入： 发送所需的数据      输出： 1 串口发送      2  waiting位     3 串口发送备份给备发送程序    4 发送丢失数
//*********************************************************************************
void main_wifi_sender (void)
{  

//	***********如果依然在等待回复，放弃上一个发送的等待，并且lostdata数加一***************
	if (waiting_for_response==1)
	{
	   lost_data++;
	   waiting_for_response=0;
	}
//	***********发送函数主体***************	                                    
	generate_remote_frame_2(g_device_NO_Hex, des, cmd_WIFI, 2, (const BYTE *)(&sending_data));
//  ***********等待回复位置1*************** 
	waiting_for_response=1;
	have_responsed=0;  
	sending_waiter=0;
}

//*********************************************************************************
//  应答检查程序               定时检查发送的数据是否得到了应答，若未，则使用辅助发送程序再次发送。 直到收到应答或有新的程序要发数据。
//*********************************************************************************
#if 0
void wifi_sender_checker (void)
{ 
	if (sending_waiter<5)
	{
		return;
	}
	else
	{
		if (waiting_for_response==1)
		{
			if (have_responsed==1)
			{
				waiting_for_response=0;
			}
			else if (have_responsed==0)
			{
				ancillary_wifi_sender ();
			}
		}
	}
}
#endif
//*********************************************************************************
//  辅助发送程序                 输入： 如果未应答，再发送数据      输出：  串口发送    
//*********************************************************************************
void ancillary_wifi_sender (void)
{                                      		    
	generate_remote_frame_2(g_device_NO_Hex, des, cmd_WIFI, 2, (const BYTE *)(&sending_data));
}



void Wifi_Ctrl()
{
	if(remote_frame_data[2]==0x55)//来自dby的电脑
	{
		
		int m=change_hex_into_dec(remote_frame_data[7]);
		int distance_delta1,angle_delta1,light_delta3=0;
		WORD distance=(WORD)(remote_frame_data[8]);	
		int i=0;
		int ave_dis=0,ave_lig=0,above_num=0;
		
		Distanz=0;

//		light_delta1=ABS(light_distance[0]-light_distance[1]);
//		light_delta2=ABS(light_distance[1]-light_distance[2]);		
		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x66)//右转
		{
			test_helm=1;
			high8=((WORD)(remote_frame_data[7])<<8);
			low8=(WORD)(remote_frame_data[8]);
			steer_rate=(high8|low8);
		}
//		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x77)//直走
//		{
//			straight_drive=1;
//		}
//		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x00)//停车
//		{
//			car_stop=1;
//		}
//		
//		if(remote_frame_data[5]==0x00&&remote_frame_data[6]==0x33)//倒车_李秋键
//		{
//			car_go_back=1;
//		}
//		if(remote_frame_data[5]==0x00&&remote_frame_data[6]==0x99)//调头_李秋键
//		{
//			car_turn_around=1;
//		}
//		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x55)//左转
//		{
//			left_turn=1;
//			high8=((WORD)(remote_frame_data[7])<<8);
//			low8=(WORD)(remote_frame_data[8]);
//			steer_rate=(high8|low8);	
//		}
		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x88)//变速
		{
			
			speed_change=1;
			high9=((WORD)(remote_frame_data[7])<<8);
			low9=(WORD)(remote_frame_data[8]);
			speed_number=(high9|low9);
		}
		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x11)//给角度zhaodeng
		{
			//double adj_rate=asin(0.5/distance)*256/360;
			
			
			barrier_left_detected=0;
			barrier_right_detected=0;
			barrier_offset=0;
			light_offset=0;
			stuck1=0;
			message_num1++;
			Distanz=distance;
			
			light_distance[0]=light_distance[1];light_distance[1]=light_distance[2];light_distance[2]=light_distance[3];
			light_distance[3]=distance;
//			distance_delta1=ABS(light_distance[2]-light_distance[3]);
			
//			for(i=0;i<9;i++)
//			{
//				light_distance1[i]=light_distance1[i+1];
//				light_angle1[i]=light_angle1[i+1];
//				ave_dis+=light_distance1[i];
//				ave_lig+=light_angle1[i];
//			}
//			
//			light_distance1[9]=distance;
//			light_angle1[9]=m;
//			ave_dis=(ave_dis+light_distance1[9])/10;
//			ave_lig=(ave_lig+light_angle1[9])/10;
//			if(message_num1=10)
//			{
//				message_num1=0;				
//				for(i=0;i<10;i++)
//				{
//					if(ABS(ave_dis-light_distance1[i])<1&&ABS(ave_lig-light_angle1[i])<4)
//					{
//						above_num++;
//					}
//				}				
//			}
//			if(above_num>8)
//			{				
//				stuck2=20;
//			}
//			above_num=0;
			
			if(light_distance[3]-light_distance[2]>4)
			{
				light_goout=1;
			}
//			if(m>=0)
//			{
//					m=m-adj_rate;
//			}
//			else
//			{
//					m=m+adj_rate;
//			}
//			
//			
			angle=m;
			
			
//			light_angle[0]=light_angle[1];light_angle[1]=light_angle[2];light_angle[2]=light_angle[3];
//			light_angle[3]=m;
//			angle_delta1=ABS(light_angle[2]-light_angle[3]);
			if(m>=0&&m<64)
			{

				left_turn=1;
				x=0;
				y=0;
			    //high8=(0x00<<8);
			    steer_rate=m;
			}
			if(m<0&&m>=-64)
			{
				right_turn=1;
				x=0;
				y=0;
				//high8=(0x00<<8);
				steer_rate=-1*m;
			}
			if(m>=64&&m<=127-27*x||m>=-128*y&&m<-100*y)
			{
				left_turn=1;
				y=1;
//				high8=(0x00<<8);
			//   low8=(WORD)((WORD)(remote_frame_data[7]));
				if(m<0)
				{
					angle=127;
				}
				leftbackturn=1;
				
			}
			if(m>=-128+y*28&&m<-64||m<=127*x&&m>100*x)
			{
				right_turn=1;
				x=1;
         	//	high8=(0x00<<8);
			//  low8=(WORD)((WORD)(remote_frame_data[7]));
				if(m>0)
				{
					angle=-128;
				}
				rightbackturn=1;
			}
//			if(m>=-128&&m<=-86||m<=127&&m>=85)
//			{
//				yanshi=1;
//			}
//			if(distance<=7&&distance>5)
//			{
//				light_offset=0.3;
//			}
//			if(distance<=12&&distance>6)
//			{
//				light_offset=1;
//			}
//			if(remote_frame_data[7]>=0x40&&remote_frame_data[7]<0xC0)//如果信标不在车前半部分范围内时执行
//			{
//				car_turn_around=1;
//			}
//			if(remote_frame_data[8]<0x02)
//			{
//				target_near=1;
//			}
			if(distance<8)
			{
				target_access=1;
			}
//			if(distance_delta1<4&&angle_delta1<6)
//			{
//				stuck2++;
//			}
//			if(distance_delta1>=4||angle_delta1>=6)
//			{
//				count++;
//				//stuck2=0;
//			}
//			if(count>4)
//			{
//				count=0;
//				stuck2=0;
//			}
			
			message_received=1;				
		}		
		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x22)//丢失
		{
			//barrier_offset=0;
			target_lost++;
		}
		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x44)//避障
		{
			light_offset=0;
			bz=1;
			//if(m>0-10*barrier_offset&&m<30&&remote_frame_data[8]<0x78)
			if(m>=0+5*barrier_offset)			
			{
				barrier_left_detected=1;
				barrier_offset=-1;
				//velocity=175;
				//low8=120-(WORD)(remote_frame_data[8]);
//				if(m>=-25&&m<=25)
//				{
//					angle_rate=50;
//				}
//				angle=-100;
//					
//				if(angle_rate>60)
//				{
//					angle_rate=60;
//				}
			}
			//if(m<0-10*barrier_offset&&m>-30&&remote_frame_data[8]<0x78)
			//else if(m)
			else
			{
				barrier_right_detected=1;
				barrier_offset=1;
				//low8=120-(WORD)(remote_frame_data[8]);
//				if(m>=-25&&m<=25)
//				{
//				   angle_rate=60;
//				}
//				angle=100;
			}
//			if(remote_frame_data[8]<0x50)
//			{
//				stuck1++;
//			}
			
			message_received=1;
		}
//		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x33)//信标灯避障
//		{
//			if(m>=0+3*barrier_offset)			
//			{
//				barrier_left_detected=1;
//			    barrier_offset=-1;
//                if(m>=-25&&m<=25)
//				{
//					angle_rate=50;
//				}				
//			}
//			else
//			{
//				barrier_right_detected=1;
//				barrier_offset=1;
//				if(m>=-25&&m<=25)
//				{
//					angle_rate=50;
//				}
//			}
//		}
		if(remote_frame_data[5]==0xAA)//测试pid
		{
			remote_frame_data[6]=remote_frame_data[6]/10;
			remote_frame_data[7]=remote_frame_data[7]/10;
			remote_frame_data[8]=remote_frame_data[8]/10;
//			set_speed_KP((WORD)(remote_frame_data[7]));
//			set_speed_KI((WORD)(remote_frame_data[8]));
			//set_speed_KD((WORD)(remote_frame_data[8]));
			test_pid=1;
			
		}
		if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0xBB)//避障
		{
			stuck2=1;
		}
	}
	//if(remote_frame_data[2]==0x02)//来自dby的自制上位机
	//{
	//	if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x66)
	//	{
	//		right_turn=1;
	//	}
	//	if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x77)
	//	{
	//		straight_drive=1;
	//	}
	//	if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x00)
	//	{
	//		car_stop=1;
	//	}
	//}


}
void Wifi_Test(void)
{
	if(remote_frame_data[5]==0x00 && remote_frame_data[6]==0x66)//右转
	{
		test_helm=1;
		high8=((WORD)(remote_frame_data[7])<<8);
		low8=(WORD)(remote_frame_data[8]);
		steer_rate=(high8|low8);
	}
	
}


