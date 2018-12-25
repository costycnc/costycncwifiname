#line 1 "costycnc.wifi.2.0_classic3_extern_ip3.ino"
  
                                                                                      
                                   
                                                       
       
  
#include "Arduino.h"
void setup();
void loop();
void sendresp();
void xplus();
void xminus();
void yplus();
void yminus();
void readparam();
void fileread();
void fileread1();
void arranqueweb();
void image();
void costel();
void svg();
void potracex();
void list();
void speed1();
void blink1();
void grbl();
void arguments();
void costycnc();
bool hFileRead1();
bool hFileRead();
bool hFileRead2();
void absolute();
bool string();
void rssi();
void header();
void init_process_string();
void process_string(String instruction, int size);
double search_string(char key, String instruction, int string_size);
bool has_command(char key, String instruction, int string_size);
void init_steppers();
void dda_move(long micro_delay);
bool can_step(long current, long target, byte direction);
void do_step(byte step_pin);
bool read_switch(byte pin);
long to_steps(float steps_per_unit, float units);
void set_target(float x, float y, float z);
void set_position(float x, float y, float z);
void calculate_deltas();
long calculate_feedrate_delay(float feedrate);
long getMaxSpeed();
void disable_steppers();
#line 7
static const uint8_t D0   = 5;
static const uint8_t D1   = 4;
static const uint8_t D2   = 0;
static const uint8_t D3   = 2;
static const uint8_t D4   = 16;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

  char c;

#define COMMAND_SIZE 128
char word1[COMMAND_SIZE];
byte serial_count;
int no_data = 0;

int X_STEPS_PER_MM=128; 
int Y_STEPS_PER_MM=128;
int FAST_XY_FEEDRATE=200;


                                                                   

#include <ESP8266WiFi.h>
                          
#include <ESP8266WebServer.h>
#include <FS.h>                                

const char* ssid = "Infostrada-2.4GHz-02C3F6";
const char* password = "costycnc1967";

ESP8266WebServer server(80);

String responseHTML = ""
  "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
  "<h1>Hello World!</h1><p>This is a captive portal example. All requests will "
  "be redirected here.</p></body></html>";


File fsUploadFile;                                                                     

String getContentType(String filename);                                               
bool handleFileRead(String path);                                                          
void handleFileUpload();                                                  

void setup() {
  init_steppers();
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  Serial.begin(9600);                                                                           
  delay(10);
  Serial.println('\n');
  
                                                                
ESP.eraseConfig();
WiFi.disconnect(true);
WiFi.mode(WIFI_AP);
WiFi.softAP("Costycnc");
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

SPIFFS.begin();                                                              

readparam();

  server.begin();
  Serial.println("TCP server started");
  server.on("/upload", HTTP_GET, []() {                                                          
    if (!handleFileRead("/upload.html"))                                       
      server.send(404, "text/plain", "404: Not Found");                                                   
  });

  server.on("/upload", HTTP_POST,                                                                
    [](){ server.send(200); },                                                                                            
    handleFileUpload                                                                
  );



  
  
                                                                     
                                                                                          
                                                             
                               
     
  
  server.on("/costycnc",costycnc);
  server.on("/grbl",grbl);
  server.on("/speed1",speed1);
  server.on("/blink",blink1);
  server.on("/fileread",fileread);
  server.on("/fileread1",fileread1);
  server.on("/list",list);
  server.on("/svg",svg);
  server.on("/potracex",potracex);
  server.on("/arguments",arguments);
  server.on("/absolute",absolute);
   server.on("/string",string);
    server.on("/rssi",rssi);
    server.on("/header",header);
     server.on("/costel",costel);
     server.on("/image",image);
     server.on("/xplus",xplus);
     server.on("/xminus",xminus);
     server.on("/yplus",yplus);
     server.on("/yminus",yminus);

 

                                         
                            
                                                  
       
  server.onNotFound([]() {                                                               
    if (!handleFileRead(server.uri()))                                         
      server.send(404, "text/plain", "404: Not Found");                                                   
  });

  server.begin();                                                       
  Serial.println("HTTP server started");
}

void loop() {
                                    
  server.handleClient();
}

String getContentType(String filename) {                                               
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) {                                                    
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";                                                          
  String contentType = getContentType(path);                                 
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {                                                                 
    if (SPIFFS.exists(pathWithGz))                                                                     
      path += ".gz";                                                                     
    File file = SPIFFS.open(path, "r");                                    
    size_t sent = server.streamFile(file, contentType);                            
    file.close();                                                                 
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);                                             
  return false;
}

