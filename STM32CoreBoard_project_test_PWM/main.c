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

unsigned int tick_1ms = 0;//1ms计数器，作为电机的基本计数器

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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOA , &GPIO_InitStructure);
	GPIO_Init(GPIOB , &GPIO_InitStructure);
	GPIO_Init(GPIOC , &GPIO_InitStructure);
	GPIO_Init(GPIOD , &GPIO_InitStructure);
	GPIO_Init(GPIOE , &GPIO_InitStructure);
	GPIO_Init(GPIOF , &GPIO_InitStructure);
	GPIO_Init(GPIOG , &GPIO_InitStructure);
	
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
	int flag = 0;
	TIM2_Init();
  InitIO();

 while(1)
 {	 
	 		if(tick_1ms >= 5)//500
		{
			tick_1ms = 0;
			flag = !flag;
			if(flag == 0)
			{
				GPIO_SetBits(GPIOA , 0xaaaa);
				GPIO_SetBits(GPIOB , 0xaaaa);
				GPIO_SetBits(GPIOC , 0xaaaa);
				GPIO_SetBits(GPIOD , 0xaaaa);
				GPIO_SetBits(GPIOE , 0xaaaa);
				GPIO_SetBits(GPIOF , 0xaaaa);
				GPIO_SetBits(GPIOG , 0xaaaa);
				
				GPIO_ResetBits(GPIOA , 0x5555);
				GPIO_ResetBits(GPIOB , 0x5555);
				GPIO_ResetBits(GPIOC , 0x5555);
				GPIO_ResetBits(GPIOD , 0x5555);
				GPIO_ResetBits(GPIOE , 0x5555);
				GPIO_ResetBits(GPIOF , 0x5555);		
				GPIO_ResetBits(GPIOG , 0x5555);	
				
			}else
			{
				GPIO_ResetBits(GPIOA , 0xaaaa);
				GPIO_ResetBits(GPIOB , 0xaaaa);
				GPIO_ResetBits(GPIOC , 0xaaaa);
				GPIO_ResetBits(GPIOD , 0xaaaa);
				GPIO_ResetBits(GPIOE , 0xaaaa);
				GPIO_ResetBits(GPIOF , 0xaaaa);
				GPIO_ResetBits(GPIOG , 0xaaaa);
				
				GPIO_SetBits(GPIOA , 0x5555);
				GPIO_SetBits(GPIOB , 0x5555);
				GPIO_SetBits(GPIOC , 0x5555);
				GPIO_SetBits(GPIOD , 0x5555);
				GPIO_SetBits(GPIOE , 0x5555);
				GPIO_SetBits(GPIOF , 0x5555);		
				GPIO_SetBits(GPIOG , 0x5555);				
			}
			
		}
		
 }
}

