#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "NombreDeTuRed";
const char *password = "TuClaveDeRed";

AsyncWebServer server(80);

// Variables globales para el estado del dispositivo
bool lucesEncendidas = false;
bool motorEncendido = false;

void configurarWifi() {
  // Configuración del Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a la red WiFi");
  Serial.println("Dirección IP: " + WiFi.localIP().toString());
}

void manejarSolicitud(AsyncWebServerRequest *request) {
  // Manejo de solicitudes
  if (request->hasParam("accion")) {
    String accion = request->getParam("accion")->value();
    if (accion == "encenderLuces") {
      lucesEncendidas = true;
    } else if (accion == "apagarLuces") {
      lucesEncendidas = false;
    } else if (accion == "arrancarMotor") {
      motorEncendido = true;
    } else if (accion == "apagarMotor") {
      motorEncendido = false;
    }
  }

  // Respuesta al cliente (puedes personalizar esto según tus necesidades)
  request->send(200, "text/plain", "Solicitud recibida");
}

void configurarPaginaWeb() {
  // Configuración de la página web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><head>";
    html += "<style>body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }";
    html += "h1 { color: #333; }</style></head><body>";
    html += "<h1>Control del Dispositivo</h1>";

    // Formulario para enviar acciones al ESP32
    html += "<form action='/' method='get'>";
    html += "<p><button type='submit' name='accion' value='encenderLuces'>Encender Luces</button></p>";
    html += "<p><button type='submit' name='accion' value='apagarLuces'>Apagar Luces</button></p>";
    html += "<p><button type='submit' name='accion' value='arrancarMotor'>Arrancar Motor</button></p>";
    html += "<p><button type='submit' name='accion' value='apagarMotor'>Apagar Motor</button></p>";
    html += "</form>";

    html += "</body></html>";

    request->send(200, "text/html", html);
  });
}

void procesarAcciones() {
  // Procesar las acciones basadas en el estado del dispositivo
  if (lucesEncendidas) {
    // Realizar acciones cuando las luces están encendidas
    Serial.println("Luces encendidas");
    // ...
  } else {
    // Realizar acciones cuando las luces están apagadas
    Serial.println("Luces apagadas");
    // ...
  }

  if (motorEncendido) {
    // Realizar acciones cuando el motor está encendido
    Serial.println("Motor encendido");
    // ...
  } else {
    // Realizar acciones cuando el motor está apagado
    Serial.println("Motor apagado");
    // ...
  }
}

void setup() {
  Serial.begin(115200);
  configurarWifi();
  configurarPaginaWeb();

  server.begin();
}

void loop() {
  // Procesar acciones continuas basadas en el estado de la página web
  procesarAcciones();

  // Otras tareas del bucle loop
  delay(1000);
}