#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

StaticJsonDocument<200> json_payload_send;
StaticJsonDocument<200> json_payload_receive;
String json_string = "";

int tiempos[10];

const char *ssid = "ESP32-TiempoVuelo"; 
const char *password = "1234567890";
const int infrarrojo = 26; //G26 -> 0: lona, 1: aire

int estadoAnterior = 1; // 0: lona, 1: aire
int contadorSaltos = 0;
String comenzo = "no";

unsigned long tiempoPrevio = 0;
const long tiempoEspera = 5;

unsigned long tiempoAterrizaje = 0;
unsigned long tiempoDespegue = 0;
unsigned long tiempoDespegueInicial = 0;
int long tiempoVuelo = 0;
String tiempoVueloStr;
String respaldoTiempos = "";

String webpage = "<!DOCTYPE html> <html> <head> <meta charset=utf-8> <meta name=viewport content='width=device-width'> <title>Tabla TOF - LeTramp</title> </head> <body> <style type=text/css>.main_table{border-collapse:collapse;border-spacing:0}.main_table td{border-color:black;border-style:solid;border-width:1px;font-family:Arial,sans-serif;font-size:14px;overflow:hidden;padding:10px 5px;word-break:normal}.main_table th{border-color:black;border-style:solid;border-width:1px;font-family:Arial,sans-serif;font-size:14px;font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal}.main_table .table_value{text-align:center;vertical-align:top}.main_table .table_headers{font-weight:bold;text-align:center;vertical-align:top}.contenedor_total{display:flex;flex-direction:column;justify-content:left;align-content:center}.contenedor_fondo{align-self:center;display:flex}.contenedor_boton{margin:1ch 1ch 1ch 1ch;align-self:center}.boton{cursor:pointer}.simulador{display:none}.contenedor_titulo{align-self:center;font-family:Arial,sans-serif;font-size:21px;font-weight:bolder;margin:1ch 1ch 1ch 1ch}.contenedor_subtitulo,.contenedor_info_gim{align-self:center;font-family:Arial,sans-serif;font-size:12px;font-weight:bold;margin:1ch 1ch 1ch 1ch;display:flex}.input_type{margin:1ch 1ch 1ch 1ch}.contenedor_tabla{align-self:center}.boton_comenzar{appearance:none;background-color:#000;border:2px solid #1a1a1a;border-radius:15px;box-sizing:border-box;color:#fff;cursor:pointer;display:inline-block;font-family:Roobert,-apple-system,BlinkMacSystemFont,'Segoe UI',Helvetica,Arial,sans-serif,'Apple Color Emoji','Segoe UI Emoji','Segoe UI Symbol';font-size:16px;font-weight:600;line-height:normal;margin:0;min-height:60px;min-width:0;outline:0;padding:16px 24px;text-align:center;text-decoration:none;transition:all 300ms cubic-bezier(.23,1,0.32,1);user-select:none;-webkit-user-select:none;touch-action:manipulation;width:100%;will-change:transform}.boton_comenzar:disabled{pointer-events:none;background-color:#808080}.boton_comenzar:hover{box-shadow:rgba(0,0,0,0.25) 0 8px 15px;transform:translateY(-2px)}.boton_comenzar:active{box-shadow:none;transform:translateY(0)}</style> <div class=contenedor_total> <div class=contenedor_titulo>Tabla ToF - LeTramp</div> <div class=contenedor_subtitulo>Estado de la serie: <span id=estado_serie>NO COMENZADA</span></div> <div class=contenedor_info_gim> <div class=input_type> <div for=fname>Nombre / Orden:</div> <input type=text id=nombre name=nombre> </div> <div class=input_type> <div for=lname>Nivel:</div> <input type=text id=nivel name=nivel> </div> <div class=input_type> <div for=lname>Categoria:</div> <input type=text id=categoria name=categoria> </div> </div> <div class=contenedor_boton> <button id=boton_comenzar class=boton_comenzar>Comenzar Serie</button> </div> <div class='contenedor_boton simulador'> <button id=boton_simular class=boton>Simular Salto</button> </div> <div class=contenedor_tabla> <table class=main_table> <thead> <tr> <th class=table_headers>Nro. Salto</th> <th class=table_headers>Tiempo Salto</th> <th class=table_headers>Tiempo acumulado</th> </tr> </thead> <tbody> <tr> <td class=table_value>1</td> <td id=tiempo_salto_1 class=table_value></td> <td id=tiempo_acumulado_1 class=table_value></td> </tr> <tr> <td class=table_value>2</td> <td id=tiempo_salto_2 class=table_value></td> <td id=tiempo_acumulado_2 class=table_value></td> </tr> <tr> <td class=table_value>3</td> <td id=tiempo_salto_3 class=table_value></td> <td id=tiempo_acumulado_3 class=table_value></td> </tr> <tr> <td class=table_value>4</td> <td id=tiempo_salto_4 class=table_value></td> <td id=tiempo_acumulado_4 class=table_value></td> </tr> <tr> <td class=table_value>5</td> <td id=tiempo_salto_5 class=table_value></td> <td id=tiempo_acumulado_5 class=table_value></td> </tr> <tr> <td class=table_value>6</td> <td id=tiempo_salto_6 class=table_value></td> <td id=tiempo_acumulado_6 class=table_value></td> </tr> <tr> <td class=table_value>7</td> <td id=tiempo_salto_7 class=table_value></td> <td id=tiempo_acumulado_7 class=table_value></td> </tr> <tr> <td class=table_value>8</td> <td id=tiempo_salto_8 class=table_value></td> <td id=tiempo_acumulado_8 class=table_value></td> </tr> <tr> <td class=table_value>9</td> <td id=tiempo_salto_9 class=table_value></td> <td id=tiempo_acumulado_9 class=table_value></td> </tr> <tr> <td class=table_value>10</td> <td id=tiempo_salto_10 class=table_value></td> <td id=tiempo_acumulado_10 class=table_value></td> </tr> </tbody> </table> </div> <div class=contenedor_fondo> <div class=contenedor_boton> <button id=boton_descargar_respaldo class=boton>Descargar Respaldo</button> </div> <div class=contenedor_boton> <button id=boton_reiniciar class=boton>Nueva Serie</button> </div> </div> </div> <script>function recopilarDatosGimnasta(){var infoGimnasta={nombre:document.getElementById('nombre').value.split(' ').join('_'),nivel:document.getElementById('nivel').value.split(' ').join('_'),categoria:document.getElementById('categoria').value.split(' ').join('_')};return infoGimnasta;};function calcularSegundos3Decimales(tiempoVueloMiliSegs){return parseFloat((tiempoVueloMiliSegs/1000)).toFixed(3);};var Socket;function initSocketConn(){Socket=new WebSocket('ws://'+window.location.hostname+':8197/');Socket.onmessage=function(event){var json_payload=JSON.parse(event.data);console.log(json_payload);if(json_payload['estado_serie']==='comenzo'&&json_payload['tiempo']==='no_disponible'){console.log('comenzo');estado_serie='comenzo';numero_saltos=1;document.getElementById('estado_serie').innerText='COMENZADA';}else if(numero_saltos<=10&&!['no_disponible','respaldo','reiniciar'].includes(json_payload['tiempo'])){console.log(parseInt(json_payload['tiempo']));var tiempo_vuelo=calcularSegundos3Decimales(parseInt(json_payload['tiempo']));tiempo_acumulado=parseFloat(tiempo_acumulado)+parseFloat(tiempo_vuelo);infoTiempos[`salto_${numero_saltos}`]={actual:tiempo_vuelo,acumulado:tiempo_acumulado.toFixed(3)};var elemento_salto=document.getElementById(`tiempo_salto_${numero_saltos}`);var elemento_acumulado=document.getElementById(`tiempo_acumulado_${numero_saltos}`);elemento_salto.innerText=tiempo_vuelo;elemento_acumulado.innerText=tiempo_acumulado.toFixed(3);numero_saltos++;};if(json_payload['tiempo']==='respaldo'){var respaldo_tiempos=json_payload['respaldo_tiempos'].split(',');var infoRespaldo={};var total=0;for(var i=0;i<respaldo_tiempos.length;i++){var tiempo_vuelo=calcularSegundos3Decimales(parseInt(respaldo_tiempos[i]));total+=parseFloat(tiempo_vuelo);infoRespaldo[`salto_${i+1}`]=tiempo_vuelo;};infoRespaldo['total']=total.toFixed(3);var info_gimnasta=recopilarDatosGimnasta();saveJsonTable(infoRespaldo,info_gimnasta);} if(numero_saltos==11){document.getElementById('estado_serie').innerText='FINALIZADA';var info_gimnasta=recopilarDatosGimnasta();saveJsonTable(infoTiempos,info_gimnasta);numero_saltos++;};};};window.onload=function(event){initSocketConn();};function comenzarSerie(){json_payload_send={comenzo_serie:'si',reiniciar:'no',descargar_respaldo:'no',nombre:document.getElementById('nombre').value,nivel:document.getElementById('nivel').value,categoria:document.getElementById('categoria').value};Socket.send(JSON.stringify(json_payload_send));document.getElementById('boton_comenzar').setAttribute('disabled','disabled');};function saveJsonTable(info_saltos,info_gimnasta){var data=JSON.stringify(info_saltos);var c=document.createElement('a');c.download=`${info_gimnasta['nombre']}_${info_gimnasta['nivel']}_${info_gimnasta['categoria']}.json`;var t=new Blob([data],{type:'text/plain'});c.href=window.URL.createObjectURL(t);c.click();};function descargarRespaldo(){json_payload_send={comenzo_serie:'no',reiniciar:'no',descargar_respaldo:'si',nombre:'',nivel:'',categoria:''};Socket.send(JSON.stringify(json_payload_send));};function iniciarNuevaSerie(){json_payload_send={comenzo_serie:'no',reiniciar:'si',descargar_respaldo:'no',nombre:'',nivel:'',categoria:''};Socket.send(JSON.stringify(json_payload_send));infoTiempos={};tiempo_acumulado=0.000;numero_saltos=0;estador_serie='no comenzo';json_payload_send={comenzo_serie:'no',reiniciar:'no',descargar_respaldo:'no',nombre:'',nivel:'',categoria:''};document.getElementById('estado_serie').innerText='NO COMENZADA';document.getElementById('nombre').value='';document.getElementById('nivel').value='';document.getElementById('categoria').value='';for(var i=1;i<=10;i++){var elemento_salto=document.getElementById(`tiempo_salto_${i}`);var elemento_acumulado=document.getElementById(`tiempo_acumulado_${i}`);elemento_salto.innerText='';elemento_acumulado.innerText='';};document.getElementById('boton_comenzar').removeAttribute('disabled');};var boton_comenzar=document.getElementById('boton_comenzar');var boton_descargar_respaldo=document.getElementById('boton_descargar_respaldo');var boton_reiniciar=document.getElementById('boton_reiniciar');boton_comenzar.addEventListener('click',comenzarSerie);boton_descargar_respaldo.addEventListener('click',descargarRespaldo);boton_reiniciar.addEventListener('click',iniciarNuevaSerie);var infoTiempos={};var tiempo_acumulado=0.000;var numero_saltos=0;var estado_serie='no comenzo';var json_payload_send={comenzo_serie:'no',reiniciar:'no',descargar_respaldo:'no',nombre:'',nivel:'',categoria:''};</script> </body> </html>";
String observer_page = "<!DOCTYPE html> <html> <head> <meta charset=utf-8> <meta name=viewport content='width=device-width'> <title>Tabla TOF - LeTramp</title> </head> <body> <style type=text/css>.main_table{border-collapse:collapse;border-spacing:0}.main_table td{border-color:black;border-style:solid;border-width:1px;font-family:Arial,sans-serif;font-size:14px;overflow:hidden;padding:10px 5px;word-break:normal}.main_table th{border-color:black;border-style:solid;border-width:1px;font-family:Arial,sans-serif;font-size:14px;font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal}.main_table .table_value{text-align:center;vertical-align:top}.main_table .table_headers{font-weight:bold;text-align:center;vertical-align:top}.contenedor_total{display:flex;flex-direction:column;justify-content:left;align-content:center}.contenedor_fondo{align-self:center;display:flex}.contenedor_boton{margin:1ch 1ch 1ch 1ch;align-self:center}.boton{cursor:pointer}.simulador{display:none}.contenedor_titulo{align-self:center;font-family:Arial,sans-serif;font-size:21px;font-weight:bolder;margin:1ch 1ch 1ch 1ch}.contenedor_subtitulo,.contenedor_info_gim{align-self:center;font-family:Arial,sans-serif;font-size:12px;font-weight:bold;margin:1ch 1ch 1ch 1ch;display:flex}.input_type{margin:1ch 1ch 1ch 1ch;display:flex}.contenedor_tabla{align-self:center}.boton_comenzar{appearance:none;background-color:#000;border:2px solid #1a1a1a;border-radius:15px;box-sizing:border-box;color:#fff;cursor:pointer;display:inline-block;font-family:Roobert,-apple-system,BlinkMacSystemFont,'Segoe UI',Helvetica,Arial,sans-serif,'Apple Color Emoji','Segoe UI Emoji','Segoe UI Symbol';font-size:16px;font-weight:600;line-height:normal;margin:0;min-height:60px;min-width:0;outline:0;padding:16px 24px;text-align:center;text-decoration:none;transition:all 300ms cubic-bezier(.23,1,0.32,1);user-select:none;-webkit-user-select:none;touch-action:manipulation;width:100%;will-change:transform}.boton_comenzar:disabled{pointer-events:none;background-color:#808080}.boton_comenzar:hover{box-shadow:rgba(0,0,0,0.25) 0 8px 15px;transform:translateY(-2px)}.boton_comenzar:active{box-shadow:none;transform:translateY(0)}.info_value{margin-left:.2ch;font-weight:lighter}</style> <div class=contenedor_total> <div class=contenedor_titulo>Tabla ToF - LeTramp</div> <div class=contenedor_subtitulo>Estado de la serie: <span id=estado_serie>NO COMENZADA</span></div> <div class=contenedor_info_gim> <div class=input_type> <div for=fname>Nombre / Orden:</div> <div id=nombre class=info_value></div> </div> <div class=input_type> <div for=lname>Nivel:</div> <div id=nivel class=info_value></div> </div> <div class=input_type> <div for=lname>Categoria:</div> <div id=categoria class=info_value></div> </div> </div> <div class='contenedor_boton simulador'> <button id=boton_simular class=boton>Simular Salto</button> </div> <div class=contenedor_tabla> <table class=main_table> <thead> <tr> <th class=table_headers>Nro. Salto</th> <th class=table_headers>Tiempo Salto</th> <th class=table_headers>Tiempo acumulado</th> </tr> </thead> <tbody> <tr> <td class=table_value>1</td> <td id=tiempo_salto_1 class=table_value></td> <td id=tiempo_acumulado_1 class=table_value></td> </tr> <tr> <td class=table_value>2</td> <td id=tiempo_salto_2 class=table_value></td> <td id=tiempo_acumulado_2 class=table_value></td> </tr> <tr> <td class=table_value>3</td> <td id=tiempo_salto_3 class=table_value></td> <td id=tiempo_acumulado_3 class=table_value></td> </tr> <tr> <td class=table_value>4</td> <td id=tiempo_salto_4 class=table_value></td> <td id=tiempo_acumulado_4 class=table_value></td> </tr> <tr> <td class=table_value>5</td> <td id=tiempo_salto_5 class=table_value></td> <td id=tiempo_acumulado_5 class=table_value></td> </tr> <tr> <td class=table_value>6</td> <td id=tiempo_salto_6 class=table_value></td> <td id=tiempo_acumulado_6 class=table_value></td> </tr> <tr> <td class=table_value>7</td> <td id=tiempo_salto_7 class=table_value></td> <td id=tiempo_acumulado_7 class=table_value></td> </tr> <tr> <td class=table_value>8</td> <td id=tiempo_salto_8 class=table_value></td> <td id=tiempo_acumulado_8 class=table_value></td> </tr> <tr> <td class=table_value>9</td> <td id=tiempo_salto_9 class=table_value></td> <td id=tiempo_acumulado_9 class=table_value></td> </tr> <tr> <td class=table_value>10</td> <td id=tiempo_salto_10 class=table_value></td> <td id=tiempo_acumulado_10 class=table_value></td> </tr> </tbody> </table> </div> </div> <script>function calcularSegundos3Decimales(tiempoVueloMiliSegs){return parseFloat((tiempoVueloMiliSegs/1000)).toFixed(3);};var Socket;function initSocketConn(){Socket=new WebSocket('ws://'+window.location.hostname+':8197/');Socket.onmessage=function(event){var json_payload=JSON.parse(event.data);console.log(json_payload);if(json_payload['reiniciar']==='si'){iniciarNuevaSerie();return 0;};if(json_payload['estado_serie']==='comenzo'&&json_payload['tiempo']==='no_disponible'){console.log('comenzo');estado_serie='comenzo';numero_saltos=1;document.getElementById('estado_serie').innerText='COMENZADA';var info=json_payload['info_gimnasta'].split(',');document.getElementById('nombre').innerText=info[0];document.getElementById('nivel').innerText=info[1];document.getElementById('categoria').innerText=info[2];}else if(numero_saltos<=10&&(json_payload['tiempo']!=='no_disponible'&&json_payload['tiempo']!=='respaldo')){console.log(parseInt(json_payload['tiempo']));var tiempo_vuelo=calcularSegundos3Decimales(parseInt(json_payload['tiempo']));tiempo_acumulado=parseFloat(tiempo_acumulado)+parseFloat(tiempo_vuelo);infoTiempos[`salto_${numero_saltos}`]={actual:tiempo_vuelo,acumulado:tiempo_acumulado.toFixed(3)};var elemento_salto=document.getElementById(`tiempo_salto_${numero_saltos}`);var elemento_acumulado=document.getElementById(`tiempo_acumulado_${numero_saltos}`);elemento_salto.innerText=tiempo_vuelo;elemento_acumulado.innerText=tiempo_acumulado.toFixed(3);numero_saltos++;} if(numero_saltos==11){document.getElementById('estado_serie').innerText='FINALIZADA';numero_saltos++;};};};window.onload=function(event){initSocketConn();};function iniciarNuevaSerie(){infoTiempos={};tiempo_acumulado=0.000;numero_saltos=0;estador_serie='no comenzo';json_payload_send={comenzo_serie:'no',reiniciar:'no',descargar_respaldo:'no'};document.getElementById('estado_serie').innerText='NO COMENZADA';document.getElementById('nombre').innerText='';document.getElementById('nivel').innerText='';document.getElementById('categoria').innerText='';for(var i=1;i<=10;i++){var elemento_salto=document.getElementById(`tiempo_salto_${i}`);var elemento_acumulado=document.getElementById(`tiempo_acumulado_${i}`);elemento_salto.innerText='';elemento_acumulado.innerText='';};};var infoTiempos={};var tiempo_acumulado=0.000;var numero_saltos=0;var estado_serie='no comenzo';var json_payload_send={comenzo_serie:'no',reiniciar:'no',descargar_respaldo:'no'};</script> </body> </html>";

