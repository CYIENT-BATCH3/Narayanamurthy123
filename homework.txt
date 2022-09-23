#include <stdio.h>
int mobile_fun();//function prototype
int music();//function prototype
int camera_function();//defining phone function
void gps_fun();//gps function prototype
int phone_fun();//function prototype
void play();//function prototype
void stop();//function prototype
void pause();//function prototype
void current();//function prototype
void list();//function prototype
void call();//function prototype
void msg();//function prototype
void revcall();//function prototype
void picture();//function prototype
void video();//function prototype
void list_img();//function prototype
void main()
{
    mobile_fun();
    
}
int mobile_fun()
{
     int key;//declaring variable
   printf("enter a perticula function \n1.music function\n2.phone_function\n3.camera_function");
   scanf("%d",&key);
   switch(key)
   {
       case 1: music();break;//calling music function
       case 2: phone_fun();break;//calling phone function
      case 3: camera_function();break;//calling camera_function
      default : printf("enter a valid function");
   }
   
    return 0;
}



int music()//defining music function
{
     int music_key;//decalring variable
     while(1)
     {
    printf("\nplease enter a key for music \n1.play\n2.stop\n3.pause\n4.current\n5.list\n9.break\n"); 
    scanf("%d",&music_key);//reading key
    
        
        if(music_key == 1)
        {
        play();//calling play function
        }
         else if(music_key==2)
         {
             stop();//calling stop function
         }
         else if(music_key==3)
         {
             pause();//calling pause function
         }
         else if(music_key == 4)
         {
             current();//calling current function
         }
         else if(music_key == 5)
         {
             list();//calling list function
         }
        else if(music_key == 9)
         {
             break;//break from loop
         }
         else
         {
         printf("enter valid key");
    
     }
     
     }
     return mobile_fun();
}
void play()//defining play function
{
    printf("\nit's playing");
}
void stop()//defining stop function
{
    printf("\n||||stop|||");
}
void pause()//defining pause function
{
    printf("\n pause");
}
void current()//defining current function
{
    printf("\n current playing song---------");
}
void list()//defining list function
{
    printf("\n song listssssssssss");
}


int phone_fun()//defining phone function
{
     int phone_key;//decalring variable
     while(1)
     {
    printf("\nplease enter a key for phone function \n1.call\n2.msg\n3.recvcall\n9.break\n\n"); 
    scanf("%d",&phone_key);//reading key
    
        
        if(phone_key == 1)
        {
        call();//calling call function
        }
         else if(phone_key==2)
         {
             msg();//calling msg function
         }
         else if(phone_key==3)
         {
             revcall();//calling revcall function
         }
         
        else if(phone_key == 9)
         {
             break;//break from loop
         }
         else
         {
         printf("\nenter valid key");
    
     }
     
     }
     return mobile_fun();
}
void call()//defining call function
{
    printf("\nit's calling");
}
void msg()//defining msg function
{
    printf("\n||||   message    |||");
}
void revcall()//defining revcall function
{
    printf("\n revcall");
}



int camera_function()//defining camera function
{
     int camera_key;//decalring variable
     while(1)
     {
         
          printf("\nEnable gps ...............\n");
        gps_fun();//calling gps function
    printf("\nplease enter a key for camera function \n1.taking picture\n2.Record video\n3.List 10 images\n9.break\n\n"); 
    scanf("%d",&camera_key);//reading key
        if(camera_key == 1)
        {
        picture();//calling picture function
        }
         else if(camera_key==2)
         {
             video();//calling video function
         }
         else if(camera_key==3)
         {
             list_img();//calling revcall function
         }
         
        else if(camera_key == 9)
         {
             break;//break from loop
         }
         else
         {
         printf("\nenter valid key");
    
     }
     }
     return mobile_fun();
}
void picture()//defining picture function
{
    printf("\nit's taking picture");
}
void video()//defining video function
{
    printf("\n||||   recording video    |||");
}
void list_img()//defining list_img function
{
    printf("\n last 10 imagesssss");
}

void gps_fun()//defining GPS function
{
    
    printf("GPS Turned on");
}
