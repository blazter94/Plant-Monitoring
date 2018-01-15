#include "mbed.h"
#include "DS18B20.h"
#include "OneWireDefs.h"
#include "TSL2561.h"
#include "DHT22.h"
#include "bold_font.h"
#include "standard_font.h"
#include "ssd1306.h"

#define THERMOMETER DS18B20

Serial serie(SERIAL_TX, SERIAL_RX);
TSL2561 LUM(PA_10, PA_9);
DHT22 Tem_Hum_A(PA_11); // Température & Humidité air
AnalogIn Humidite(PA_0);
SSD1306 ecran(D3 /* cs */, D6/* reset */, A5 /* dc */, A4 /* clock */, D2 /* data */);
//Serial PC(PA_2, PA_3);
Serial sigfox(PA_2, PA_3);
DigitalOut myled(LED1);

int i, Hum_sol, Temp_sol, Lum, Temp_air, Hum_air;

int main()
{   

    ecran.initialise();
    ecran.clear();
    ecran.set_contrast(255);
    ecran.set_font(bold_font, 8);
    
    serie.printf("New Program \r\n");
     // device( crcOn, useAddress, parasitic, mbed pin )
    THERMOMETER device(true, true, false, PA_8);
    
    while (!device.initialize());    // keep calling until it works
    
    while (true)
    {   
        myled = !myled;
        Tem_Hum_A.sample();
        device.readTemperature(); 
        Temp_air = (int)(Tem_Hum_A.getTemperature()/10);
        Hum_air = (int) (Tem_Hum_A.getHumidity()/10);
        Hum_sol = (int)(Humidite.read()* 100) ;
        Temp_sol = (int)device.readTemperature();
        //TSL2561 LUM(PB_7, PB_6); // Luminosité (I2C:SDA,SCL)
        Lum = (int) LUM.lux();
        printf("Temperature air: %d\n\r", Tem_Hum_A.getTemperature()/10);
        printf("Hum air: %d\n\r", Tem_Hum_A.getHumidity()/10);
        serie.printf("\rLum: %f \r\n", LUM.lux());
        serie.printf("Temperature sol: %d \n\r",Temp_sol);
        serie.printf("Hum sol: %f \n\r",Humidite.read()*100);
        printf("------------------------------------------------------------\n\r");
        sigfox.printf("AT$SS=%02x %02x %02x %02x %02x \r\n", Temp_sol, Temp_air, Lum, Hum_sol, Hum_air);
        
        //OLED
        ecran.set_font(bold_font, 8);
        ecran.printf("Valeurs");
        ecran.printf("\r\n"); 
        ecran.update();
        ecran.set_font(standard_font, 6);
        ecran.printf("Temp air = %d",Temp_air );
        ecran.printf("\r\n");
        ecran.printf("Hum air = %d", Tem_Hum_A.getHumidity()/10);
        ecran.printf("\r\n");
        ecran.printf("Lum = %f", LUM.lux());
        ecran.printf("\r\n");
        ecran.printf("Temp sol = %d",Temp_sol );
        ecran.printf("\r\n");
        ecran.printf("Hum sol = %f", Humidite.read()*100);
        ecran.printf("\r\n");
        ecran.update();
        ecran.clear();
        
        
        
        wait(10);
    }
    return EXIT_SUCCESS;
}