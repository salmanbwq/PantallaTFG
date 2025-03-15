//
// Created by salman on 21/02/25.
//

#ifndef IRDATAREAD_H
#define IRDATAREAD_H

/**
 * Method to get Device type
 * @param name Device name
 * @return Device type
 */
char *getIRDeviceType(char *name);

/**
 * Method to get IR Device Command
 * @param deviceName Device Name
 * @param commandName Command name
 * @return Command content
 */
char *getIRCommand(char *deviceName, const char *commandName);


#endif //IRDATAREAD_H
