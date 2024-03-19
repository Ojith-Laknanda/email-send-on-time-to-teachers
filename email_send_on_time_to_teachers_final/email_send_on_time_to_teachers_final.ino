#include <Wire.h> // Library for I2C communication
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include "RTClib.h"


#include <ESP_Mail_Client.h>


#define WIFI_SSID "Ojiya"
#define WIFI_PASSWORD "ojith@1234"

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "abcdepro2@gmail.com"
#define AUTHOR_PASSWORD "gzeaaqpjlfkoluvx"


/* Recipient's email*/
String RECIPIENT_EMAIL= "kholaknanda@gmail.com";

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

//count for bell
int count=0;

#include <LiquidCrystal_I2C.h> // Library for LCD

LiquidCrystal_I2C lcd1 = LiquidCrystal_I2C(0x25, 16, 2);
LiquidCrystal_I2C lcd2 = LiquidCrystal_I2C(0x26, 16, 2);
LiquidCrystal_I2C lcd3 = LiquidCrystal_I2C(0x23, 16, 2);
LiquidCrystal_I2C lcd4 = LiquidCrystal_I2C(0x27, 16, 2);
 
RTC_DS1307 rtc;     // Setup an instance of DS1307 naming it RTC
DateTime t;

#define main_bt1 D0
#define main_bt2 D6

#define buzzer D5

int hh = 0, mm = 0, ss = 0, dd = 0, bb = 0, set_day;
int yy = 0;
String Day = "  ";
int StartHH  = 0, StartMM  = 0, FinishHH  = 0, FinishMM  = 0, setMode = 0, setAlarm = 0, alarmMode=1;

int stop =0, mode=0, flag=0;
struct ClassInfo {
  int hour;
  int minute;
  const char* teacherName;
  const char* subject1Name;
  const char* subject2Name;
  const char* email;
};
const ClassInfo MONDAY_SCHEDULE[] = {
  {7, 30, "MR Kalansooriya", "UXUI", "Programing","idpp136@gmail.com"},//next subject teacher`s mail required 
  {8, 30, "MR Budditha", "Programing", "Networking","iddp138@gmail.com"},
  {9, 10, "Mrs Punsissi", "Networking", "Security","idpp139@gmail.com"},
  {9, 50, "Miss Upeksha", "Security", "Interval","iddp140@gmail.com"},
  {10, 50, "Mrs Wijendra", "DSA", "DataBase","idpp141@gmail.com"},
  {21, 45, "Mrs Madushanka", "DataBase", "OOP","idpp145@gmail.com"},
  {21, 50, "Miss Illmini", "OOP", "OS","idpp146@gmail.com"},
  {22, 10, "Miss Sandali", "OS", "Good Day","idpp144@gmail.com"}};

const ClassInfo TUESDAY_SCHEDULE[] = {
  {7, 30, "MR Kalansooriya", "UXUI", "Programing","idpp136@gmail.com"},//next subject teacher`s mail required 
  {8, 30, "MR Budditha", "Programing", "Networking","iddp138@gmail.com"},
  {9, 10, "Mrs Punsissi", "Networking", "Security","idpp139@gmail.com"},
  {9, 50, "Miss Upeksha", "Security", "Interval","iddp140@gmail.com"},
  {10, 50, "Mrs Wijendra", "DSA", "DataBase","idpp141@gmail.com"},
  {21, 45, "Mrs Madushanka", "DataBase", "OOP","idpp145@gmail.com"},
  {21, 50, "Miss Illmini", "OOP", "OS","idpp146@gmail.com"},
  {22, 10, "Miss Sandali", "OS", "Good Day","idpp144@gmail.com"}};

const ClassInfo WEDNESDAY_SCHEDULE[] = {
  {7, 30, "MR Kalansooriya", "UXUI", "Programing","idpp136@gmail.com"},//next subject teacher`s mail required 
  {8, 30, "MR Budditha", "Programing", "Networking","iddp138@gmail.com"},
  {9, 10, "Mrs Punsissi", "Networking", "Security","idpp139@gmail.com"},
  {9, 50, "Miss Upeksha", "Security", "Interval","iddp140@gmail.com"},
  {10, 50, "Mrs Wijendra", "DSA", "DataBase","idpp141@gmail.com"},
  {21, 45, "Mrs Madushanka", "DataBase", "OOP","idpp145@gmail.com"},
  {21, 50, "Miss Illmini", "OOP", "OS","idpp146@gmail.com"},
  {22, 10, "Miss Sandali", "OS", "Good Day","idpp144@gmail.com"}};


const ClassInfo THURSDAY_SCHEDULE[] = {
  {7, 30, "MR Kalansooriya", "UXUI", "Programing","idpp136@gmail.com"},//next subject teacher`s mail required 
  {8, 30, "MR Budditha", "Programing", "Networking","iddp138@gmail.com"},
  {9, 10, "Mrs Punsissi", "Networking", "Security","idpp139@gmail.com"},
  {9, 50, "Miss Upeksha", "Security", "Interval","iddp140@gmail.com"},
  {10, 50, "Mrs Wijendra", "DSA", "DataBase","idpp141@gmail.com"},
  {21, 45, "Mrs Madushanka", "DataBase", "OOP","idpp145@gmail.com"},
  {21, 50, "Miss Illmini", "OOP", "OS","idpp146@gmail.com"},
  {22, 10, "Miss Sandali", "OS", "Good Day","idpp144@gmail.com"}};
  
