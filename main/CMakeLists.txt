idf_component_register(SRCS main.c lcd.c touch.c "ui/MainScreen.c" "ui/RF/RfScreen.c" "ui/IR/IRScreen.c" "ui/IR/Receive/ReceiveIrScreen.c" "ui/IR/Sender/SendIrScreen.c" "ui/IR/DevicesTypes/TV/TVIRScreen.c" "ui/IR/DevicesTypes/Lamp/LampIR.c"  "ui/RFID/RfidScreen.c" "ui/RFID/Send/SendRfidScreen.c" "ui/RFID/Receive/ReceiveRfidScreen.c" "ui/Settings/AddRFID/AddRfidScreen.c"
        "ui/Settings/AddRF/AddrfdispScreen.c" "ui/RF/Send/SendRfScreen.c" "ui/RF/Receive/ReceiveRfScreen.c" "ui/Settings/SettingsScreen.c" "ui/Settings/AddIR/AddIrDispScreen.c" "ui/RF/DevicesTypes/Garage/GarageRfScreen.c" "ui/RF/DevicesTypes/Alarm/AlarmRfScreen.c"
        "ui/RF/Utils/JSONManager/RFDataStore.c" "ESPNOW/espNow.c" "ui/CommonUI/Keyboard.c" "ui/CommonUI/InterfacesUtils.c" "ui/RF/Utils/JSONManager/RFDataRead.c" "ui/RFID/Utils/JSONManager/RfidDataStore.c" "ui/UILibs/CJSONStorage/JSONManager.c" "ui/UILibs/CJSONStorage/Write/WriteJson.c" "ui/UILibs/CJSONStorage/Read/ReadJson.c"
        "ui/UILibs/Popup/Confirmation/ConfirmationPopup.c" "ui/UILibs/Popup/Loading/Loading.c" "ui/RFID/Utils/JSONManager/RFIDDataRead.c" "ui/RF/Utils/Dropdown/RFDropDown.c" "ui/RF/Utils/DispositiveSelectorType/DispositiveSelector.c" "ui/IR/Utils/JSONManager/IRDataRead.c" "ui/IR/Utils/JSONManager/IRDataStore.c"
        "ui/IR/Utils/DispositiveSelector/IRDispositiveSelector.c" "ui/Settings/Reset/Reset.c" "ui/IR/DevicesTypes/AC/AC.c" "ui/IR/Utils/DropDown/IRDropdown.c" "ui/IR/DevicesTypes/Utils/COMMANDS/Send/IRCommandSender.c" "ui/IR/DevicesTypes/Utils/COMMANDS/Receiver/IRCommandReceiver.c" "ui/RF/Utils/Commands/Sender/RFCommandSender.c"
        "ui/RF/Utils/Commands/Receiver/RFCommandReceiver.c"

        INCLUDE_DIRS "."
        REQUIRES esp_lcd driver esp_wifi nvs_flash mqtt spiffs)
