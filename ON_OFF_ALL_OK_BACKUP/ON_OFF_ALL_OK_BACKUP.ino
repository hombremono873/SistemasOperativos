#include <WiFi.h>

const char* ssid = "TERESA";
const char* password = "Laila**88";

const int maquinas[] = {13, 12, 14, 27, 26, 25, 33, 32};
const int numMaquinas = sizeof(maquinas) / sizeof(maquinas[0]);

WiFiServer server(80);
const char* nombreEquipos[] = {"Alumbrado", "Horno FC01", "Banda CV03", "Ventilador Fa32 Fondo del Horno", "Bomba PP321", "Bomba de Emergencia PP333", "Alarma Contra incendio", "Emergencia CCC"};
const char* nombresEquipo[] = {"Yuly Alvear", "Laidy Castaño", "Omar Torres"};


void setup() {
  Serial.begin(115200);
  delay(10);
 
  // Configurar pines de las máquinas como salidas y apagarlas inicialmente
  for (int i = 0; i < numMaquinas; i++) {
    pinMode(maquinas[i], OUTPUT);
    digitalWrite(maquinas[i], LOW);
  }

  // Conectar a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a la red WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Iniciar el servidor
  server.begin();

 }

void loop() {
  manejarCliente();
}

void procesarPeticion(const String& peticion) {
  // Variable para controlar si se cumplen todas las condiciones del Horno FC01
  bool condicionesHornoCumplidas = true;

  // Encender o apagar salidas individuales
  for (int i = 0; i < numMaquinas; i++) {
    String On = "/on" + String(i);
    String Off = "/off" + String(i);

    // Lógica adicional para el Horno FC01
    if (i == 1 && peticion.indexOf(On) != -1) {
      // Verificar que el "Ventilador Fa32 del fondo del Horno" esté ON
      if (digitalRead(maquinas[3]) == LOW) {
        Serial.println("Error: El Ventilador Fa32 del fondo del Horno debe estar encendido para activar el Horno FC01.");
        condicionesHornoCumplidas = false;
      }

      // Verificar que la "Bomba PP32" o la "Bomba de Emergencia PP332" estén ON
      if (digitalRead(maquinas[4]) == LOW && digitalRead(maquinas[5]) == LOW) {
        Serial.println("Error: La Bomba PP32 o la Bomba de Emergencia PP332 deben estar encendidas para activar el Horno FC01.");
        condicionesHornoCumplidas = false;
      }
    }

    if (peticion.indexOf(On) != -1) {
      energizarMaquina(i);
    } else if (peticion.indexOf(Off) != -1) {
      apagarMaquina(i);

      // Si se apaga una máquina relacionada con el Horno FC01, apagar el Horno también
      if (i == 3 || i == 4 || i == 5) {
        condicionesHornoCumplidas = false;
      }
    }
  }

  // Apagar el Horno FC01 si alguna de las condiciones no se cumple
  if (!condicionesHornoCumplidas) {
    apagarMaquina(1);  // Apagar el Horno FC01
  }
}

// Función principal para manejar al cliente
void manejarCliente() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Nuevo cliente conectado");

    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.println(request);

        // Procesar la solicitud
        procesarPeticion(request);

        // Enviar el estado de las máquinas al cliente
        enviarEstadoMaquina(client);

        delay(1);
        client.stop();
        Serial.println("Cliente desconectado");
      }
    }
  }
}

// Función para energizar una máquina en la posición 'Index'
void energizarMaquina(int Index) {
  digitalWrite(maquinas[Index], HIGH);
}

// Función para apagar una máquina en la posición 'Index'
void apagarMaquina(int Index) {
  digitalWrite(maquinas[Index], LOW);
}

// Función para enviar el estado de todas las máquinas al cliente
void enviarEstadoMaquina(WiFiClient& client) {
  // Configurar la respuesta HTTP
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.println("<html>");
  client.println("<head><meta name='viewport' content='width=device-width, initial-scale=1.0'><style>");
  client.println("body { font-family: Arial, sans-serif; text-align: center; background-color: black; color: white;}");
  client.println("h1 { text-shadow: 2px 2px 4px black; }");
  client.println("button { background-color: #4caf50; color: white; padding: 15px 20px; font-size: 16px; border: none; margin: 10px; cursor: pointer; }");
  client.println(".group { display: flex; justify-content: space-around; flex-wrap: wrap; }");
  client.println(".device { margin: 20px; width: 40%; border: 2px solid #4caf50; padding: 10px; border-radius: 10px; background-color: transparent;}");
  client.println(".author-info { position: absolute; top: 10px; left: 70px; text-align: left; color: white; font-size: 12px; }");
  client.println("</style></head>");
  client.println("<body>");
  client.println("<div class='author-info'>");
  for (int i = 0; i < sizeof(nombresEquipo) / sizeof(nombresEquipo[0]); i++) {
    client.println("<p>" + String(nombresEquipo[i]) + "</p>");
  }
  client.println("</div>");
  client.println("<h1>Cuarto de Control Central</h1>");

  // Organizar en dos grupos
  client.println("<div class='group'>");
  for (int i = 0; i < numMaquinas; i++) {
    enviarEstadoEquipos(client, i);
  }
  client.println("</div>");

  client.println("</body></html>");
}
void enviarEstadoEquipos(WiFiClient& client, int index) {
  client.println("<div class='device'>");
  client.print("<h2 style='color: white;'>");
  client.print(nombreEquipos[index]);
  client.print(": ");
  client.print(digitalRead(maquinas[index]) ? "On" : "Off");
  client.println("</h2>");
  client.println("<form>");
  client.println("<button style='border-radius: 10px;' type='button' onclick=\"location.href='/on" + String(index) + "'\">Encender</button>");
  client.println("<button style='border-radius: 10px;' type='button' onclick=\"location.href='/off" + String(index) + "'\">Apagar</button>");
  client.println("</form>");
  client.println("</div>");
}