const ClassInfo FRIDAY_SCHEDULE[] = {
  {7, 30, "MR Kalansooriya", "UXUI", "Programing","idpp136@gmail.com"},//next subject teacher`s mail required 
  {8, 30, "MR Budditha", "Programing", "Networking","iddp138@gmail.com"},
  {9, 10, "Mrs Punsissi", "Networking", "Security","idpp139@gmail.com"},
  {9, 50, "Miss Upeksha", "Security", "Interval","iddp140@gmail.com"},
  {10, 50, "Mrs Wijendra", "DSA", "DataBase","idpp141@gmail.com"},
  {21, 45, "Mrs Madushanka", "DataBase", "OOP","idpp145@gmail.com"},
  {21, 50, "Miss Illmini", "OOP", "OS","idpp146@gmail.com"},
  {22, 10, "Miss Sandali", "OS", "Good Day","idpp144@gmail.com"}};

const int NUM_MONDAY_CLASSES = sizeof(MONDAY_SCHEDULE) / sizeof(ClassInfo);
const int NUM_TUESDAY_CLASSES = sizeof(TUESDAY_SCHEDULE) / sizeof(ClassInfo);
const int NUM_WEDNESDAY_CLASSES = sizeof(WEDNESDAY_SCHEDULE) / sizeof(ClassInfo);
const int NUM_THURSDAY_CLASSES = sizeof(THURSDAY_SCHEDULE) / sizeof(ClassInfo);
const int NUM_FRIDAY_CLASSES = sizeof(FRIDAY_SCHEDULE) / sizeof(ClassInfo);

