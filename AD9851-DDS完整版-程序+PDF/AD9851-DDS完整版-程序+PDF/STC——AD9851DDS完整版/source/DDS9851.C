
/**********************************************************
                       康威科技
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/
**********************************************************/

#include "DDS9851.H"

//***************************************************//
//函数1:           ad9851_reset()                    //
//函数2:           ad9851_reset_serial()             //
//函数3:           ad9851_wr_parrel(unsigned char w0,double frequence)//
//函数4:           ad9851_wr_serial(unsigned char w0,double frequence)//
//版本:              V1.1                            //
//日期：             2008/12/19                      //
//修改日期：         2008/12/19                      //
//编写者：        xyc5208@163.com)            //
//***************************************************//
//                  子程序说明                       //
//***************************************************//
//函数1:  ad9851_reset()
//        复位ad9851，之后为并口写入模式        
//函数2:  ad9851_reset_serial()
//        复位ad9851，之后为串口写入模式
//函数3:  ad9851_wr_parrel(unsigned char w0,double frequence)
//        并口写ad9851数据，w0为ad9851中w0的数据，frequence
//        为写入的频率	
//函数4:  ad9851_wr_serial(unsigned char w0,double frequence)
//        串口写ad9851数据，w0为ad9851中w0的数据，frequence
//        为写入的频率	
//需定义的位：
         //ad9851_w_clk    ;
         //ad9851_fq_up    ;
         //ad9851_rest     ;
         //ad9851_bit_data ;
//例：
         //sbit ad9851_w_clk    =P2^2;
         //sbit ad9851_fq_up    =P2^1;
         //sbit ad9851_rest     =P2^0;
         //sbit ad9851_bit_data =P1^7;
//***************************************************//
//                 写数据说明                        //
//***************************************************//
//写数据例：
//       ad9851_reset()
//       wr_lcd02_data(unsigned char x)
//       ad9851_wr_parrel(0x01,1000)
//       ad9851_wr_serial(0x01,1000)
//***************************************************//
//---------------------------------------------------//
//                   程序                            //
//---------------------------------------------------//

//***************************************************//
//              ad9851复位(并口模式)                 //
//---------------------------------------------------//
void ad9851_reset()
{
ad9851_w_clk=0;
ad9851_fq_up=0;
//rest信号
ad9851_rest=0;
ad9851_rest=1;
ad9851_rest=0;
}
//***************************************************//
//              ad9851复位(并口模式)                 //
//---------------------------------------------------//
void ad9851_reset_serial()
{
ad9851_w_clk=0;
ad9851_fq_up=0;
//rest信号
ad9851_rest=0;
ad9851_rest=1;
ad9851_rest=0;
//w_clk信号
ad9851_w_clk=0;
ad9851_w_clk=1;
ad9851_w_clk=0;
//fq_up信号
ad9851_fq_up=0;
ad9851_fq_up=1;
ad9851_fq_up=0;
}
//***************************************************//
//          向ad9851中写命令与数据(并口)             //
//---------------------------------------------------//

void ad9851_wr_parrel(unsigned char w0,double frequence)
{
unsigned char w;
long int y;
double x;
//计算频率的HEX值
x=4294967295/180;//适合180M晶振/180为最终时钟频率（或30M六倍频）
//如果时钟频率不为180MHZ，修改该处的频率值，单位MHz ！！！
frequence=frequence/1000000;
frequence=frequence*x;
y=frequence;
//写w0数据
w=w0;   
D_DATA=w;      //w0
ad9851_w_clk=1;
ad9851_w_clk=0;
//写w1数据
w=(y>>24);
D_DATA=w;      //w1
ad9851_w_clk=1;
ad9851_w_clk=0;
//写w2数据
w=(y>>16);
D_DATA=w;      //w2
ad9851_w_clk=1;
ad9851_w_clk=0;
//写w3数据
w=(y>>8);
D_DATA=w;      //w3
ad9851_w_clk=1;
ad9851_w_clk=0;
//写w4数据
w=(y>>=0);
D_DATA=w;      //w4
ad9851_w_clk=1;
ad9851_w_clk=0;
//移入始能
ad9851_fq_up=1;
ad9851_fq_up=0;
}
//***************************************************//
//          向ad9851中写命令与数据(串口)             //
//---------------------------------------------------//
void ad9851_wr_serial(unsigned char w0,double frequence)
{
unsigned char i,w;
long int y;
double x;
//计算频率的HEX值
x=4294967295/180;//适合180M晶振/180为最终时钟频率（或30M六倍频）
//如果时钟频率不为180MHZ，修改该处的频率值，单位MHz  ！！！
frequence=frequence/1000000;
frequence=frequence*x;
y=frequence;
//写w4数据
w=(y>>=0);
for(i=0;i<8;i++)
{
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//写w3数据
w=(y>>8);
for(i=0;i<8;i++)
{
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//写w2数据
w=(y>>16);
for(i=0;i<8;i++)
{
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//写w1数据
w=(y>>24);
for(i=0;i<8;i++)
{
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//写w0数据
w=w0;   
for(i=0;i<8;i++)
{
ad9851_bit_data=(w>>i)&0x01;
ad9851_w_clk=1;
ad9851_w_clk=0;
}
//移入始能
ad9851_fq_up=1;
ad9851_fq_up=0;
}


