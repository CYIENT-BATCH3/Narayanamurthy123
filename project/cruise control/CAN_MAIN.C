/*
 * File:   CAN_MAIN.C
 * Author: sk69648
 *
 * Created on September 27, 2022, 12:07 PM
 */


#include"CAN_COMM.h"                                    
//#define SPEED 60
//#define RANGE 10

char cc_nrcs_stat = 0;                                                          //variable to store status of the NRCS fault
char cc_switch_stat = 0;                                                        //variable to store status of the cc switch status
char speed_cmd = 0;                                                             //variable to store the speed cmd uses 2 bits so 4 values will be there
char warning_lamp_status  = 0;                                                  //variable to store the warning lamp status
char nrcs_data = 0;                                                             //variable to store the NRCS data coming from the BUS with id 0X22F
char fault_counter = 0;                                                         //variable to store the no.of fault signal come from the NRCS msg
char current_speed = 0;                                                         //variable to store current speed feed through NRCS msg byte 4 bit 0 and 1
char speed_cmd_status = 0;                                                      //variable to store the speed cmd status
char range_stat = 0;                                                            //variable to store the range status
char byte1 = 0;                                                                 //byte val for 1st byte of transmitting msg contain speed cmd
char byte2 = 0;                                                                 //byte val for 2nd byte of transmitting msg contain cc on/off stat and nrcs stat
char byte3 = 0;                                                                 //byte val for 3rd byte of transmitting msg contain warning lamp stat


void main(void) {
    init_inp_out();                                                             //init all the hardware input and output
   
    sys_init();                                                                   //intiaising all intrpt
    can_init();                                                                    //init all pins used in can protocol
    set_baudrate();                                                                 //set baudrate to 250 kbps
    set_mask();                                                                     //set mask to 00
    set_filter();                                                                   //set filter to 00
   
    while(1)                                                                        //infinite loop
    {   
        cc_led_change();                                                        //call led status change for cc function
        change_err_stat_nLED();                                                 //call led status and error status chaange whenever a error occured
        
        if(cc_switch_stat)                                                      //if cc is on transmit the data according with the nrcs and speed
        {
          
            //find_distance_from_nrcs();
            //find_speed_cmd();
            set_byte_transmission();                                            //change value for the byte1,byte2,byte3 according to the status
        }
        else
        {
            cc_nrcs_stat =0;                                                    // default transmission condition every flag is 0
            speed_cmd = 0;                                                      // 
            warning_lamp_status=0;
            nrcs_data = 0;
            current_speed = 0;                                                   //variable to store vspeed into 0
            fault_counter=0;                                                     //fault counter to 0
            byte1 = 0;                                                           //every byte is zero
            byte2 = 0;                                                          //every byte is zero
            byte3 = 0;                                                          //every byte is zero                
        }
         can_write(byte1,byte2,byte3);                                          //write each byte into can bus 
    }
   
    
    return;
}



void cc_led_change(void)
{
     /* turning on cruize control while pressing switch and turning off while again pressing switch or by pressing break */
        if(PORTBbits.RB0 == 0 && PORTBbits.RB1 == 1)                            //comparing cc on pushbutton and break push button
        {
            cc_switch_stat = !cc_switch_stat;                                   //toggle the switch status
            __delay_ms(200);                                                    //de bouncing time
        }
        if(PORTBbits.RB1 == 0 && cc_switch_stat == 1)           //check break condition  awhenever  cruize control is on
        {
            cc_switch_stat = !cc_switch_stat;                   //toggle ss_status
             __delay_ms(200);                                   //de bouncing time
        }
        
        PORTCbits.RC0 = cc_switch_stat;             //cruize control led assighned with cruze status
}



void change_err_stat_nLED(void)
{
    /* checking for fault values coming from the nrcs data and enable fault led and nrcs fault status */
 if(cc_switch_stat ==1 && fault_counter ==1)            //check weather when cc is on and fault data is coming or not
        {
            cc_nrcs_stat = 1;                           //if error is occured then change nrcs error status
            PORTCbits.RC1 = cc_nrcs_stat;               //change the fault led
            
        }
        else 
        {
            cc_nrcs_stat = 0;                           //else clear nrcs statsus turn of led
           PORTCbits.RC1 = cc_nrcs_stat;
            
        }
        warning_lamp_status = cc_nrcs_stat;             //assighn the nrcs fault statsu to warning lamp status
}

