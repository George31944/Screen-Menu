#include "screen.h"
#include "sh1106.h"  
#include "photo.h"
//	显示主界面
void page0(int icon_light)
{
	sh1106_clear();
	sh1106_draw_bitmap(7,22,gImage_tmp,24,26);		//	显示图像
	sh1106_draw_bitmap(37,22,gImage_fans,24,26);
	sh1106_draw_bitmap(67,22,gImage_music,24,26);
	if(icon_light==0)
	sh1106_draw_bitmap(97,22,gImage_light_off,24,26);
	else
	sh1106_draw_bitmap(97,22,gImage_light_on,24,26);
}
//	温度界面
void page1(float* tmp,float* hum)
{
	sh1106_clear();
	sh1106_draw_bitmap(6,2,wen,24,24);
	sh1106_draw_bitmap(30,2,du,24,24);
	sh1106_draw_bitmap(54,2,maohao,8,24);
	sh1106_draw_bitmap(84,2,xiaoshudian,8,24);
	sh1106_draw_bitmap(6,30,shi,24,24);
	sh1106_draw_bitmap(30,30,du,24,24);
	sh1106_draw_bitmap(54,30,maohao,8,24);
	sh1106_draw_bitmap(84,30,xiaoshudian,8,24);
	sh1106_draw_bitmap(52,52,gImage_back,24,14);
	page1_changetmp( tmp, hum);	
}	
void page1_changetmp(float* tmp,float* hum)		//	将温湿度打印在屏幕上
{
		int t=(int) (*tmp*100); 		//	去小数点
		int h=(int) (*hum*100);
		int x=0;
	//	打印温度
		x=t%10;
		t/=10;
			sh1106_draw_bitmap(102,2,nums[x],16,24);		//	打印百分位
		x=t%10;
		t/=10;
			sh1106_draw_bitmap(90,2,nums[x],16,24);		//	打印十分位
		x=t%10;
		t/=10;
			sh1106_draw_bitmap(72,2,nums[x],16,24);		//	打印个位
		x=t%10;
			sh1106_draw_bitmap(60,2,nums[x],16,24);		//	打印十位
			sh1106_draw_bitmap(84,2,xiaoshudian,8,24);		//	打印小数点
	//	打印湿度
		x=h%10;
		h/=10;
			sh1106_draw_bitmap(102,30,nums[x],16,24);		//	打印百分位
		x=h%10;
		h/=10;
			sh1106_draw_bitmap(90,30,nums[x],16,24);		//	打印十分位
		x=h%10;
		h/=10;
			sh1106_draw_bitmap(72,30,nums[x],16,24);		//	打印个位
		x=h%10;
			sh1106_draw_bitmap(60,30,nums[x],16,24);		//	打印十位
			sh1106_draw_bitmap(84,30,xiaoshudian,8,24);		//	打印小数点
}
void page1_erasetmp()		//	擦除原来的温度数据
{
	
			sh1106_erase_bitmap(102,2,num_clear,16,24);		//	打印百分位

			sh1106_erase_bitmap(90,2,num_clear,16,24);		//	打印十分位

			sh1106_erase_bitmap(72,2,num_clear,16,24);		//	打印个位
	
			sh1106_erase_bitmap(60,2,num_clear,16,24);		//	打印十位

			sh1106_erase_bitmap(102,30,num_clear,16,24);		//	打印百分位

			sh1106_erase_bitmap(90,30,num_clear,16,24);		//	打印十分位
			
			sh1106_erase_bitmap(72,30,num_clear,16,24);		//	打印个位
	
			sh1106_erase_bitmap(60,30,num_clear,16,24);		//	打印十位
		
}
//	风扇界面
void page2_init(int x)				
{
		sh1106_clear();
		if(x==0)
		sh1106_draw_bitmap(8,4,gImage_switch_off,24,26);
		else
		sh1106_draw_bitmap(8,4,gImage_switch_on,24,26);
		sh1106_draw_bitmap(8,34,gImage_back2,24,26);
		sh1106_draw_bitmap(40,4,gImage_speed_up,24,25);
		sh1106_draw_bitmap(40,34,gImage_speed_dawn,24,25);
		if(x==0||x==1)
		sh1106_draw_bitmap(72,8,gImage_speed1,48,49);
		else if(x==2)
		sh1106_draw_bitmap(72,8,gImage_speed2,48,49);
		else
		sh1106_draw_bitmap(72,8,gImage_speed3,48,49);
}
void page2_speed(int x)		//	改变转速图标
{
		sh1106_erase_bitmap(72,8,speed_clear,48,49);
		if(x==1||x==0)
		sh1106_draw_bitmap(72,8,gImage_speed1,48,49);
		else if(x==2)
		sh1106_draw_bitmap(72,8,gImage_speed2,48,49);
		else
		sh1106_draw_bitmap(72,8,gImage_speed3,48,49);
}
void page2_boder_erase(int x)	//	切换指示框	
{
		if(x==0)
			sh1106_erase_bitmap(8,4,bianjie2,24,26);
		else if(x==1)
			sh1106_erase_bitmap(8,34,bianjie2,24,26);
		else if(x==2)
			sh1106_erase_bitmap(40,4,bianjie2,24,26);
		else 
			sh1106_erase_bitmap(40,34,bianjie2,24,26);
}
void page2_boder_draw(int x)
{
		if(x==0)
			sh1106_draw_bitmap(8,4,bianjie2,24,26);
		else if(x==1)
			sh1106_draw_bitmap(8,34,bianjie2,24,26);
		else if(x==2)
			sh1106_draw_bitmap(40,4,bianjie2,24,26);
		else 
			sh1106_draw_bitmap(40,34,bianjie2,24,26);
}
//	电压界面
int v_num[3];			//	存储上个电压值
void page3_init(float val)		
{
		sh1106_clear();
		sh1106_draw_bitmap(10,20,dian,24,24);
		sh1106_draw_bitmap(35,20,ya,24,24);
		sh1106_draw_bitmap(69,20,maohao,8,24);
		page3_changevalue(val);
		sh1106_draw_bitmap(52,52,gImage_back,24,14);
}
void page3_changevalue(float val)		//	改变电压
{
		int t=(int)(val*100);
		int x;
	
		x=t%10;	
		v_num[0]=x;
		t/=10;	
		sh1106_draw_bitmap(110,20,nums[x],16,24);		//	打印百分位
		x=t%10;	
		v_num[1]=x;
		t/=10;
		sh1106_draw_bitmap(98,20,nums[x],16,24);		//	打印十分位
		x=t%10;	
		v_num[2]=x;
		t/=10;
		sh1106_draw_bitmap(80,20,nums[x],16,24);		//	打印个位
		sh1106_draw_bitmap(92,20,xiaoshudian,8,24);		//	打印小数点
}
void page3_erasevalue(float val)		//	擦除原来的电压数据
{
		int t=(int)(val*100);
		int x;
		x=t%10;	
		t/=10;	
		if(x!=v_num[0])
			sh1106_erase_bitmap(110,20,num_clear,16,24);		//	擦除百分位
		x=t%10;
		t/=10;
		if(x!=v_num[1])
			sh1106_erase_bitmap(98,20,num_clear,16,24);			//	擦除十分位
		x=t%10;
		t/=10;
		if(x!=v_num[0])
			sh1106_erase_bitmap(80,20,num_clear,16,24);			//	擦除个位
}
