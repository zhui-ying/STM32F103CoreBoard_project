/********************************* �����к�̫�������޹�˾ *******************************
* ʵ �� �� �����İ����ʵ��
* ʵ��˵�� ������IO����1Hz��Ƶ�ʷ���PWM��
* ʵ��ƽ̨ ����̫ARM��Ƭ��������
* ���ӷ�ʽ ��
* ע    �� ������JTAG�ļ������Ų����������PWM������ԭ����ο�GPIO���ģʽ
* ��    �� ����̫���Ӳ�Ʒ�з���    QQ ��1909197536
* ��    �� ��http://shop120013844.taobao.com/
****************************************************************************************/

#include "stm32f10x.h"

//ȫ�ֱ�������

volatile unsigned int tick_1ms = 0;//1ms����������Ϊ����Ļ���������
int sys_status=0;
int count=0;

uint16_t port_a,port_b,port_c,port_d,port_e,port_f,port_g;

void InitIO()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//ʹ��LEDʹ�õ�GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//ʹ��LEDʹ�õ�GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);//ʹ��LEDʹ�õ�GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);//ʹ��LEDʹ�õ�GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);//ʹ��LEDʹ�õ�GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF , ENABLE);//ʹ��LEDʹ�õ�GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG , ENABLE);//ʹ��LEDʹ�õ�GPIOʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOA , &GPIO_InitStructure);
	GPIO_Init(GPIOB , &GPIO_InitStructure);
	GPIO_Init(GPIOC , &GPIO_InitStructure);
	GPIO_Init(GPIOD , &GPIO_InitStructure);
	GPIO_Init(GPIOE , &GPIO_InitStructure);
	GPIO_Init(GPIOF , &GPIO_InitStructure);
	GPIO_Init(GPIOG , &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG , &GPIO_InitStructure);
	
					GPIO_SetBits(GPIOA , 0xffff);
				GPIO_SetBits(GPIOB , 0xffff);
				GPIO_SetBits(GPIOC , 0xffff);
				GPIO_SetBits(GPIOD , 0xffff);
				GPIO_SetBits(GPIOE , 0xffff);
				GPIO_SetBits(GPIOF , 0xffff);
				GPIO_SetBits(GPIOG , 0xffff);
				
	
  //GPIO_SetBits(GPIOE , GPIO_Pin_All);//�ر����еģ̣ţ�ָʾ��
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
    //��������Զ�װ�صļ���ֵ�����ڼ����Ǵ�0��ʼ�ģ�����Ϊ100us
    TIM_TimeBaseStructure.TIM_Period = (100 - 1);//10kHz
    // �������Ԥ��Ƶϵ����������Ϊ0ʱ��ʾ����Ƶ����Ҫ��1
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);//1MHz
    // �߼�Ӧ�ñ��β��漰�������ڶ�ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR,TIx)
    // ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    //���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //��ʼ����ʱ��5
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* Clear TIM5 update pending flag[���TIM5����жϱ�־] */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    /* TIM IT enable */ //������ж�
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /* TIM5 enable counter */
    TIM_Cmd(TIM2, ENABLE);  //������ʹ�ܣ���ʼ����

    /* �жϲ������� */
    NVIC_TIM2Configuration();
}

int main(void)
{
//	int flag = 0;
	int index=0;
	
	TIM2_Init();
  InitIO();
	
	sys_status = 1;//����
	tick_1ms=0;
	while(tick_1ms < 200);
	tick_1ms = 0;
	sys_status = 0;//Ϩ��
	while(tick_1ms < 200);
	tick_1ms = 0;
	sys_status = 1;//����
	while(tick_1ms < 200);
	tick_1ms = 0;	
	sys_status = 0;//Ϩ��
	tick_1ms=0;
	while(tick_1ms < 200);
	sys_status = 1;//����
	tick_1ms=0;
	while(tick_1ms < 200);
	tick_1ms = 0;
	sys_status = 0;//Ϩ��
	
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
	 
	 port_g = port_g | 0x8000;//ȥ��LED��һ��
	 port_a = port_a | 0xe000;//PA13,14,15
	 port_b = port_b | 0x0018;//PB3,PB4
		
	//��Ϊ0�ĸ���
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
		 sys_status = 0;//û�жԵض̽ӵ�
	 
	 if(count == 1)
			sys_status = 1;//��һ��������
	 
	 if(count > 1)
	 {
			sys_status = 2;//�ж�����쳣,��һ�ᣬ����һ�»���ȥ��
		 	tick_1ms=0;
	    while(tick_1ms < 500);
	 }
			
	 
 }
}

