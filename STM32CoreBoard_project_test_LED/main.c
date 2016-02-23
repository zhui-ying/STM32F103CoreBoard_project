/********************************* 深圳市航太电子有限公司 *******************************
* 实 验 名 ：核心板测试实验
* 实验说明 ：所有IO口以1Hz的频率发出PWM波
* 实验平台 ：航太ARM单片机开发板
* 连接方式 ：
* 注    意 ：连接JTAG的几个引脚不会产生具体PWM，具体原因请参看GPIO输出模式
* 作    者 ：航太电子产品研发部    QQ ：1909197536
* 店    铺 ：http://shop120013844.taobao.com/
****************************************************************************************/

#include "stm32f10x.h"

//全局变量定义

volatile unsigned int tick_1ms = 0;//1ms计数器，作为电机的基本计数器
int sys_status=0;
int count=0;

uint16_t port_a,port_b,port_c,port_d,port_e,port_f,port_g;

void InitIO()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//使能LED使用的GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//使能LED使用的GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);//使能LED使用的GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);//使能LED使用的GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);//使能LED使用的GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF , ENABLE);//使能LED使用的GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG , ENABLE);//使能LED使用的GPIO时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOA , &GPIO_InitStructure);
	GPIO_Init(GPIOB , &GPIO_InitStructure);
	GPIO_Init(GPIOC , &GPIO_InitStructure);
	GPIO_Init(GPIOD , &GPIO_InitStructure);
	GPIO_Init(GPIOE , &GPIO_InitStructure);
	GPIO_Init(GPIOF , &GPIO_InitStructure);
	GPIO_Init(GPIOG , &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//下拉输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG , &GPIO_InitStructure);
	
					GPIO_SetBits(GPIOA , 0xffff);
				GPIO_SetBits(GPIOB , 0xffff);
				GPIO_SetBits(GPIOC , 0xffff);
				GPIO_SetBits(GPIOD , 0xffff);
				GPIO_SetBits(GPIOE , 0xffff);
				GPIO_SetBits(GPIOF , 0xffff);
				GPIO_SetBits(GPIOG , 0xffff);
				
	
  //GPIO_SetBits(GPIOE , GPIO_Pin_All);//关闭所有的ＬＥＤ指示灯
}

static void NVIC_TIM2Configuration(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM5 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* Time base configuration */
    //这个就是自动装载的计数值，由于计数是从0开始的，周期为100us
    TIM_TimeBaseStructure.TIM_Period = (100 - 1);//10kHz
    // 这个就是预分频系数，当由于为0时表示不分频所以要减1
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);//1MHz
    // 高级应用本次不涉及。定义在定时器时钟(CK_INT)频率与数字滤波器(ETR,TIx)
    // 使用的采样频率之间的分频比例
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    //向上计数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //初始化定时器5
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* Clear TIM5 update pending flag[清除TIM5溢出中断标志] */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    /* TIM IT enable */ //打开溢出中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /* TIM5 enable counter */
    TIM_Cmd(TIM2, ENABLE);  //计数器使能，开始工作

    /* 中断参数配置 */
    NVIC_TIM2Configuration();
}

int main(void)
{
//	int flag = 0;
	int index=0;
	
	TIM2_Init();
  InitIO();
	
	sys_status = 1;//点亮
	tick_1ms=0;
	while(tick_1ms < 200);
	tick_1ms = 0;
	sys_status = 0;//熄灭
	while(tick_1ms < 200);
	tick_1ms = 0;
	sys_status = 1;//点亮
	while(tick_1ms < 200);
	tick_1ms = 0;	
	sys_status = 0;//熄灭
	tick_1ms=0;
	while(tick_1ms < 200);
	sys_status = 1;//点亮
	tick_1ms=0;
	while(tick_1ms < 200);
	tick_1ms = 0;
	sys_status = 0;//熄灭
	
 while(1)
 {	 
	 count = 0;
	 port_a = GPIO_ReadInputData(GPIOA);
	 port_b = GPIO_ReadInputData(GPIOB);
	 port_c = GPIO_ReadInputData(GPIOC);
	 port_d = GPIO_ReadInputData(GPIOD);
	 port_e = GPIO_ReadInputData(GPIOE);
	 port_f = GPIO_ReadInputData(GPIOF);
	 port_g = GPIO_ReadInputData(GPIOG);
	 
	 port_g = port_g | 0x8000;//去掉LED那一个
	 port_a = port_a | 0xe000;//PA13,14,15
	 port_b = port_b | 0x0018;//PB3,PB4
		
	//数为0的个数
	 for(index = 0;index<16;index++)
	 {
			if((port_a & (0x01<<index)) == 0)
			{
				count++;
			}
			if((port_b & (0x01<<index)) == 0)
			{
				count++;
			}
			if((port_c & (0x01<<index)) == 0)
			{
				count++;
			}
			if((port_d & (0x01<<index)) == 0)
			{
				count++;
			}
			if((port_e & (0x01<<index)) == 0)
			{
				count++;
			}
			if((port_f & (0x01<<index)) == 0)
			{
				count++;
			}
			if((port_g & (0x01<<index)) == 0)
			{
				count++;
			}
	 }
	 
	 if(count == 0)
		 sys_status = 0;//没有对地短接的
	 
	 if(count == 1)
			sys_status = 1;//有一个，正常
	 
	 if(count > 1)
	 {
			sys_status = 2;//有多个，异常,闪一会，避免一下划过去了
		 	tick_1ms=0;
	    while(tick_1ms < 500);
	 }
			
	 
 }
}

