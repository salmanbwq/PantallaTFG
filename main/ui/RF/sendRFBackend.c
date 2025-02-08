//
// Created by salman on 25/01/25.
//

//static char searchElement(char id);

/**
 * Funcion para enviar comando por esp-now.
 * P.e sendRF/Freq/Comnando
 */

/*
void sendRFBackend(char id) {
    char *command = "sendRf" + searchElement(id) != "Device not found" ? searchElement(id) : "";

    esp_now_send_data(lcd, (uint8_t *) command, strlen(command));
}

static char searchElement(char id) {
    static char result[64]; // Buffer estático para almacenar la cadena resultante

    for (int i = 0; i < MAX_DEVICES; i++) {
        if (strcmp(devicesRf[i].id, id) == 0) {
            // Concatenar frecuencia y comando con "/"
            snprintf(result, sizeof(result), "/%s/%s", devicesRf[i].frequency, devicesRf[i].command);
            return result; // Retornar la cadena concatenada
        }
    }

    // Si no se encuentra, devolver un mensaje genérico
    return "Device not found";
}

static char searchCommand(DeviceRF *devicesRf, char *label) {
    for (int i = 0; i < MAX_COMMANDS; i++) {
        if (strcmp(devicesRf->command[i], label) == 0) {

        }
    }
}
*/