AsyncWebServer server(8097);
WebSocketsServer webSocket = WebSocketsServer(8197);


void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            }
            break;
        case WStype_TEXT:
              { 
                Serial.printf("[%u] get Text: %s\n", num, payload);
                DeserializationError error = deserializeJson(json_payload_receive, payload);
                if (error){
                  Serial.print("deserializeJson() failed!!");
                  return;
                } else {
                  const char* reiniciar = json_payload_receive["reiniciar"];
                  const char* descargar_respaldo = json_payload_receive["descargar_respaldo"];
                  const char* comenzo_serie = json_payload_receive["comenzo_serie"];
                  const char* nombre = json_payload_receive["nombre"];
                  const char* nivel = json_payload_receive["nivel"];
                  const char* categoria = json_payload_receive["categoria"];
                  Serial.println(String(nombre) + "," + String(nivel) + "," + String(categoria));

                  if (String(comenzo_serie) == "si") {
                  comenzo = "si";
                  contadorSaltos ++;
                  json_string = "";
                  JsonObject object = json_payload_send.to<JsonObject>();
                  object["estado_serie"] = "comenzo";
                  object["tiempo"] = "no_disponible";
                  object["respaldo_tiempos"] = "no_solicitado";
                  object["reiniciar"] = "no";
                  object["info_gimnasta"] = String(nombre) + "," + String(nivel) + "," + String(categoria);
                  serializeJson(json_payload_send, json_string);
                  Serial.println(json_string);
                  webSocket.broadcastTXT(json_string);
                };
                if (String(descargar_respaldo) == "si") {
                  Serial.println("descargaaaar");
                  respaldoTiempos = "";
                  for (int i = 0; i <= 9; i++){
                    if (i == 0) {
                      respaldoTiempos = respaldoTiempos + tiempos[i];
                      } else {
                       respaldoTiempos = respaldoTiempos + "," + tiempos[i];
                      }
                    
                    };
                  json_string = "";
                  JsonObject object = json_payload_send.to<JsonObject>();
                  object["estado_serie"] = "no_comenzo";
                  object["tiempo"] = "respaldo";
                  object["respaldo_tiempos"] = respaldoTiempos;
                  object["reiniciar"] = "no";
                  object["info_gimnasta"] = "";
                  serializeJson(json_payload_send, json_string);
                  Serial.println(json_string);
                  webSocket.broadcastTXT(json_string);
                    
                };
                if (String(reiniciar) == "si"){
                  Serial.println("reiniiiiciar");
                  estadoAnterior = 1; // 0: lona, 1: aire
                  contadorSaltos = 0;
                  comenzo = "no";
                  tiempoPrevio = 0;
                  json_string = "";
                  JsonObject object = json_payload_send.to<JsonObject>();
                  object["estado_serie"] = "reiniciar";
                  object["tiempo"] = "reiniciar";
                  object["respaldo_tiempos"] = "reiniciar";
                  object["reiniciar"] = "si";
                  object["info_gimnasta"] = "";
                  serializeJson(json_payload_send, json_string);
                  Serial.println(json_string);
                  webSocket.broadcastTXT(json_string);
                  
                  
                  
                };
                }
                              
              }
            break;

    }
}