void handleFileUpload(){                                   
  
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
    Serial.print("handleFileUpload Name: "); Serial.println(filename);
    fsUploadFile = SPIFFS.open(filename, "w");                                                                               
    filename = String();
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);                                        
  } else if(upload.status == UPLOAD_FILE_END){
    if(fsUploadFile) {                                                                           
      fsUploadFile.close();                                                      
      Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
      server.sendHeader("Access-Control-Allow-Origin", "*");
                                                                                                      
      server.send(303);
    } else {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}


#line 1 "_init.ino"

                                        
#define X_STEPS_PER_INCH 416.772354
                                      
#define X_MOTOR_STEPS    400

#define Y_STEPS_PER_INCH 416.772354
                         
#define Y_MOTOR_STEPS    400

#define Z_STEPS_PER_INCH 16256.0
#define Z_STEPS_PER_MM   640.0
#define Z_MOTOR_STEPS    400

                       
                           
#define FAST_Z_FEEDRATE  50.0

                         
#define CURVE_SECTION_INCHES 0.019685
#define CURVE_SECTION_MM 0.5

                                                                            
                                            
#define SENSORS_INVERTING 0

                                                                            
#define TEMPERATURE_SAMPLES 5

                                                                                         
                            
 
                                                                                     
                                                                                         


        
          
                    
                            
                           
                           
                           
                               

                             
                     
                     
                      
                         

                       
                      
                      
                      
                         


#line 1 "custom.ino"

char movex[]="X10\n";
char movey[]="Y10\n";
char a[128]="COSTYCNC";




int x=0;

void sendresp() {
  
String text= "<!DOCTYPE html><html><head>";
  text +="<meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body>";
  text +="<p>Until cnc finished moving you can not make another commands\n"; 
  text +="If cnc finished moving you can make this commands:</p>";
  text +="<p><h1><a href='/xplus'>X+ </a>";
  text +="<a href='/xminus'>X- </a>";
  text +="<a href='/yplus'>Y+ </a>";
  text +="<a href='/yminus'>Y- </a></h1></p>";
  text +="<p><a href='index.html'>index</a></p></body></html>";
   server.send(200, "text/html", text);
}

void xplus() {
  sendresp();
  set_position(0.0, 0.0, 0.0);
  String message="G01 X10 Y0\n";
  process_string(message,message.length());
  set_position(0.0, 0.0, 0.0);
}

void xminus() {
  sendresp();
  set_position(10.0, 0.0, 0.0);
  String message="G01 X0 Y0\n";
  process_string(message,message.length());
  set_position(0.0, 0.0, 0.0);
}


void yplus() {
  sendresp();
  set_position(0.0, 0.0, 0.0);
  String message="G01 X0 Y10\n";
  process_string(message,message.length());
  set_position(0.0, 0.0, 0.0);
  
}

void yminus() {
  sendresp();
  set_position(0.0, 10.0, 0.0);
  String message="G01 X0 Y0\n";
  process_string(message,message.length());
  set_position(0.0, 0.0, 0.0);
}


void readparam() {
  
String message="";
byte param=0;
char t;
 File f = SPIFFS.open("/settings.txt", "r");
 while(f.available()) {
     t = f.read();
     message += t;
     if(t=='\n'){              
      param++;
      if(param==1){
      X_STEPS_PER_MM=message.toInt(); 
      Serial.print("parametru1=");
      Serial.println( X_STEPS_PER_MM);
      
      }
      if(param==2){
      Y_STEPS_PER_MM=message.toInt();
      Serial.print("parametru2="); 
      Serial.println( Y_STEPS_PER_MM); 
      } 
      if(param==3){
      FAST_XY_FEEDRATE=message.toInt();
      Serial.print("parametru3=");
      Serial.println(FAST_XY_FEEDRATE);  
      }
      message="";                           
       }
    }
f.close();
}


void fileread() { 
String message="";
char t;

init_process_string();
server.send(200, "text/html", "until cnc not finished cut you not can not make any command!!!" );
 File f = SPIFFS.open("/test.nc", "r");
 while(f.available()) {
     t = f.read();
     message += t;
     if(t=='\n'){              
       process_string(message,message.length());                                       
       message="";                           
       }
    }
f.close();
} 

void fileread1() { 
String message="";
char t;

init_process_string();
server.send(200, "text/html", "until cnc not finished cut you not can not make any command!!!" );
 File f = SPIFFS.open("/test1.nc", "r");
 while(f.available()) {
     t = f.read();
     message += t;
     if(t=='\n'){              
       process_string(message,message.length());                                       
       message="";                           
       }
    }
f.close();
} 

 void arranqueweb()
{
   server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
   server.sendHeader("Pragma", "no-cache");
   server.sendHeader("Expires", "-1");
                                               
   server.setContentLength(CONTENT_LENGTH_UNKNOWN);
   server.send(200, "text/html", "");
   server.sendContent("<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /><meta name=\"apple-mobile-web-app-capable\" content=\"yes\" /><meta name=\"apple-mobile-web-app-status-bar-style\" content=\"black-translucent\" /><style>body {font-family:arial;} </style>");
   server.sendContent("</head><body><h1>Node 1</h1>");
   server.sendContent("<h6>Node test v 0.5</h6></body></html>");
   server.client().stop();
}


  
                             
                            
                      

                                                                 
                      
  
                     

                                                                                                                          
                                                      
                                  

                            
                                                                                 
                  
  
                                                               
                                                                           
                                          
   
  
 

                                       
                      
                                               
                    
   

void image() { 
String message="";
String gcodex="";
String gcodey="";
char t;
int a;
String puncte="";

   server.setContentLength(CONTENT_LENGTH_UNKNOWN);
   server.send(200, "text/html", "");

server.sendContent("<!DOCTYPE html><html><head><script>document.body.innerHTML ='';</script>");
server.sendContent("<meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body>");
server.sendContent("<svg height='300' width='1500'><g transform='scale(2)'><polyline points='");
 File f = SPIFFS.open("/test.nc", "r");
 while(f.available()) {
     t = f.read();
     if(t=='/'){message = f.readStringUntil('\n');}
                               
                    
     if(t=='X'){a=1;gcodex="";}                                            
     if(t=='Y'){a=2;gcodey="";}                                            
     if(t=='\n'){              
       a=0;
       
       puncte +=gcodex;
       puncte +=",";
       puncte +=gcodey;
       server.sendContent(puncte);
       puncte =" ";
       
                                               
       }
     if(a==1) if(t !='X' && t !=' ') gcodex +=t;                                             
     if(a==2) if(t !='Y' && t !=' ') gcodey +=t;                                             
     }
f.close();


server.sendContent("' style='fill:white;stroke:red;stroke-width:.5' />");
server.sendContent("Sorry, your browser does not support inline SVG.</svg></body></html>");
server.client().stop();
} 



void costel() { 
String message="";
String gcodex="";
String gcodey="";
char t;
int a;
String inceput= "<!DOCTYPE html><html><head><script>document.body.innerHTML ='';</script>";
inceput +="<meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body><svg height='500' width='500'>";
inceput +="<g transform='scale(2)'><polyline points='";
String sfarsit ="' style='fill:white;stroke:red;stroke-width:.5' />";
sfarsit +="Sorry, your browser does not support inline SVG.</svg></body></html>";
String puncte="";

 File f = SPIFFS.open("/test.nc", "r");
 while(f.available()) {
     t = f.read();
                               
     message += t;
     if(t=='X'){a=1;gcodex="";}                                            
     if(t=='Y'){a=2;gcodey="";}                                            
     if(t=='\n'){              
       a=0;
       puncte +=gcodex+','+gcodey+' ';
       server.send(100, "text/html", inceput+puncte+sfarsit);                                                               
       process_string(message,message.length());                                       
       message="";                           
       
       }
     if(a==1) if(t !='X' && t !=' ') gcodex +=t;                                             
     if(a==2) if(t !='Y' && t !=' ') gcodey +=t;                                             
     }
f.close();
server.send(200, "text/html", "ok" );
} 

void svg()
{
  String gcode="G01 X1.27 Y2.00";
  char b;
  String x2;
  String y2;


 String svginceput = "<div><svg height='210' width='500'><line x1='0' y1='0' x2='";

 String svgy2 ="' y2='";
 String svgsfarsit="' style='stroke:rgb(255,0,0);stroke-width:2' />Sorry, your browser does not support inline SVG.</svg></div>";




  
   for (int i=1; i<=13; i++){
    b=gcode[i];
        if (b=='X'){
      x2="";
x2 +=gcode[i+1];
x2 +=gcode[i+2];
x2 +=gcode[i+3];
    }
        if (b=='Y'){
      y2="";
y2 +=gcode[i+1];
y2 +=gcode[i+2];
y2 +=gcode[i+3];
    }
    }
      


                                           
           
                                                        
   
                              
              
                                              
                                                                                                   
                                                                                    

                                              
   server.send(200, "text/html", svginceput+x2+svgy2+y2+svgsfarsit);

}

void potracex() { 
 String message = "";
 String mes = "costycnc";

  String gcode=server.arg(0);
  int lastStringLength = gcode.length(); 
 
                                                  

  if (server.arg(0)== ""){                          

  message = "Feedback Argument not found";

  }else{                      






  
  message = "Gcode1 = " ;
  message +=lastStringLength;


message += "<textarea rows='4' cols='50' name='textg'>";
  
  message += gcode;                                            
  message += "</textarea>";
   message += "<p>go to <a href='http://192.168.4.1'>index</a></p>";

 }

                                             
                                                                            
                                                   
                                        
  
                                                         
                                
 String svg="";
 String svginceput = "<div><svg height='500' width='500'><g transform='scale(10)'><polyline stroke='black' fill='none' stroke-width:.1 points='";
 String svgsfarsit="' />Sorry, your browser does not support inline SVG.</g></svg></div>";

                                                                     
                                                
String inceput = "<html><head><script>document.body.innerHTML = '<html></html>';</script>";
inceput +="<meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body><div><textarea>";
String dupaa="</textarea></div>";
String progress="<progress value='";
String sfarsit="' max='85'></progress></body></html>";
char b;
int c=0;

int j=0;
String x="0",y="0";
b=server.arg(0)[1];
 Serial.println("test=");
 Serial.println(b);
 Serial.println("=sfarsit");


int u=0;
  for (int i=1; i<=lastStringLength; i++){
    
    
    b=gcode[i];

                                                         
    if (b=='X'){
      x="";
x +=gcode[i+1];
x +=gcode[i+2];
if(gcode[i+3]=='.'){
x +=",";  
}else{
x +=gcode[i+3]; 
x +=","; 
}
    }
    if (b=='Y'){
      y="";
 y +=gcode[i+1];
 y +=gcode[i+2];
if(gcode[i+3]=='.'){
y +=" ";  
}else{
y +=gcode[i+3]; 
y +=" "; 
}
    }


                                                           
   a[j]=b;
  if(b=='\n')
  {
    a[j]=0;
    
    c++;
    svg +=x;
    svg +=y;
    
server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(100, "text/html", inceput+a+dupaa+svginceput+svg+svgsfarsit+progress+c+sfarsit);
    Serial.println(a);
    process_string(a,j);
    j=-1;
  
   }
j++;
  }
  
  disable_steppers();
                                                                                      
  server.send(200, "text/html",String("ok")+c);


}
                                                                   

void list()
{
  
  String str = "<p>go to <a href='index.html'>index</a></p>";
Dir dir = SPIFFS.openDir("/");
while (dir.next()) {
    str += dir.fileName();
    str += " / ";
    str += dir.fileSize();
    str += "\r\n";
   
    
}
Serial.print(str);  
server.send(200, "text/html", str);
}

                                                                                                               
void speed1()
{

String str= "<!DOCTYPE html><html><head>";
str +="<meta name='viewport' content='width=device-width, initial-scale=1.0'></head><body>";

 str += "<h1><p>go to <a href='index.html'>index</a></p>";
 str +=" X step/mm=";
 str +=X_STEPS_PER_MM ;

 
 str +=" Y step/mm=";
 str +=Y_STEPS_PER_MM ;


 str +=" Feedrate=";
 str +=FAST_XY_FEEDRATE ;

                                                    
str +="<br>";
str +="<form action='arguments'>";

str +="Feedrate: <input type='text' name='feedrate' value=";
str +=FAST_XY_FEEDRATE;
str +="><br>";

str +="X step/mm: <input type='text' name='stepx' value=";
str +=X_STEPS_PER_MM;
str +="><br>";

str +="Y step/mm: <input type='text' name='stepy' value=";
str +=Y_STEPS_PER_MM;
str +="><br>";

str +="<input type='submit' value='Set parameters'>";
str +="</form></h1></body></html>";

                                 
                                                                            
                                                
         
server.send(200, "text/html", str);
}

void blink1()
{
 digitalWrite(2, HIGH); 
 delay(1000);
 digitalWrite(2, LOW); 
 delay(1000); 
  server.send(200, "text/html",String("<script>location.replace('/potracex')</script>"));
}
void grbl()
{

String message = "";

                                                   
                                    
                   
     
       
                              
                     
     
                                 
   
                                                       
                          
       
  message = "<p>go to <a href='http://192.168.4.1'>index</a></p>";
    message = "Again-> <p><a href='/grbl'>/grbl</a></p>";
   server.send(200, "text/html", word1 + message);
    process_string(movex, strlen(movex));
    process_string(movey, strlen(movey));
  
  
                    
      
    init_process_string();
     
     
   
}

void arguments() { 
 String message = "<h1>";

 if (server.arg("feedrate")== ""){
  }else{                      
  message += "feedrate = ";
  String valx=server.arg("feedrate");
  FAST_XY_FEEDRATE=valx.toInt();
  message += FAST_XY_FEEDRATE;                                            
  ;                             
 }

 if (server.arg("stepx")== ""){
  }else{                      
  message += "stepx = ";
  String valx=server.arg("stepx");
  X_STEPS_PER_MM=valx.toInt();
  message += X_STEPS_PER_MM;                                            
  ;                          
 }

 if (server.arg("stepy")== ""){
  }else{                      
  message += "stepy = ";
  String valx=server.arg("stepy");
  Y_STEPS_PER_MM=valx.toInt();
  message += Y_STEPS_PER_MM;                                            
  ;                          
 }

 
 message += "<p>go to <a href='index.html'>index</a></p></h1>";
  server.send(200, "text/html", message);                                     


}

void costycnc() { 
 String message = "";

  if (server.arg("Feedback")== ""){                          

  message = "Feedback Argument not found";

  }else{                      

  message = "Feedback Argument = ";
  message += server.arg("Feedback");                                            
  ;                            

 message += "<p>go to <a href='http://192.168.4.1'>index</a></p>";
 }

  server.send(200, "text/html", message);                                     


}

bool hFileRead1() {                                                    
 String message = "";

                          
 File f = SPIFFS.open("/test.nc", "r");

char *p;
char b;
char a[128];
int j=0;
  if (!f) {
      Serial.println("file open failed");
       server.send(200, "text/plain", "file open failed");
      return 0;
  }  Serial.println("====== Reading from SPIFFS file =======");
    message = "<h1>====== Reading from SPIFFS file =======</h1>";
   message += " <p>You need wait until cnc finished moving</p>";
   message += "go to index -><a href='/index.html'>/index.html</a></p>";
server.sendHeader("Access-Control-Allow-Origin", "*");
server.send(200, "text/html",message );
                             
  for (int i=1; i<=f.size(); i++){
    b=f.read();
   a[j]=b;  
  if(b=='\n')
  {
    a[j]=0;
    Serial.println(a);
    process_string(a,j);
    j=-1;
  
   }
j++;
  }
  f.close();
  disable_steppers();
 
}

bool hFileRead() {                                                    
 String message = "";
 String message1= "";
 String message2= "";

                          
 File f = SPIFFS.open("/test.nc", "r");

  if (!f) {
      Serial.println("file open failed");
       server.send(200, "text/plain", "file open failed");
      return 0;
  }  Serial.println("====== Reading from SPIFFS file =======");
    message = "<html><head>";
    message +="<script>document.body.innerHTML = '';</script></head><body><h1>====== Reading from SPIFFS file =======</h1>";
   message += " <p>You need wait until cnc finished moving</p>";
   message += "go to index -><a href='/index.html'>/index.html</a></p></body></html>";
server.sendHeader("Access-Control-Allow-Origin", "*");




message1="";
 message = "<html><head>";
    message +="<script>document.body.innerHTML = '';</script></head><body><h1>====== Reading from SPIFFS file =======</h1>";
      message +="<svg xmlns='http://www.w3.org/2000/svg' version='1.1'";
      message +="width='250' height='250'><rect x='25' y='25' width='200'";
      message +="height='200' fill='lime' stroke-width='4' stroke='pink'></rect></svg></html>";

char b;
int i=0;
 while(f.available()) {

                                            
     
                                           
      b=f.read();
      i++;
      message1 +=b;
       if(b=='\n')
  {
    process_string(message1,message1.length());
    server.send(100, "text/html",message+message1);
    message1="";
    i=0;
   }
      
      process_string(message1,message1.length());
        }
  f.close();
  disable_steppers();
      server.send(200, "text/html",message1+message +" "+message1.length());
 
}

bool hFileRead2() {                                                    
String message;
                          
 File f = SPIFFS.open("/test.nc", "r");
   
                        
                                            
                                            
                           
     
       
      while(f.available()) {
                                            
      message += f.readStringUntil('\n');
        }
      
f.close();
server.send(200, "text/html", message );
} 



void absolute()
{
  String message="asolute=";
 float delta_x;
 float target_x=2.07;
 float current_x=1.03;

 

delta_x = fabs(target_x - current_x);
 message +=delta_x;
 message +="<br><p>go to <a href='http://192.168.4.1'>index</a></p>";
 
   server.send(200, "text/html", message ); 

}



bool string() {                                                    
String message;
                          
 File f = SPIFFS.open("/test.nc", "r");
   
                        
                                            
                                            
                           
     
       
      while(f.available()) {
                                            
      message += f.readStringUntil('\n');
        }
      
f.close();
server.send(200, "text/html", message );
} 

void rssi()
{
long rssi = WiFi.RSSI();
 
   server.send(200, "text/html", String(rssi)); 

}

void header()
{
 server.sendHeader("bla","bla"); 
  server.send(200,"text/html","bla bla bla");
}


#line 1 "process_string.ino"
                                           
struct LongPoint {
	long x;
	long y;
 	long z;
};

struct FloatPoint {
	float x;
	float y;
 	float z;
};

FloatPoint current_units;
FloatPoint target_units;
FloatPoint delta_units;

FloatPoint current_steps;
FloatPoint target_steps;
FloatPoint delta_steps;

boolean abs_mode = true;                                  

                             
float x_units = X_STEPS_PER_MM;
float y_units = Y_STEPS_PER_MM;
float z_units = Z_STEPS_PER_MM;
float curve_section = CURVE_SECTION_MM;

                    
byte x_direction = 1;
byte y_direction = 1;
byte z_direction = 1;

                            
void init_process_string()
{
	                  
	for (byte i=0; i<COMMAND_SIZE; i++)
		word1[i] = 0;
	serial_count = 0;
x_units = X_STEPS_PER_MM;
y_units = Y_STEPS_PER_MM;
}

                         
float feedrate = 0.0;
long feedrate_micros = 0;

                                          
void process_string(String instruction, int size)
{
  Serial.print("acum sunt in process_string");
  int val;
	                                                                    
	if (instruction[0] == '/')
	{
		Serial.println("ok");
		return;
	}

  byte code = 0;;

	            
	FloatPoint fp;
	fp.x = 0.0;
	fp.y = 0.0;
	fp.z = 0.0;

	
	
                      
                  
                                           
                                                       
	
  
                        
                      
                             
  		
	                     
	if (
		has_command('G', instruction, size) ||
		has_command('X', instruction, size) ||
		has_command('Y', instruction, size) ||
		has_command('Z', instruction, size)
	)
	{
		            
		code = (int)search_string('G', instruction, size);
		
		                                                      
		switch (code)
		{
			case 0:
			case 1:
			case 2:
			case 3:
				if(abs_mode)
				{
     
					                                                       
					                                                                             
					if (has_command('X', instruction, size))
						fp.x = search_string('X', instruction, size);
					else
						fp.x = current_units.x;
				
					if (has_command('Y', instruction, size))
						fp.y = search_string('Y', instruction, size);
					else
						fp.y = current_units.y;
				
					if (has_command('Z', instruction, size))
						fp.z = search_string('Z', instruction, size);
					else
						fp.z = current_units.z;
				}
				else
				{
     
					fp.x = search_string('X', instruction, size) + current_units.x;
					fp.y = search_string('Y', instruction, size) + current_units.y;
					fp.z = search_string('Z', instruction, size) + current_units.z;
				}
     
			break;
		}

		               
		switch (code)
		{
			                   
			                      
			                                     
			case 0:
			case 1:
				                 
				set_target(fp.x, fp.y, fp.z);

				                         
				if (has_command('G', instruction, size))
				{
					                                        
					if (code == 1)
					{
						                      
						feedrate = search_string('F', instruction, size);
						if (feedrate > 0)
							feedrate_micros = calculate_feedrate_delay(feedrate);
						                   
						else
							feedrate_micros = getMaxSpeed();
					}
					                               
					else
						feedrate_micros = getMaxSpeed();
				}
				                         
				else
				{
					                            
					if (feedrate > 0)
						feedrate_micros = calculate_feedrate_delay(feedrate);
					                   
					else
						feedrate_micros = getMaxSpeed();
				}

				               
                                
                                  
				dda_move(feedrate_micros);
                                           
  
			break;
			
			               
			case 2:
			                      
			case 3:
				FloatPoint cent;

				                                         
				cent.x = search_string('I', instruction, size) + current_units.x;
				cent.y = search_string('J', instruction, size) + current_units.y;
				float angleA, angleB, angle, radius, length, aX, aY, bX, bY;

				aX = (current_units.x - cent.x);
				aY = (current_units.y - cent.y);
				bX = (fp.x - cent.x);
				bY = (fp.y - cent.y);
				
				if (code == 2) {             
					angleA = atan2(bY, bX);
					angleB = atan2(aY, aX);
				} else {                    
					angleA = atan2(aY, aX);
					angleB = atan2(bY, bX);
				}

				                                                 
				                                                    
				                                                
				                                
				if (angleB <= angleA) angleB += 2 * M_PI;
				angle = angleB - angleA;

				radius = sqrt(aX * aX + aY * aY);
				length = radius * angle;
				int steps, s, step;
				steps = (int) ceil(length / curve_section);

				FloatPoint newPoint;
				for (s = 1; s <= steps; s++) {
					step = (code == 3) ? s : steps - s;                         
					newPoint.x = cent.x + radius * cos(angleA + angle * ((float) step / steps));
					newPoint.y = cent.y + radius * sin(angleA + angle * ((float) step / steps));
					set_target(newPoint.x, newPoint.y, fp.z);

					                                                   
					if (feedrate > 0)
						feedrate_micros = calculate_feedrate_delay(feedrate);
					else
						feedrate_micros = getMaxSpeed();

					            
					dda_move(feedrate_micros);
				}
	
			break;

			       
			case 4:
				delay((int)search_string('P', instruction, size));
			break;

			                  
			case 20:
				x_units = X_STEPS_PER_INCH;
				y_units = Y_STEPS_PER_INCH;
				z_units = Z_STEPS_PER_INCH;
				curve_section = CURVE_SECTION_INCHES;
				
				calculate_deltas();
			break;

			              
			case 21:
				x_units = X_STEPS_PER_MM;
				y_units = Y_STEPS_PER_MM;
				z_units = Z_STEPS_PER_MM;
				curve_section = CURVE_SECTION_MM;
				
				calculate_deltas();
			break;

			          
			case 28:
				set_target(0.0, 0.0, 0.0);
				dda_move(getMaxSpeed());
			break;

			                                    
			case 30:
				fp.x = search_string('X', instruction, size);
				fp.y = search_string('Y', instruction, size);
				fp.z = search_string('Z', instruction, size);

				                 
				if(abs_mode)
				{
					if (!has_command('X', instruction, size))
						fp.x = current_units.x;
					if (!has_command('Y', instruction, size))
						fp.y = current_units.y;
					if (!has_command('Z', instruction, size))
						fp.z = current_units.z;
						
					set_target(fp.x, fp.y, fp.z);
				}
				else
					set_target(current_units.x + fp.x, current_units.y + fp.y, current_units.z + fp.z);
				
				           
				dda_move(getMaxSpeed());

				          
				set_target(0.0, 0.0, 0.0);
				dda_move(getMaxSpeed());
			break;

			                      
			case 90:
				abs_mode = true;
			break;

			                         
			case 91:
				abs_mode = false;
			break;

			             
			case 92:
				set_position(0.0, 0.0, 0.0);
			break;

  
                           
           

                           

                         
           

                           
  

			default:
				Serial.print("huh? G");
				Serial.println(code,DEC);
		}
	}
	
	                    
	if (has_command('M', instruction, size))
	{
		code = search_string('M', instruction, size);
		switch (code)
		{
			                                                                       
			case 0:
				true;
			break;
	  
          
                        
         

          
                         
         

          
                       
         
   		  
                                      
            
                                                                  
         

                              
            
                              
                                       
         

                              
            
                              
                                       
         

                      
            
                          
         

                                        
            
                                                                         

                               
                                                                
     
                                   
                        
                                                
                  
     
    
         

                                        
            
                       
                                               
         
   
                
            
                             
         

                 
            
                           
         
     
			default:
				Serial.print("Huh? M");
				Serial.println(code);
		}		
	}
	
	                           
	Serial.println("ok");
                             
}

                                                                   
double search_string(char key, String instruction, int string_size)
{
	char temp[10] = "";

	for (byte i=0; i<string_size; i++)
	{
		if (instruction[i] == key)
		{
			i++;      
			int k = 0;
			while (i < string_size && k < 10)
			{
				if (instruction[i] == 0 || instruction[i] == ' ')
					break;

				temp[k] = instruction[i];
				i++;
				k++;
			}
			return strtod(temp, NULL);
		}
	}
	
	return 0;
}

                                    
bool has_command(char key, String instruction, int string_size)
{
	for (byte i=0; i<string_size; i++)
	{
		if (instruction[i] == key)
			return true;
	}
	
	return false;
}
#line 1 "stepper_control.ino"
#define max1(a,b) ((a)>(b)?(a):(b))
                    
long max_delta;
long x_counter;
long y_counter;
long z_counter;
bool x_can_step;
bool y_can_step;
bool z_can_step;
int milli_delay;

int costyx=1;
int costyy=1;

void init_steppers()
{
	                         
	disable_steppers();
	
	                  
	current_units.x = 0.0;
	current_units.y = 0.0;
	current_units.z = 0.0;
	target_units.x = 0.0;
	target_units.y = 0.0;
	target_units.z = 0.0;
	
	                              
	                             
	                                
	                            
	                            
	
	                              
	                             
	                                
	                            
	                            
	
	                              
	                             
	                                
	                            
	                            
	
	                   
	calculate_deltas();
}

void dda_move(long micro_delay)
{
	                     
	                                   
	                                   
	                                   
	
	                       
	max_delta = max1(delta_steps.x, delta_steps.y);
	max_delta = max1(delta_steps.z, max_delta);

	             
	long x_counter = -max_delta/2;
	long y_counter = -max_delta/2;
	long z_counter = -max_delta/2;
	
	                
	bool x_can_step = 0;
	bool y_can_step = 0;
	bool z_can_step = 0;
	
	if (micro_delay >= 16383)
		milli_delay = micro_delay / 1000;
	else
		milli_delay = 0;

	                  
	do
	{
  if(!max_delta)
break;
		x_can_step = can_step(current_steps.x, target_steps.x, x_direction);
		y_can_step = can_step(current_steps.y, target_steps.y, y_direction);
		z_can_step = can_step(current_steps.z, target_steps.z, z_direction);

		if (x_can_step)
		{
			x_counter += delta_steps.x;
      
			
			if (x_counter > 0)
			{
				                      
				x_counter -= max_delta;
				
				if (x_direction){
					current_steps.x++;
          costyx=costyx+1;
          if (costyx > 8) costyx=1;
    
}
				else{
					current_steps.x--;     
          costyx=costyx-1;
          if (costyx < 1) costyx=8;
     }
    }
		}

		if (y_can_step)
		{
			y_counter += delta_steps.y;
			
			if (y_counter > 0)
			{
				                      
				y_counter -= max_delta;

				if (y_direction){
					current_steps.y++;
          costyy=costyy+1;
          if (costyy > 8) costyy=1;
     }
				else{
					current_steps.y--;
          costyy=costyy-1;
          if (costyy < 1) costyy=8;
      }
      
  	}
		}
		
		if (z_can_step)
		{
			z_counter += delta_steps.z;
			
			if (z_counter > 0)
			{
				                      
				z_counter -= max_delta;
				
				if (z_direction)
					current_steps.z++;
				else
					current_steps.z--;
			}
		}
   yield();
     if (costyy==1) {digitalWrite(D5, HIGH);digitalWrite(D6, LOW);digitalWrite(D7, LOW);digitalWrite(D8, LOW);}                        
     if (costyy==2) {digitalWrite(D5, HIGH);digitalWrite(D6, HIGH);digitalWrite(D7, LOW);digitalWrite(D8, LOW);}                       
     if (costyy==3) {digitalWrite(D5, LOW);digitalWrite(D6, HIGH);digitalWrite(D7, LOW);digitalWrite(D8, LOW);}                      
     if (costyy==4) {digitalWrite(D5, LOW);digitalWrite(D6, HIGH);digitalWrite(D7, HIGH);digitalWrite(D8, LOW);}                   
     if (costyy==5) {digitalWrite(D5, LOW);digitalWrite(D6, LOW);digitalWrite(D7, HIGH);digitalWrite(D8, LOW);}                   
     if (costyy==6) {digitalWrite(D5, LOW);digitalWrite(D6, LOW);digitalWrite(D7, HIGH);digitalWrite(D8, HIGH);}                   
     if (costyy==7) {digitalWrite(D5, LOW);digitalWrite(D6, LOW);digitalWrite(D7, LOW);digitalWrite(D8, HIGH);}                   
     if (costyy==8) {digitalWrite(D5, HIGH);digitalWrite(D6, LOW);digitalWrite(D7, LOW);digitalWrite(D8, HIGH);}                   

     if (costyx==1) {digitalWrite(D0, HIGH);digitalWrite(D1, LOW);digitalWrite(D2, LOW);digitalWrite(D3, LOW);}                        
     if (costyx==2) {digitalWrite(D0, HIGH);digitalWrite(D1, HIGH);digitalWrite(D2, LOW);digitalWrite(D3, LOW);}                       
     if (costyx==3) {digitalWrite(D0, LOW);digitalWrite(D1, HIGH);digitalWrite(D2, LOW);digitalWrite(D3, LOW);}                      
     if (costyx==4) {digitalWrite(D0, LOW);digitalWrite(D1, HIGH);digitalWrite(D2, HIGH);digitalWrite(D3, LOW);}                   
     if (costyx==5) {digitalWrite(D0, LOW);digitalWrite(D1, LOW);digitalWrite(D2, HIGH);digitalWrite(D3, LOW);}                   
     if (costyx==6) {digitalWrite(D0, LOW);digitalWrite(D1, LOW);digitalWrite(D2, HIGH);digitalWrite(D3, HIGH);}                   
     if (costyx==7) {digitalWrite(D0, LOW);digitalWrite(D1, LOW);digitalWrite(D2, LOW);digitalWrite(D3, HIGH);}                   
     if (costyx==8) {digitalWrite(D0, HIGH);digitalWrite(D1, LOW);digitalWrite(D2, LOW);digitalWrite(D3, HIGH);}                   
     
		
		                                
	
		                     
		if (milli_delay > 0)
			{
			  delay(milli_delay);
			}		
		else
			{
			  delayMicroseconds(micro_delay);
			}
	}
	while (x_can_step || y_can_step || z_can_step);
	 
	                               
 
	current_units.x = target_units.x;
	current_units.y = target_units.y;
	current_units.z = target_units.z;
	calculate_deltas();
}

bool can_step(long current, long target, byte direction)
{
	                            
	if (target == current)
		return false;
  else
	return true;
}

void do_step(byte step_pin)
{
	digitalWrite(step_pin, HIGH);
	delayMicroseconds(5);
	digitalWrite(step_pin, LOW);
}

bool read_switch(byte pin)
{
	                             
	
	if ( SENSORS_INVERTING )
		return !digitalRead(pin) && !digitalRead(pin);
	else
		return digitalRead(pin) && digitalRead(pin);
}

long to_steps(float steps_per_unit, float units)
{
	return steps_per_unit * units;
}

void set_target(float x, float y, float z)
{
	target_units.x = x;
	target_units.y = y;
	target_units.z = z;
	
	calculate_deltas();
}

void set_position(float x, float y, float z)
{
	current_units.x = x;
	current_units.y = y;
	current_units.z = z;
	
	calculate_deltas();
}

void calculate_deltas()
{
	                    
	delta_units.x = fabs(target_units.x - current_units.x);
	delta_units.y = fabs(target_units.y - current_units.y);
	delta_units.z = fabs(target_units.z - current_units.z);
Serial.print("int X_STEPS_PER_MM=");
Serial.println(X_STEPS_PER_MM);
				
	                                          
	current_steps.x = to_steps(x_units, current_units.x);
	current_steps.y = to_steps(y_units, current_units.y);
	current_steps.z = to_steps(z_units, current_units.z);

	target_steps.x = to_steps(x_units, target_units.x);
	target_steps.y = to_steps(y_units, target_units.y);
	target_steps.z = to_steps(z_units, target_units.z);

	delta_steps.x = fabs(target_steps.x - current_steps.x);
	delta_steps.y = fabs(target_steps.y - current_steps.y);
	delta_steps.z = fabs(target_steps.z - current_steps.z);
	
	                       
	x_direction = (target_units.x >= current_units.x);
	y_direction = (target_units.y >= current_units.y);
	z_direction = (target_units.z >= current_units.z);

	                                
	                                       
	                                       
	                                       
}


long calculate_feedrate_delay(float feedrate)
{
	                              
	float distance = sqrt(delta_units.x*delta_units.x + delta_units.y*delta_units.y + delta_units.z*delta_units.z);
	long master_steps = 0;
	
	                         
	if (delta_steps.x > delta_steps.y)
	{
		if (delta_steps.z > delta_steps.x)
			master_steps = delta_steps.z;
		else
			master_steps = delta_steps.x;
	}
	else
	{
		if (delta_steps.z > delta_steps.y)
			master_steps = delta_steps.z;
		else
			master_steps = delta_steps.y;
	}

	                                                                                          
	                                                                       
	                                                                   
	                                                                     
                                  
                                
                                  
                                

                                  
                                
                                  
                                

 
                                               
                                             
                                               
	return ((distance * 60000000.0) / feedrate) / master_steps;	
}

long getMaxSpeed()
{
	if (delta_steps.z > 0)
		return calculate_feedrate_delay(FAST_Z_FEEDRATE);
	else
		return calculate_feedrate_delay(FAST_XY_FEEDRATE);
}

void disable_steppers()
{

     digitalWrite(D0, LOW);digitalWrite(D1, LOW);digitalWrite(D2, LOW);digitalWrite(D3, LOW);
     digitalWrite(D5, LOW);digitalWrite(D6, LOW);digitalWrite(D7, LOW);digitalWrite(D8, LOW);
            
            
	                     
	                                  
	                                  
	                                  
  
}

