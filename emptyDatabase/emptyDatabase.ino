#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>

//Library Wifi dan OTA Esp32s
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>




// Set these to run example.
#define FIREBASE_HOST "https://presensi-3d6a7.firebaseio.com/"
#define FIREBASE_AUTH "dVuq28kSJxVht9EN9Ej9bWsi5KDeo8Z4WFAvYNmX"

#define WIFI_SSID "ssid"
#define WIFI_PASSWORD "986pswdr"

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
String nam;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
HardwareSerial mySerial(2);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;

const char* host = "esp32mertani";

WebServer server(80);
///////////////////////////////////////////////////LAYOUT PAGE LOGIN OTA///////////////////////////////////////////////////////////////////

/* Style */
String style =
"<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
"input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
"#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
"#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
"form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

/* Login page */
String loginIndex = 
"<form name=loginForm>" // Nama Form Login
"<h1>MERTANI</h1>"
"<input name=userid placeholder='User ID'> "
"<input name=pwd placeholder=Password type=Password> "
"<input type=submit onclick=check(this.form) class=btn value=Login></form>"
"<script>"
"function check(form) {"
"if(form.userid.value=='mertani' && form.pwd.value=='')" //Username & Password Login OTA
"{window.open('/serverIndex')}"
"else"
"{alert('Error Password or Username')}" // Ketika Gagal Login
"}"
"</script>" + style;
 
/* Server Index Page */
String serverIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
"<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
"<label id='file-input' for='file'>   Choose file...</label>"
"<input type='submit' class=btn value='Update'>"
"<br><br>"
"<div id='prg'></div>"
"<br><div id='prgbar'><div id='bar'></div></div><br></form>"
"<script>"
"function sub(obj){"
"var fileName = obj.value.split('\\\\');"
"document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
"};"
"$('form').submit(function(e){"
"e.preventDefault();"
"var form = $('#upload_form')[0];"
"var data = new FormData(form);"
"$.ajax({"
"url: '/update',"
"type: 'POST',"
"data: data,"
"contentType: false,"
"processData:false,"
"xhr: function() {"
"var xhr = new window.XMLHttpRequest();"
"xhr.upload.addEventListener('progress', function(evt) {"
"if (evt.lengthComputable) {"
"var per = evt.loaded / evt.total;"
"$('#prg').html('progress: ' + Math.round(per*100) + '%');"
"$('#bar').css('width',Math.round(per*100) + '%');"
"}"
"}, false);"
"return xhr;"
"},"
"success:function(d, s) {"
"console.log('success!') "
"},"
"error: function (a, b, c) {"
"}"
"});"
"});"
"</script>" + style;

//////////////////////////////////////////////////////FINISHED LAYOUT PAGE LOGIN///////////////////////////////////////////////////////////

void setup()  
{
   Serial.begin(9600);
  lcd.init();                   
  lcd.backlight();
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.setCursor(0, 0);
  lcd.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    lcd.setCursor(0, 1);
    lcd.print("........");
    delay(500);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(2000);
  lcd.clear();
  
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  finger.begin(57600);
  finger.getTemplateCount();
  lcd.setCursor(0, 0);
  lcd.print("Mode :");
  lcd.setCursor(0,1);
  lcd.print("Menghapus Data ");
  delay(2000);
  lcd.clear();
  SetupOta();
  finger.emptyDatabase();
  lcd.setCursor(0, 0);
  lcd.print("Data Berhasil");
  lcd.setCursor(0,1);
  lcd.print("Dihapus ");
}

void loop() {
  server.handleClient();
}
