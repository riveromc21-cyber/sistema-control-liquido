#include <driverlib.h>
unsigned char boton=0;
int main(void) {


    // Stop watchdog timer
     WDT_A_hold(WDT_A_BASE);
    P1DIR|=BIT0;    //P1.0--->encedido/apagado de motor de tanque
    P1SEL0 &=~ BIT0;
    P1SEL1 &= ~BIT0;
    P1OUT &= ~BIT0;
    P1DIR|=BIT1;    //P1.0--->encedido/apagado de motor de banda
    P1SEL0 &=~ BIT1;
    P1SEL1 &= ~BIT1;
    P1OUT &= ~BIT1;

    P2DIR&= ~BIT0; //P2.0 = SENSOR1 
    P2DIR&= ~BIT1; //P2.1 = SENSOR2
    P2DIR&= ~BIT2; //P2.2 = SENSOR3----TANQUE 2
    P2DIR&= ~BIT6; //P2.6 = SENSOR4
    P2DIR&= ~BIT4; //P2.4 = SENSOR5
    P2DIR&= ~BIT5; //P2.5 = BOTON ARRANQUE/PARO
    P2SEL0 &=~ (BIT0+BIT1+BIT2+BIT6+BIT4+BIT5);
    P2SEL1 &= ~(BIT0+BIT1+BIT2+BIT6+BIT4+BIT5);
    P2REN|=(BIT0+BIT1+BIT2+BIT6+BIT4+BIT5);
    P2OUT&=~(BIT0+BIT1+BIT2+BIT6+BIT4+BIT5);

    PMM_unlockLPM5();
    P2IES &=~(BIT6+BIT4+BIT5); //INTERRUPCION CON FRANCO DE SUBIRDA
    P2IE|=(BIT6+BIT4+BIT5); //PERMISO LOCAL PARA INTERRUPCION
    P2IFG=0X00; 
    __bis_SR_register(GIE); //PERMISO GLOBAL 
      while(1)
    {

       if ((P2IN&BIT2)==0) P1OUT&=~BIT0;
        else{
        while (((P2IN&BIT0)==BIT0)&&((P2IN&BIT2)==BIT2)) P1OUT &=~BIT0;
        while (((P2IN&BIT1)==0)&&((P2IN&BIT2)==BIT2))P1OUT|=BIT0;
        }
    }
}
//VECTOR DE INTERRUPCION
#pragma vector=PORT2_VECTOR
__interrupt void banda(void)
{
    if((P2IFG&BIT5)==BIT5&&boton==0)
    {
        boton=1;
        P1OUT|=BIT1; 
    }
    else if(((P2IFG & BIT5)==BIT5)&&boton==1)
    {
    boton = 0;       
    P1OUT &= ~BIT1;  // Apaga banda
    }
    if(((P2IFG&BIT6)==BIT6)&&boton==1)P1OUT&=~BIT1;
    if(((P2IFG&BIT4)==BIT4)&&boton==1)P1OUT|=BIT1;
    P2IFG=0X00; //BORRADO DE BANDERAS
}

