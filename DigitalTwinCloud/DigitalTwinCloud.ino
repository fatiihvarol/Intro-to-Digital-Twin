#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>                                   //SoftwareSerial kütüphanimizi ekliyoruz.                                         //DHT11 sensör kütüphanemizi ekliyoruz.

String agAdi = "Yavuz iPhone'u";                 //Ağımızın adını buraya yazıyoruz.    
String agSifresi = "123456780";           //Ağımızın şifresini buraya yazıyoruz.

int rxPin =9;                                               //ESP8266 RX pini
int txPin = 10;                                               //ESP8266 TX pini

String ip = "184.106.153.149";                                //Thingspeak ip adresi


SoftwareSerial esp(rxPin, txPin);                             //Seri haberleşme pin ayarlarını yapıyoruz.

MPU6050 ivmesensor;
int ax,ay,az,gx,gy,gz;

//Hitec ELECTRONICS
const int trigPin = 11;
const int echoPin = 12;

const int trigPin2 = 10;
const int echoPin2 = 9;
// defines variables
long duration;
int distance;

long duration2;
int distance2;
void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin2, INPUT); // Sets the echoPin as an Input

Wire.begin();


Serial.begin(9600); // Starts the serial communication
Serial.println("Started");
  esp.begin(115200);                                          //ESP8266 ile seri haberleşmeyi başlatıyoruz.
  esp.println("AT");                                          //AT komutu ile modül kontrolünü yapıyoruz.
  Serial.println("AT Yollandı");
  while(!esp.find("OK")){                                     //Modül hazır olana kadar bekliyoruz.
    esp.println("AT");
    Serial.println("ESP8266 Bulunamadı.");
  }
  Serial.println("OK Komutu Alındı");
  esp.println("AT+CWMODE=1");                                 //ESP8266 modülünü client olarak ayarlıyoruz.
  while(!esp.find("OK")){                                     //Ayar yapılana kadar bekliyoruz.
    esp.println("AT+CWMODE=1");
    Serial.println("Ayar Yapılıyor....");
  }
  Serial.println("Client olarak ayarlandı");
  Serial.println("Aga Baglaniliyor...");
  esp.println("AT+CWJAP=\""+agAdi+"\",\""+agSifresi+"\"");    //Ağımıza bağlanıyoruz.
  while(!esp.find("OK"));                                     //Ağa bağlanana kadar bekliyoruz.
  Serial.println("Aga Baglandi.");
  delay(1000);
ivmesensor.initialize();

}
void loop() {
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(20);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(100);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance in (cm) = ");
Serial.println(distance);


// Clears the trigPin
digitalWrite(trigPin2, LOW);
delayMicroseconds(20);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin2, HIGH);
delayMicroseconds(100);
digitalWrite(trigPin2, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration2 = pulseIn(echoPin2, HIGH);
// Calculating the distance
distance2= duration2*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance2 in (cm) = ");
Serial.println(distance2);


ivmesensor.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);

Serial.print("ivme x=");
Serial.print("\t");
Serial.println(ax);

Serial.print("ivme y=");
Serial.print("\t");
Serial.println(ay);

Serial.print("ivme z=");
Serial.print("\t");
Serial.println(az);

Serial.print("gyro x=");
Serial.print("\t");
Serial.println(gx);

Serial.print("gyro y=");
Serial.print("\t");
Serial.println(gy);

Serial.print("gyro z=");
Serial.print("\t");
Serial.println(gz);

esp.println("AT+CIPSTART=\"TCP\",\""+ip+"\",80");           //Thingspeak'e bağlanıyoruz.
  if(esp.find("Error")){                                      //Bağlantı hatası kontrolü yapıyoruz.
    Serial.println("AT+CIPSTART Error");
  }

  String veri = "GET https://api.thingspeak.com/update?api_key=X4UEO3LKH9RDT1U7";   //Thingspeak komutu. Key kısmına kendi api keyimizi yazıyoruz.                                   //Göndereceğimiz sıcaklık değişkeni
  veri += "&field1=";
  veri += String(distance);                                       //Göndereceğimiz nem değişkeni
  veri += "\r\n\r\n"; 
  esp.print("AT+CIPSEND=");                                   //ESP'ye göndereceğimiz veri uzunluğunu veriyoruz.
  esp.println(veri.length()+2);
  delay(2000);
  if(esp.find(">")){                                          //ESP8266 hazır olduğunda içindeki komutlar çalışıyor.
    esp.print(veri);                                          //Veriyi gönderiyoruz.
    Serial.println(veri);
    Serial.println("Veri gonderildi.");
    delay(1000);
  }
  Serial.println("Baglantı Kapatildi.");
  esp.println("AT+CIPCLOSE");                                //Bağlantıyı kapatıyoruz
  delay(1000);





}