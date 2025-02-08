idf_component_register(SRCS demo.c lcd.c touch.c "ui/MainScreen.c" "ui/RF/RfScreen.c" "ui/IR/IRScreen.c" "ui/IR/ReceiveIrScreen.c" "ui/IR/SendIrScreen.c" "ui/IR/SendTvIrScreen.c" "ui/RFID/RfidScreen.c" "ui/RFID/SendRfidScreen.c" "ui/RFID/ReceiveRfidScreen.c" "ui/Settings/AddRfidScreen.c"
        "ui/Settings/AddrfdispScreen.c" "ui/RF/SendRfScreen.c" "ui/RF/ReceiveRfScreen.c" "ui/Settings/SettingsScreen.c" "ui/Settings/AddIrDispScreen.c" "ui/RF/GarageRfScreen.c" "ui/RF/LightRfScreen.c" "ui/RF/AlarmRfScreen.c" "ui/RF/sendRFBackend.c" "ui/RFID/sendRfidBackend.c"
        "ui/RF/RFDataStore.c" "ESPNOW/espNow.c" "ui/GeneralObjects/Keyboard.c" "ui/GeneralObjects/InterfacesUtils.c"
        INCLUDE_DIRS "."
        REQUIRES esp_lcd driver esp_wifi nvs_flash)
