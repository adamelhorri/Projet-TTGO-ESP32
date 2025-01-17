#include "globals.h"
#include "cors_module.h"

void addCorsHeaders() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, Content-Type, Accept");
}

void handleCors() {
  if (server.method() == HTTP_OPTIONS) {
    addCorsHeaders();
    server.send(200, "text/plain", "");
  }
}
