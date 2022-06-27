#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>


const char* ssid = "fibertewifi800";
const char* password = "argentina22";

const int infrarrojo = 26; //G26

String comenzo = "no";


WebServer server(8097);
WebSocketsServer webSocket = WebSocketsServer(8197);


String webpage = "<!DOCTYPE html><html><head><meta charset=utf-8><meta name=viewport content='width=device-width'><title>Tabla TOF - LeTramp</title></head><body><style type=text/css>.main_table{border-collapse:collapse;border-spacing:0}.main_table td{border-color:black;border-style:solid;border-width:1px;font-family:Arial,sans-serif;font-size:14px;overflow:hidden;padding:10px 5px;word-break:normal}.main_table th{border-color:black;border-style:solid;border-width:1px;font-family:Arial,sans-serif;font-size:14px;font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal}.main_table .table_value{text-align:center;vertical-align:top}.main_table .table_headers{font-weight:bold;text-align:center;vertical-align:top}.contenedor_total{display:flex;flex-direction:column;justify-content:left;align-content:center}.contenedor_boton{margin:1ch 1ch 1ch 1ch;align-self:center}.boton{cursor:pointer}.simulador{display:none}.contenedor_titulo{align-self:center;font-family:Arial,sans-serif;font-size:21px;font-weight:bolder;margin:1ch 1ch 1ch 1ch}.contenedor_subtitulo,.contenedor_info_gim{align-self:center;font-family:Arial,sans-serif;font-size:12px;font-weight:bold;margin:1ch 1ch 1ch 1ch}.contenedor_tabla{align-self:center}.boton_comenzar{appearance:none;background-color:#000;border:2px solid #1a1a1a;border-radius:15px;box-sizing:border-box;color:#fff;cursor:pointer;display:inline-block;font-family:Roobert,-apple-system,BlinkMacSystemFont,'Segoe UI',Helvetica,Arial,sans-serif,'Apple Color Emoji','Segoe UI Emoji','Segoe UI Symbol';font-size:16px;font-weight:600;line-height:normal;margin:0;min-height:60px;min-width:0;outline:0;padding:16px 24px;text-align:center;text-decoration:none;transition:all 300ms cubic-bezier(.23,1,0.32,1);user-select:none;-webkit-user-select:none;touch-action:manipulation;width:100%;will-change:transform}.boton_comenzar:disabled{pointer-events:none;background-color:#808080}.boton_comenzar:hover{box-shadow:rgba(0,0,0,0.25) 0 8px 15px;transform:translateY(-2px)}.boton_comenzar:active{box-shadow:none;transform:translateY(0)}</style><div class=contenedor_total><div class=contenedor_titulo>Tabla ToF - LeTramp</div><div class=contenedor_subtitulo>Estado de la serie: <span id=estado_serie>NO COMENZADA</span></div><div class=contenedor_info_gim><div for=fname>Nombre completo:</div><input type=text id=nombre name=nombre><div for=lname>Edad:</div><input type=text id=edad name=edad><div for=lname>Categoria:</div><input type=text id=categoria name=categoria></div><div class=contenedor_boton><button id=boton_comenzar class=boton_comenzar>Comenzar Serie</button></div><div class='contenedor_boton simulador'><button id=boton_simular class=boton>Simular Salto</button></div><div class=contenedor_tabla><table class=main_table><thead><tr><th class=table_headers>Nro. Salto</th><th class=table_headers>Tiempo Salto</th><th class=table_headers>Tiempo acumulado</th></tr></thead><tbody><tr><td class=table_value>1</td><td id=tiempo_salto_1 class=table_value></td><td id=tiempo_acumulado_1 class=table_value></td></tr><tr><td class=table_value>2</td><td id=tiempo_salto_2 class=table_value></td><td id=tiempo_acumulado_2 class=table_value></td></tr><tr><td class=table_value>3</td><td id=tiempo_salto_3 class=table_value></td><td id=tiempo_acumulado_3 class=table_value></td></tr><tr><td class=table_value>4</td><td id=tiempo_salto_4 class=table_value></td><td id=tiempo_acumulado_4 class=table_value></td></tr><tr><td class=table_value>5</td><td id=tiempo_salto_5 class=table_value></td><td id=tiempo_acumulado_5 class=table_value></td></tr><tr><td class=table_value>6</td><td id=tiempo_salto_6 class=table_value></td><td id=tiempo_acumulado_6 class=table_value></td></tr><tr><td class=table_value>7</td><td id=tiempo_salto_7 class=table_value></td><td id=tiempo_acumulado_7 class=table_value></td></tr><tr><td class=table_value>8</td><td id=tiempo_salto_8 class=table_value></td><td id=tiempo_acumulado_8 class=table_value></td></tr><tr><td class=table_value>9</td><td id=tiempo_salto_9 class=table_value></td><td id=tiempo_acumulado_9 class=table_value></td></tr><tr><td class=table_value>10</td><td id=tiempo_salto_10 class=table_value></td><td id=tiempo_acumulado_10 class=table_value></td></tr></tbody></table></div></div><script>function recopilarDatosGimnasta(){var infoGimnasta={nombre:document.getElementById('nombre').value.split(' ').join('_'),edad:document.getElementById('edad').value.split(' ').join('_'),categoria:document.getElementById('categoria').value.split(' ').join('_')};return infoGimnasta;};function comenzarTiempo(){if(numero_saltos===0){numero_saltos=1;t0=medir_tiempo_2_decimales();document.getElementById('estado_serie').innerText='COMENZADA';boton_comenzar.setAttribute('disabled','disabled');}};function medir_tiempo_2_decimales(){return((new Date().getTime())/1000).toFixed(2);};function restar_a_dos_decimales(tmenor,tmayor){return(Math.round(((parseFloat(tmayor)-parseFloat(tmenor))*100))/100).toFixed(2);};function simularSalto(){if(numero_saltos===1){var t1=medir_tiempo_2_decimales();infoTiempos[`salto_${numero_saltos}`]={actual:restar_a_dos_decimales(t0,t1),acumulado:restar_a_dos_decimales(t0,t1)};var elemento_salto=document.getElementById(`tiempo_salto_${numero_saltos}`);var elemento_acumulado=document.getElementById(`tiempo_acumulado_${numero_saltos}`);elemento_salto.innerText=infoTiempos[`salto_${numero_saltos}`]['actual'];elemento_acumulado.innerText=infoTiempos[`salto_${numero_saltos}`]['acumulado'];numero_saltos+=1;}else if(numero_saltos>1&&numero_saltos<11){var t1=medir_tiempo_2_decimales();infoTiempos[`salto_${numero_saltos}`]={actual:restar_a_dos_decimales(infoTiempos[`salto_${numero_saltos-1}`]['acumulado'],restar_a_dos_decimales(t0,t1)),acumulado:restar_a_dos_decimales(t0,t1)};var elemento_salto=document.getElementById(`tiempo_salto_${numero_saltos}`);var elemento_acumulado=document.getElementById(`tiempo_acumulado_${numero_saltos}`);elemento_salto.innerText=infoTiempos[`salto_${numero_saltos}`]['actual'];elemento_acumulado.innerText=infoTiempos[`salto_${numero_saltos}`]['acumulado'];numero_saltos+=1;} if(numero_saltos===11){document.getElementById('estado_serie').innerText='FINALIZADA';var infoGimnastaFinal=recopilarDatosGimnasta();saveJsonTable(infoTiempos,infoGimnastaFinal);numero_saltos+=1;}};var Socket;function initSocketConn(){Socket=new WebSocket('ws://'+window.location.hostname+':8197/');Socket.onmessage=function(event){processCommand(event);if(event.data==='toco'&&estado_serie==='no comenzo'){t0=medir_tiempo_2_decimales();}else if(event.data==='toco'&&estado_serie==='comenzo'){simularSalto();};if(event.data==='comenzo'){estado_serie='comenzo';numero_saltos=1;document.getElementById('estado_serie').innerText='COMENZADA';};};};function processCommand(event){console.log(`Sensor:${event.data}.Estado serie:${estado_serie}`);};window.onload=function(event){initSocketConn();};function comenzarSerie(){Socket.send('comenzo');document.getElementById('boton_comenzar').setAttribute('disabled','disabled');};function saveJsonTable(info_saltos,info_gimnasta){var data=JSON.stringify(info_saltos);var c=document.createElement('a');c.download=`${info_gimnasta['nombre']}_${info_gimnasta['edad']}_${info_gimnasta['categoria']}.json`;var t=new Blob([data],{type:'text/plain'});c.href=window.URL.createObjectURL(t);c.click();};var boton_comenzar=document.getElementById('boton_comenzar');boton_comenzar.addEventListener('click',comenzarSerie);var infoTiempos={};var numero_saltos=0;var estado_serie='no comenzo';</script></body></html>";

void handleRoot() {
  server.send(200, "text/html", webpage);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  server.send(404, "text/plain", message);
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

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
                comenzo = "si";
                webSocket.broadcastTXT("comenzo");
              }
            break;

    }

}


void setup() {
  pinMode(infrarrojo, INPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "new endpoint");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

}

void loop(void) {
  server.handleClient();
  webSocket.loop();
  if (digitalRead(infrarrojo) == 0){
                webSocket.broadcastTXT("toco");
    }
  delay(100);//allow the cpu to switch to other tasks
}