void init_inp_out(void)
{
    /* TRISCbits.RC5 = 0;             //for trouble shooting purpose 
     TRISCbits.RC6 = 0; 
     PORTCbits.RC5 = 0; 
     PORTCbits.RC6 = 0;
     TRISCbits.RC7 = 0;
     PORTCbits.RC7 = 0;
     */
     
    TRISBbits.RB0 = 1;                      //cc pushbutton in RB0
    TRISBbits.RB1 = 1;                      //break pushbutton in RB1
    TRISCbits.RC0 = 0;                      //CC ON or OFF LED
    TRISCbits.RC1 = 0;                      //fault LED
    INTCON2bits.RBPU = 0;                   //enable pullup resistors in PORT B
    PORTCbits.RC0 = 0;                      //SET THE INTIAL CONDITION FOR THE LEDS
    PORTCbits.RC1 = 0;
    
}

void nrcs_data_extract(void)                   
{
    /* extract the nrcs data from byte 3 0 1 2 3 bits */
    nrcs_data = data_byte[2];                           //assigh the 3 rd index of array to variable
    nrcs_data = nrcs_data & 0x0f;                       //extract the last 4 nibble for the nrcs data
    
    
    current_speed = data_byte[3];                       //assighn 4th bit to variable
    current_speed = current_speed & 0x03;               //extract last 2  bits to current speed
    
    find_fault_data();                                  //call fault data function
    find_distance_from_nrcs();                          //call distance from nrcs function
    find_speed_cmd();                                   //call speed cmd function
}


void find_fault_data(void)                              //find the data coming is faulty or not
{
    if (nrcs_data == 0x00 || nrcs_data == 0x0f)         //if thr data is 00 or 0f it is considered as faulty
    {
        fault_counter =1;                               //then change the status of fault counter to 1      
    }
    else
        fault_counter =0;                               //else fault counter is 0
}

void find_distance_from_nrcs(void)                                      //function to seperate the 16 data coming in 4 bits as nrcs data to 3 catagories
{      
           /* 
            * 0x01 0x02 0x03 0x04       ---- low range
            * 0x05 0x06 0x07 0x08       ---- medium range
            * 0x09 0x0a 0x0b 0x0c 0x0d  ---- high range
            * 
            */
  
   {
       if(nrcs_data == 0x01 || nrcs_data == 0x02 || nrcs_data == 0x03 || nrcs_data == 0x04)     //when data belongs to 0x01-0x04 then change status of the rangestatus into1
           range_stat = 1;                  //low
       else if (nrcs_data == 0x05 || nrcs_data == 0x06 || nrcs_data == 0x07 || nrcs_data == 0x08)           //when data belongs to 0x04-0x08 then change staatsu of the range status into 2
           range_stat = 2;                  //medium
       else if (nrcs_data == 0x09 || nrcs_data == 0x0a || nrcs_data == 0x0b || nrcs_data == 0x0c || nrcs_data == 0x0d || nrcs_data == 0x0e)         //when data belongs to 0x08-0x0e then change staatsu of the range status into3
           range_stat = 3;                  //high  means no vehicle in range 
                   
   }
}
void find_speed_cmd(void)                                               //according to the range stat and current speed the cc will create the sequence for controlling the speed of the car 0000 - 1010 data
{
         /* 
          * 
          * speed
          * 0x00  -- 40
          * 0x01  -- 50
          * 0x02  -- 70
          * 0x03  -- 90
          * 
          * 
          */
  
   if(range_stat == 1 && current_speed == 0x00)
       speed_cmd_status = 0x00;                         //range is low speed is low/below 
   else if(range_stat == 1 && current_speed == 0x01)
       speed_cmd_status = 0x01;
   else if(range_stat == 1 && current_speed == 0x02)
       speed_cmd_status = 0x02;
   else if(range_stat == 1 && current_speed == 0x03)
       speed_cmd_status = 0x03;
   else if(range_stat == 2 && current_speed == 0x00)
       speed_cmd_status = 0x04;
   else if(range_stat == 2 && current_speed == 0x01)
       speed_cmd_status = 0x05;
   else if(range_stat == 2 && current_speed == 0x02)
       speed_cmd_status = 0x06;
   else if(range_stat == 2 && current_speed == 0x03)
       speed_cmd_status = 0x07;
   else if(range_stat == 3 && current_speed == 0x00)
       speed_cmd_status = 0x08;
   else if(range_stat == 3 && current_speed == 0x01)
       speed_cmd_status = 0x09;
   else if(range_stat == 3 && current_speed == 0x02)
       speed_cmd_status = 0x0a;
   else if(range_stat == 3 && current_speed == 0x03)
       speed_cmd_status = 0x0b;  
}

void set_byte_transmission()                    //set all byte for transmission
{
    byte1 = speed_cmd_status;                   //set byte 1 with the speed commend statsu
    byte2 = (cc_switch_stat << 4) | (cc_nrcs_stat);     //set byte 2 4th bit with cc on off stat byte 0 with cc_nrcs_fault_stat
    byte3 = warning_lamp_status;                        //set 3rd byte with warning lamp status in 0th bit
}
