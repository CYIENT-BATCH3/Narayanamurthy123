/*
 this files contain all the function defnition for the can configuration read and write in can bus
 */



#include"CAN_COMM.h"            //include all the function prototype and macros defined in header file

void sys_init()
{
    GIE = ENABLE;               //enable global interrupt
    PEIE = ENABLE;              //enable peripheral interrupt
    PIE3bits.RXB0IE =ENABLE;    //intilially can interrupt enable for recieve buffer 0 
    IPR3bits.RXB0IP=ENABLE;     //disable  priority setting for receive buffer 0
    
}

void can_init()
{
    TRISBbits.RB3 = ENABLE;      //init the can pins for the transmission and recieve
    TRISBbits.RB2 = DISABLE;
}


void set_baudrate()             //set the baud rate to 250 kbps
{
   
    set_mode(CONFIG_MODE);      //make the mode to config mode
    get_mode(CONFIG_MODE);       //check weather it is in config mode
    BRGCON1 = 0XC1;                 //configure the baudrate to 250 kbps using BRGCON1 BRGCON2 BRGCON3
    BRGCON2 = 0XAE;
    BRGCON3 = 0X45;
    CIOCON = 0X20;
    set_mode(NORMAL_MODE_TRANS);      //change back to normal mode for transmission 
}

void set_mode(char mode)
{
   CANCON = mode;               //set the mode according to argumnent passed 
}

void get_mode(char mode)
{
    while(CANSTAT != mode);      //check the mode and the set operation is succesfull or not
}



/*---------------- receiver code ----------------------------------------------*/
void set_mask()                 //set mask to 0x00 to recieve all message id    
{
    RXM0SIDH = 0x00;
    RXM0SIDL = 0x00;
}

void set_filter()                //set all filter to 0x00 to recieve all msg id
{
    RXF0SIDH = 0x00;
    RXF0SIDL = 0x00;
    RXF1SIDH = 0x00;
    RXF1SIDL = 0x00;
    
    //RXB0CON = 0x00;     //check at compilation
    
}

void __interrupt() ISR1()               //interrupt service routing whenever any interrupt occur the controller itself going to call this routine
{
    
    if(PIR3bits.RXB0IF)                 //check for can interrupt only
    {
        //PORTCbits.RC7 = 1;
         //PORTBbits.RB0 = 1;             
        PIR3bits.RXB0IF = DISABLE;      //make interrpt as clear for recieveing next data
        can_read();                      //call can read fn when ever 
        
    }
}

void can_read()
{
    set_mode(NORMAL_MODE_RECEIV);               //set to normal mode and recieve on buffer 0
    RXB0CON = 0x00;                             //clear the rxful bit
    
   
      data_byte[0] = RXB0D0;                    //accpet and assighn all data from buffer to array
      data_byte[1] = RXB0D1;                    //data buffers
      data_byte[2] = RXB0D2; 
      data_byte[3] = RXB0D3; 
      data_byte[4] = RXB0D4; 
      data_byte[5] = RXB0D5; 
      data_byte[6] = RXB0D6; 
      data_byte[7] = RXB0D7; 
      data_byte[8] = RXB0DLC;
      data_byte[9] = RXB0SIDL;                  //msg id low value
      data_byte[10]= RXB0SIDH;                  //msg id high val
      
    
    RXB0CONbits.RXFUL = 0;      //to clear the data in buffer for the next reception
    if(data_byte[9] == 0xE0 && data_byte[10] == 0x45)           //accepting only one data
    nrcs_data_extract();                                        //calling nrcs data extract function for extracting speed and nrcs data
    
}





void can_write(char byte_speed_cmd_1,char byte_cc_status_2,char wl_status_3)            //can write function with 3 arguments
{
    __delay_ms(200);                                                                    //sending data with 200 ms time gap
    set_mode(NORMAL_MODE_TRANS);                                                        //set into normal mode for transmission
    //PORTCbits.RC6 = 1;
     CIOCON = 0X20;                                                                     //enable can i/p o/p control enable can tx pin to vcc when recessive
    
     
     TXB0SIDH = 0x67;                   //for address 0x337 for sending data to dashbord or engine
     TXB0SIDL = 0xE0;
     TXB0DLC = 0X08;                    //data length is 8 bit
     TXB0D0  = byte_speed_cmd_1;        //1 byte contains speed command
     TXB0D1  = byte_cc_status_2;        //2 byte contains the cc turn on status and nrcs fault status
     TXB0D2  = wl_status_3;             //3rd byte contains warning lamp status
     TXB0D3  = 0x00;
     TXB0D4  = 0X00;
     TXB0D5  = 0X00;
     TXB0D6  = 0X00;
     TXB0D7  = 0X00;
     TXB0CON = 0x08;                     //set transmit request status TXREQ,for requesting to send the data after buffer assighned to corresponding values
}


