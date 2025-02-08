//
// Created by salman on 25/01/25.
//
/*
static char getDispCommand(char *id);

static char *extractId(char *input);


 * Funcion para enviar clave por Rfid.
 * Formato: sendRfid/commando



void sendRfidBackend(char *name) {
    char *command = "sendRfid/" + getDispCommand(name);

    esp_now_send_data(lcd, (uint8_t *) command, strlen(command));
}

static char getDispCommand(char *id) {
    for (int i = 0; i < MAX_DEVICES; i++) {
        if (strcmp(devicesRfid[i].id, extractId(id)) == 0) {
            return devicesRfid[i].command;
        }
    }
    return "vacio";
}

static char *extractId(char *input) {
    static char id[32]; // Buffer estático para devolver el id
    char temp[64]; // Copia temporal de la cadena original

    strncpy(temp, input, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0'; // Asegurar terminación nula

    // Usar strtok para dividir la cadena
    char *token = strtok(temp, "#"); // Primera parte (name)
    if (token != NULL) {
        token = strtok(NULL, "#"); // Segunda parte (id)
        if (token != NULL) {
            strncpy(id, token, sizeof(id));
            id[sizeof(id) - 1] = '\0'; // Asegurar terminación nula
            return id; // Retornar el id extraído
        }
    }

    return NULL; // Si no se encuentra el delimitador o id, devolver NULL
}
*/