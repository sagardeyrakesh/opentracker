
void storage_config_fill() {
  //fill settings storage space with zeros on very first run
  debug_print(F("storage_config_fill() started"));

  for(int i = STORAGE_CONFIG_PAGE;i<STORAGE_DATA_START;i++) {
    debug_print(F("storage_config_fill(): filling address with 0"));
    debug_print(i);
    dueFlashStorage.write(i,0);
  }

  //set first run flag
  dueFlashStorage.write(STORAGE_FIRST_RUN_PAGE,1);

  debug_print(F("storage_config_fill() finished"));
}

void settings_load() {
  //load all settings from EEPROM
  byte tmp;

  debug_print(F("settings_load() started"));

  byte first_run = dueFlashStorage.read(STORAGE_FIRST_RUN_PAGE);
  debug_print(F("settings_load(): First run flag:"));
  debug_print(first_run);

  if(first_run != 1) {
    //first run was not set, this is first even run of the board use config from tracker.h
    debug_print(F("settings_load(): setting defaults from config"));
    config.interval = INTERVAL;
    config.interval_send = INTERVAL_SEND;
    config.powersave = POWERSAVE;
    config.alarm_on =  DEFAULT_ALARM_ON;

    strlcpy(config.key, KEY, sizeof(config.key));
    strlcpy(config.sms_key, SMS_KEY, sizeof(config.sms_key));
    strlcpy(config.sim_pin, SIM_PIN, sizeof(config.sim_pin));
    strlcpy(config.apn, DEFAULT_APN, sizeof(config.apn));
    strlcpy(config.user, DEFAULT_USER, sizeof(config.user));
    strlcpy(config.pwd, DEFAULT_PASS, sizeof(config.pwd));
    strlcpy(config.alarm_phone, DEFAULT_ALARM_SMS, sizeof(config.alarm_phone));

    debug_print(F("settings_load(): set config.interval:"));
    debug_print(config.interval);
    debug_print(config.apn);

    addon_event(ON_SETTINGS_DEFAULT);

    dueFlashStorage.write(STORAGE_FIRST_RUN_PAGE,1);  //set first run flag
    settings_save(); //save settings
  } else {
    debug_print(F("settings_load(): no a first run, loading settings."));

    byte* b = dueFlashStorage.readAddress(STORAGE_CONFIG_PAGE); // byte array which is read from flash at adress
    memcpy(&config, b, sizeof(settings)); // copy byte array to temporary struct

    addon_event(ON_SETTINGS_LOAD);
  }

  //setting defaults in case nothing loaded
  debug_print(F("settings_load(): config.interval:"));
  debug_print(config.interval);

  if((config.interval == -1) || (config.interval < 0) || (config.interval > 5184000)) {
    debug_print(F("settings_load(): interval not found, setting default"));
    config.interval = INTERVAL;

    debug_print(F("settings_load(): set config.interval:"));
    debug_print(config.interval);
  }

  //interval send
  debug_print(F("settings_load(): config.interval_send:"));
  debug_print(config.interval_send);

  if((config.interval_send == -1) || (config.interval_send < 0) || (config.interval_send > 100)) {
    debug_print(F("settings_load(): interval_send not found, setting default"));
    config.interval_send = INTERVAL_SEND;

    debug_print(F("settings_load(): set config.interval_send:"));
    debug_print(config.interval_send);
  }

  //powersave
  debug_print(F("settings_load(): config.powersave:"));
  debug_print(config.powersave);

  if((config.powersave != 1) && (config.powersave != 0)) {
    debug_print(F("settings_load(): powersave not found, setting default"));
    config.powersave = POWERSAVE;

    debug_print(F("settings_load(): set config.powersave:"));
    debug_print(config.powersave);
  }

  tmp = config.key[0];
  if(tmp == 255) { //this check is not sufficient
    debug_print(F("settings_load(): key not found, setting default"));
    strlcpy(config.key, KEY, sizeof(config.key));
  }

  tmp = config.sms_key[0];
  if(tmp == 255) { //this check is not sufficient
    debug_print("settings_load(): SMS key not found, setting default");
    strlcpy(config.sms_key, SMS_KEY, sizeof(config.sms_key));
  }

  tmp = config.sim_pin[0];
  if(tmp == 255) { //this check is not sufficient
    debug_print("settings_load(): SIM pin not found, setting default");
    strlcpy(config.sim_pin, SIM_PIN, sizeof(config.sim_pin));
  }

  tmp = config.apn[0];
  if(tmp == 255) {
    debug_print("settings_load(): APN not set, setting default");
    strlcpy(config.apn, DEFAULT_APN, sizeof(config.apn));
  }

  tmp = config.user[0];
  if(tmp == 255) {
    debug_print("settings_load(): APN user not set, setting default");
    strlcpy(config.user, DEFAULT_USER, sizeof(config.user));
  }

  tmp = config.pwd[0];
  if(tmp == 255) {
    debug_print("settings_load(): APN password not set, setting default");
    strlcpy(config.pwd, DEFAULT_PASS, sizeof(config.pwd));
  }

  tmp = config.alarm_phone[0];
  if(tmp == 255) {
    debug_print("settings_load(): Alarm SMS number not set, setting default");
    strlcpy(config.alarm_phone, DEFAULT_ALARM_SMS, sizeof(config.alarm_phone));
  }
  
  debug_print(F("settings_load() finished"));
}

void settings_save() {
  debug_print(F("settings_save() started"));

  //save all settings to flash
  byte b2[sizeof(settings)]; // create byte array to store the struct
  memcpy(b2, &config, sizeof(settings)); // copy the struct to the byte array
  dueFlashStorage.write(STORAGE_CONFIG_PAGE, b2, sizeof(settings)); // write byte array to flash

  addon_event(ON_SETTINGS_SAVE);
  
  debug_print(F("settings_save() finished"));
}