// callback: send home observer page
void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  request->send(200, "text/html", observer_page);
}

// callback: send admin page
void onAdminRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  request->send(200, "text/html", webpage);
}

// callback: NOT FOUND page
void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  request->send(404, "text/plain", "Not Found");
}


void setup() {
  
  // iniciar el sensor y la comunicacion con el serial
  pinMode(infrarrojo, INPUT);
  Serial.begin(115200);

  // inicial el ESP como access point
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());

  // manejo de los dos estados definidos de la aplicacion web
  server.on("/admin-le-tramp", HTTP_GET, onAdminRequest);
  server.on("/", HTTP_GET, onIndexRequest);
  server.onNotFound(onPageNotFound);
  
  // inicio del servidor web
  server.begin();
  Serial.println("HTTP server started");

  // inicio del servidor de websockets y manejo de eventos del mismo
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);

}

void loop(void) {
  // manejando la data del websocket
  webSocket.loop();

  if (comenzo == "no" && (millis() - tiempoPrevio) >= tiempoEspera) {
    tiempoPrevio = millis();
    if (digitalRead(infrarrojo) == 0 && estadoAnterior == 1) {
      Serial.println("aterrizo - no comenzo");
      estadoAnterior = 0;
    }
    if (digitalRead(infrarrojo) == 1 && estadoAnterior == 0){
      Serial.println("despego - no comenzo");
      tiempoDespegueInicial = millis();
      estadoAnterior = 1;
    }
  }

  if (comenzo == "si" && (millis() - tiempoPrevio) >= tiempoEspera) {
    tiempoPrevio = millis();
    
    if (digitalRead(infrarrojo) == 0 && estadoAnterior == 1 && contadorSaltos < 11){
      tiempoAterrizaje = millis();
      if (contadorSaltos == 1) {
        tiempoVuelo = tiempoAterrizaje - tiempoDespegueInicial;
      } else {
        tiempoVuelo = tiempoAterrizaje - tiempoDespegue;  
      }
      tiempoVueloStr = String(tiempoVuelo);
      json_string = "";
      JsonObject object = json_payload_send.to<JsonObject>();
      object["estado_serie"] = "comenzo";
      object["tiempo"] = tiempoVueloStr;
      object["respaldo_tiempos"] = "no_solicitado";
      object["reiniciar"] = "no";
      object["info_gimnasta"] = "";
      serializeJson(json_payload_send, json_string);
      Serial.println(json_string);
      webSocket.broadcastTXT(json_string);
      tiempos[contadorSaltos - 1] = tiempoVuelo;
      Serial.println("aterrizo:");
      Serial.println(tiempoVuelo);
      Serial.println(contadorSaltos);
      Serial.println();
      estadoAnterior = 0;
      contadorSaltos ++;
    }
    if (digitalRead(infrarrojo) == 1 && estadoAnterior == 0){
        tiempoDespegue = millis();
        Serial.println("despego");
        Serial.println();
        estadoAnterior = 1;
    }
  }



}