bool isClassTime(const ClassInfo& c, const DateTime& t) {
   Serial.print("c.hour");
   Serial.println(c.hour);

   Serial.print("t.hour");
   Serial.println(t.hour());
   
   Serial.print("c.minute");
   Serial.println(c.minute);

   Serial.print("t.minute");
   Serial.println(t.minute());

  Serial.print("\n\nt.minute -2 ");
   Serial.println((c.minute-2)==t.minute());
   Serial.print("\n\n");
    
  if((c.hour == t.hour()) && (c.minute == t.minute())){
    Serial.println("ojiyaaaaaaaaaaaaaaaa\n\n");

    if(count==0){
      bellCall();
      count++;
    }
    
  
    return true;
  }else if((c.hour == t.hour()) && ((c.minute-2) == t.minute())){
    Serial.println("hiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n\n");

  //reset the bell for next period
  count=0;
  
  smtp.callback(smtpCallback);

  /* Declare the session config data */
  ESP_Mail_Session session;
   /* Declare the message class */
  SMTP_Message message;
     /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";
   /* Set the message headers */
  message.sender.name = "ESP";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "NEXT CLASS IN 5 MIN";
  message.addRecipient("TEACHER", c.email);

   /*Send HTML message*/
  String htmlMsg = "<div style=\"color:#2f4468;\"><h1>You have a class in 5 minutes</h1><p>- Sent from ESP board</p></div>";
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  
  /* Connect to server with the session config */
  smtp.connect(&session);
    
    

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
    Serial.println("your subject is next to class no : 1\n" );

    
    
    delay(58000);///////////////---------------------------->
    return false;

  
  }else{
    return false;
  }
  
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void displayClassInfo(const ClassInfo& c) {

  lcd1.clear();
  lcd2.clear();
  lcd3.clear();
  lcd4.clear();
  
  lcd2.setCursor(0, 0);
  lcd3.setCursor(0, 0);
  lcd2.print(c.teacherName);
  lcd3.print(c.teacherName);
  lcd2.setCursor(0, 1);
  lcd3.setCursor(0, 1);

    lcd2.print(digitalRead(main_bt2) == LOW ? "Present" : "Absent");
    lcd3.print(digitalRead(main_bt1) == LOW ? "Present" : "Absent");
  
  lcd1.setCursor(0, 0);
  lcd4.setCursor(0, 0);
  lcd4.print("NOW ");
  lcd1.print("NOW ");
  lcd4.print(c.subject1Name);
  lcd1.print(c.subject1Name);
  lcd1.setCursor(0, 1);
  lcd4.setCursor(0, 1);
  lcd4.print("NEXT ");
  lcd1.print("NEXT ");
  lcd4.print(c.subject2Name);
  lcd1.print(c.subject2Name);


      Serial.println("okkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk\n\n");
      
    

  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
void setup () 
{
  Serial.begin(115200); // Set serial port speed
   pinMode(main_bt1,INPUT);
   pinMode(main_bt2,INPUT);
   pinMode(buzzer,OUTPUT);
//---------------------------------------------- wifi   ------------------------------------
    Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
 
//---------------------------------------------- wifi   ------------------------------------
  lcd1.begin();
  lcd2.begin();
  lcd3.begin();
  lcd4.begin();
  
  Wire.begin(); // Start the I2C
  rtc.begin();  // Init RTC
 // RTC.adjust(DateTime(2023, 4, 17, 17, 24, 00));  // Time and date is expanded to date and time on your computer at compiletime
 // Serial.print('Time and date set');
   
  
  
  lcd1.backlight();
  lcd2.backlight();
  lcd3.backlight();
  lcd4.backlight();
  delay(1000);
  lcd1.noBacklight();
  lcd2.noBacklight();
  lcd3.noBacklight();
  lcd4.noBacklight();
  delay(1000);
  
  lcd1.setCursor (5,0);
  lcd1.print("ojiya");
  lcd1.setCursor (5,1);
  lcd1.print("TIMER");

  lcd2.setCursor (5,0);
  lcd2.print("ojiya");
  lcd2.setCursor (5,1);
  lcd2.print("TIMER");

  lcd3.setCursor (5,0);
  lcd3.print("ojiya");
  lcd3.setCursor (5,1);
  lcd3.print("TIMER");

  lcd4.setCursor (5,0);
  lcd4.print("ojiya");
  lcd4.setCursor (5,1);
  lcd4.print("TIMER");

   lcd1.backlight();
  lcd2.backlight();
  lcd3.backlight();
  lcd4.backlight();
  
  delay (4000);
  lcd1.clear();
  lcd2.clear();
  lcd3.clear();
  lcd4.clear();
}
 

void loop(){
 t = rtc.now();
 int dow = t.dayOfTheWeek();
 Serial.print("Today is ");
  switch (dow) {
    case 0:
      Serial.println("Sunday");
      Day="Sunday";
      break;
    case 1:
      Serial.println("Monday");
      Day="Monday";
      break;
    case 2:
      Serial.println("Tuesday");
      Day="Tuesday";
      break;
    case 3:
      Serial.println("Wednesday");
      Day="Wednesday";
      break;
    case 4:
      Serial.println("Thursday");
      Day="Thursday";
      break;
    case 5:
      Serial.println("Friday");
      Day="Friday";
      break;
    case 6:
      Serial.println("Saturday");
      Day="Saturday";
      break;
  }

 

if (dow == 1){
    for (int i = 0; i < NUM_MONDAY_CLASSES; i++) {
     
    const ClassInfo& c = MONDAY_SCHEDULE[i];
    if (isClassTime(c, t)) {
      Serial.println("yo yooooooooo\n\n");
      displayClassInfo(c);


      
      delay(3000);
      break;
    }
  }
    
  }
  else if ( dow == 2){
     for (int i = 0; i < NUM_TUESDAY_CLASSES; i++) {
    const ClassInfo& c = TUESDAY_SCHEDULE[i];
    if (isClassTime(c, t)) {
      displayClassInfo(c);
   
   

      
      delay(3000);
      break;
    }
  }
  }
  else if ( dow == 3){
    for (int i = 0; i < NUM_WEDNESDAY_CLASSES; i++) {
    const ClassInfo& c = WEDNESDAY_SCHEDULE[i];
    if (isClassTime(c, t)) {
      displayClassInfo(c);

      delay(3000);
      break;
    }
  }
  }
  else if ( dow == 4){
       for (int i = 0; i < NUM_THURSDAY_CLASSES; i++) {
    const ClassInfo& c = THURSDAY_SCHEDULE[i];
    if (isClassTime(c, t)) {
      displayClassInfo(c);
   
  
      delay(3000);
      break;
    }
  }
  }
  else if ( dow == 5){
       for (int i = 0; i < NUM_FRIDAY_CLASSES; i++) {
    const ClassInfo& c = FRIDAY_SCHEDULE[i];
    if (isClassTime(c, t)) {
      displayClassInfo(c);

      
      delay(3000);
      break;
    }
  }
  }     
  else if(dow == 6 || dow == 7) {
  
  lcd1.clear();
  lcd2.clear();
  lcd3.clear();
  lcd4.clear();
  
  lcd1.setCursor (5,0);
  lcd1.print("HAPPY");
  lcd1.setCursor (5,1);
  lcd1.print("WEEKEND");

  lcd2.setCursor (5,0);
  lcd2.print("HAPPY");
  lcd2.setCursor (5,1);
  lcd2.print("WEEKEND");

  lcd3.setCursor (5,0);
  lcd3.print("HAPPY");
  lcd3.setCursor (5,1);
  lcd3.print("WEEKEND");

  lcd4.setCursor (5,0);
  lcd4.print("HAPPY");
  lcd4.setCursor (5,1);
  lcd4.print("WEEKEND");

   lcd1.backlight();
  lcd2.backlight();
  lcd3.backlight();
  lcd4.backlight();
  

  
     }

  delay(1000);
}
//calling bell
void bellCall(){
  digitalWrite(buzzer,HIGH);
      for(int i=0;i<=3;i++){
        digitalWrite(buzzer,HIGH);
        delay(1000);
        digitalWrite(buzzer,LOW);
        delay(1000);
      }
    digitalWrite(buzzer,LOW);  
}
 
/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");
  }
}
