///updated
/**
 * @file       TinyGsmClientSIM7600.h
 * @author     Volodymyr Shymanskyy
 * @license    LGPL-3.0
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Nov 2016
 */

#ifndef SRC_TINYGSMCLIENTSIM7600_H_
#define SRC_TINYGSMCLIENTSIM7600_H_

// #define TINY_GSM_DEBUG Serial
// #define TINY_GSM_USE_HEX

#define TINY_GSM_MUX_COUNT 10
#define TINY_GSM_BUFFER_READ_AND_CHECK_SIZE

#include "TinyGsmBattery.tpp"
#include "TinyGsmGPRS.tpp"
#include "TinyGsmGPS.tpp"
#include "TinyGsmGSMLocation.tpp"
#include "TinyGsmModem.tpp"
#include "TinyGsmSMS.tpp"
#include "TinyGsmTCP.tpp"
#include "TinyGsmTemperature.tpp"
#include "TinyGsmTime.tpp"
#include "TinyGsmNTP.tpp"


#define GSM_NL "\r\n"
static const char GSM_OK[] TINY_GSM_PROGMEM    = "OK" GSM_NL;
static const char GSM_ERROR[] TINY_GSM_PROGMEM = "ERROR" GSM_NL;
#if defined       TINY_GSM_DEBUG
static const char GSM_CME_ERROR[] TINY_GSM_PROGMEM = GSM_NL "+CME ERROR:";
static const char GSM_CMS_ERROR[] TINY_GSM_PROGMEM = GSM_NL "+CMS ERROR:";
#endif

enum RegStatus {
  REG_NO_RESULT    = -1,
  REG_UNREGISTERED = 0,
  REG_SEARCHING    = 2,
  REG_DENIED       = 3,
  REG_OK_HOME      = 1,
  REG_OK_ROAMING   = 5,
  REG_UNKNOWN      = 4,
};

class TinyGsmSim7600 : public TinyGsmModem<TinyGsmSim7600>,
                       public TinyGsmGPRS<TinyGsmSim7600>,
                       public TinyGsmTCP<TinyGsmSim7600, TINY_GSM_MUX_COUNT>,
                       public TinyGsmSMS<TinyGsmSim7600>,
                       public TinyGsmGSMLocation<TinyGsmSim7600>,
                       public TinyGsmGPS<TinyGsmSim7600>,
                       public TinyGsmTime<TinyGsmSim7600>,
                       public TinyGsmNTP<TinyGsmSim7600>,
                       public TinyGsmBattery<TinyGsmSim7600>,
                       public TinyGsmTemperature<TinyGsmSim7600> {
  friend class TinyGsmModem<TinyGsmSim7600>;
  friend class TinyGsmGPRS<TinyGsmSim7600>;
  friend class TinyGsmTCP<TinyGsmSim7600, TINY_GSM_MUX_COUNT>;
  friend class TinyGsmSMS<TinyGsmSim7600>;
  friend class TinyGsmGPS<TinyGsmSim7600>;
  friend class TinyGsmGSMLocation<TinyGsmSim7600>;
  friend class TinyGsmTime<TinyGsmSim7600>;
  friend class TinyGsmNTP<TinyGsmSim7600>;
  friend class TinyGsmBattery<TinyGsmSim7600>;
  friend class TinyGsmTemperature<TinyGsmSim7600>;

  /*
   * Inner Client
   */
 public:
  class GsmClientSim7600 : public GsmClient {
    friend class TinyGsmSim7600;

   public:
    GsmClientSim7600() {}

    explicit GsmClientSim7600(TinyGsmSim7600& modem, uint8_t mux = 0) {
      init(&modem, mux);
    }

    bool init(TinyGsmSim7600* modem, uint8_t mux = 0) {
      this->at       = modem;
      sock_available = 0;
      prev_check     = 0;
      sock_connected = false;
      got_data       = false;

      if (mux < TINY_GSM_MUX_COUNT) {
        this->mux = mux;
      } else {
        this->mux = (mux % TINY_GSM_MUX_COUNT);
      }
      at->sockets[this->mux] = this;

      return true;
    }

   public:
    virtual int connect(const char* host, uint16_t port, int timeout_s) {
      stop();
      TINY_GSM_YIELD();
      rx.clear();
      sock_connected = at->modemConnect(host, port, mux, false, timeout_s);
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES

    void stop(uint32_t maxWaitMs) {
      dumpModemBuffer(maxWaitMs);
      at->sendAT(GF("+CIPCLOSE="), mux);
      sock_connected = false;
      at->waitResponse();
    }
    void stop() override {
      stop(15000L);
    }

    /*
     * Extended API
     */

    String remoteIP() TINY_GSM_ATTR_NOT_IMPLEMENTED;
  };

  /*
   * Inner Secure Client
   */

  /*TODO(?))
  class GsmClientSecureSIM7600 : public GsmClientSim7600
  {
  public:
    GsmClientSecure() {}

    GsmClientSecure(TinyGsmSim7600& modem, uint8_t mux = 0)
     : public GsmClient(modem, mux)
    {}

  public:
    int connect(const char* host, uint16_t port, int timeout_s) override {
      stop();
      TINY_GSM_YIELD();
      rx.clear();
      sock_connected = at->modemConnect(host, port, mux, true, timeout_s);
      return sock_connected;
    }
    TINY_GSM_CLIENT_CONNECT_OVERRIDES
  };
  */

  /*
   * Constructor
   */
 public:
  explicit TinyGsmSim7600(Stream& stream) : stream(stream) {
    memset(sockets, 0, sizeof(sockets));
  }

  /*
   * Basic functions
   */
 protected:
  bool initImpl(const char* pin = NULL) {
    DBG(GF("### TinyGSM Version:"), TINYGSM_VERSION);
    DBG(GF("### TinyGSM Compiled Module:  TinyGsmClientSIM7600"));

    if (!testAT()) { return false; }

    sendAT(GF("E0"));  // Echo Off
    if (waitResponse() != 1) { return false; }

#ifdef TINY_GSM_DEBUG
    sendAT(GF("+CMEE=2"));  // turn on verbose error codes
#else
    sendAT(GF("+CMEE=0"));  // turn off error codes
#endif
    waitResponse();

    DBG(GF("### Modem:"), getModemName());

    // Disable time and time zone URC's
    sendAT(GF("+CTZR=0"));
    if (waitResponse(10000L) != 1) { return false; }

    // Enable automatic time zome update
    sendAT(GF("+CTZU=1"));
    if (waitResponse(10000L) != 1) { return false; }

    SimStatus ret = getSimStatus();
    // if the sim isn't ready and a pin has been provided, try to unlock the sim
    if (ret != SIM_READY && pin != NULL && strlen(pin) > 0) {
      simUnlock(pin);
      return (getSimStatus() == SIM_READY);
    } else {
      // if the sim is ready, or it's locked but no pin has been provided,
      // return true
      return (ret == SIM_READY || ret == SIM_LOCKED);
    }
  }

  String getModemNameImpl() {
    String name = "SIMCom SIM7600";

    sendAT(GF("+CGMM"));
    String res2;
    if (waitResponse(1000L, res2) != 1) { return name; }
    res2.replace(GSM_NL "OK" GSM_NL, "");
    res2.replace("_", " ");
    res2.trim();

    name = res2;
    DBG("### Modem:", name);
    return name;
  }

  bool factoryDefaultImpl() {  // these commands aren't supported
    return false;
  }

  /*
   * Power functions
   */
 protected:
  bool restartImpl(const char* pin = NULL) {
    if (!testAT()) { return false; }
    sendAT(GF("+CRESET"));
    if (waitResponse(10000L) != 1) { return false; }
    delay(5000L);  // TODO(?):  Test this delay!
    return init(pin);
  }

  bool powerOffImpl() {
    sendAT(GF("+CPOF"));
    return waitResponse() == 1;
  }

  bool radioOffImpl() {
    if (!setPhoneFunctionality(4)) { return false; }
    delay(3000);
    return true;
  }

  bool sleepEnableImpl(bool enable = true) {
    sendAT(GF("+CSCLK="), enable);
    return waitResponse() == 1;
  }

  bool setPhoneFunctionalityImpl(uint8_t fun, bool reset = false) {
    sendAT(GF("+CFUN="), fun, reset ? ",1" : "");
    return waitResponse(10000L) == 1;
  }

  /*
   * Generic network functions
   */
 public:
  RegStatus getRegistrationStatus() {
    return (RegStatus)getRegistrationStatusXREG("CGREG");
  }

 protected:
  bool isNetworkConnectedImpl() {
    RegStatus s = getRegistrationStatus();
    return (s == REG_OK_HOME || s == REG_OK_ROAMING);
  }

 public:
  String getNetworkModes() {
    sendAT(GF("+CNMP=?"));
    if (waitResponse(GF(GSM_NL "+CNMP:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    return res;
  }

  int16_t getNetworkMode() {
    sendAT(GF("+CNMP?"));
    if (waitResponse(GF(GSM_NL "+CNMP:")) != 1) { return false; }
    int16_t mode = streamGetIntBefore('\n');
    waitResponse();
    return mode;
  }

  bool setNetworkMode(uint8_t mode) {
    sendAT(GF("+CNMP="), mode);
    return waitResponse() == 1;
  }

  String getLocalIPImpl() {
    sendAT(GF("+IPADDR"));  // Inquire Socket PDP address
    // sendAT(GF("+CGPADDR=1"));  // Show PDP address
    String res;
    if (waitResponse(10000L, res) != 1) { return ""; }
    res.replace(GSM_NL "OK" GSM_NL, "");
    res.replace(GSM_NL, "");
    res.trim();
    return res;
  }

  /*
   * GPRS functions
   */
 protected:
  bool gprsConnectImpl(const char* apn, const char* user = NULL,
                       const char* pwd = NULL) {
    gprsDisconnect();  // Make sure we're not connected first

    // Define the PDP context

    // The CGDCONT commands set up the "external" PDP context

    // Set the external authentication
    if (user && strlen(user) > 0) {
      sendAT(GF("+CGAUTH=1,0,\""), user, GF("\",\""), pwd, '"');
      waitResponse();
    }

    // Define external PDP context 1
    sendAT(GF("+CGDCONT=1,\"IP\",\""), apn, '"', ",\"0.0.0.0\",0,0");
    waitResponse();

    // Configure TCP parameters

    // Select TCP/IP application mode (command mode)
    sendAT(GF("+CIPMODE=0"));
    waitResponse();

    // Set Sending Mode - send without waiting for peer TCP ACK
    sendAT(GF("+CIPSENDMODE=0"));
    waitResponse();

    // Configure socket parameters
    // AT+CIPCCFG= <NmRetry>, <DelayTm>, <Ack>, <errMode>, <HeaderType>,
    //            <AsyncMode>, <TimeoutVal>
    // NmRetry = number of retransmission to be made for an IP packet
    //         = 10 (default)
    // DelayTm = number of milliseconds to delay before outputting received data
    //          = 0 (default)
    // Ack = sets whether reporting a string "Send ok" = 0 (don't report)
    // errMode = mode of reporting error result code = 0 (numberic values)
    // HeaderType = which data header of receiving data in multi-client mode
    //            = 1 (+RECEIVE,<link num>,<data length>)
    // AsyncMode = sets mode of executing commands
    //           = 0 (synchronous command executing)
    // TimeoutVal = minimum retransmission timeout in milliseconds = 75000
    sendAT(GF("+CIPCCFG=10,0,0,0,1,0,75000"));
    if (waitResponse() != 1) { return false; }

    // Configure timeouts for opening and closing sockets
    // AT+CIPTIMEOUT=<netopen_timeout> <cipopen_timeout>, <cipsend_timeout>
    sendAT(GF("+CIPTIMEOUT="), 75000, ',', 15000, ',', 15000);
    waitResponse();

    // Start the socket service

    // This activates and attaches to the external PDP context that is tied
    // to the embedded context for TCP/IP (ie AT+CGACT=1,1 and AT+CGATT=1)
    // Response may be an immediate "OK" followed later by "+NETOPEN: 0".
    // We to ignore any immediate response and wait for the
    // URC to show it's really connected.
    sendAT(GF("+NETOPEN"));
    if (waitResponse(75000L, GF(GSM_NL "+NETOPEN: 0")) != 1) { return false; }

    return true;
  }

  bool gprsDisconnectImpl() {
    // Close all sockets and stop the socket service
    // Note: On the LTE models, this single command closes all sockets and the
    // service
    sendAT(GF("+NETCLOSE"));
    if (waitResponse(60000L, GF(GSM_NL "+NETCLOSE: 0")) != 1) { return false; }

    return true;
  }

  bool isGprsConnectedImpl() {
    sendAT(GF("+NETOPEN?"));
    // May return +NETOPEN: 1, 0.  We just confirm that the first number is 1
    if (waitResponse(GF(GSM_NL "+NETOPEN: 1")) != 1) { return false; }
    waitResponse();

    sendAT(GF("+IPADDR"));  // Inquire Socket PDP address
    // sendAT(GF("+CGPADDR=1")); // Show PDP address
    if (waitResponse() != 1) { return false; }

    return true;
  }
/////////-add sms and call function-/////////////////
  /*
   * Phone Call functions
   */
 public:
  bool setGsmBusy(bool busy = true) {
    sendAT(GF("+GSMBUSY="), busy ? 1 : 0);
    return waitResponse() == 1;
  }

  bool callAnswer() {
    sendAT(GF("A"));
    return waitResponse() == 1;
  }

  // Returns true on pick-up, false on error/busy
  bool callNumber(const String& number) {
    if (number == GF("last")) {
      sendAT(GF("DL"));
    } else {
      sendAT(GF("D"), number, ";");
    }
    int status = waitResponse(60000L,
                              GFP(GSM_OK),
                              GF("BUSY" GSM_NL),
                              GF("NO ANSWER" GSM_NL),
                              GF("NO CARRIER" GSM_NL));
    switch (status) {
    case 1:  return true;
    case 2:
    case 3:  return false;
    default: return false;
    }
  }

  bool callHangup() {
    sendAT(GF("H"));
    return waitResponse() == 1;
  }

  // 0-9,*,#,A,B,C,D
  bool dtmfSend(char cmd, int duration_ms = 100) {
    duration_ms = constrain(duration_ms, 100, 1000);

    sendAT(GF("+VTD="), duration_ms / 100); // VTD accepts in 1/10 of a second
    waitResponse();

    sendAT(GF("+VTS="), cmd);
    return waitResponse(10000L) == 1;
  }

  /*
   * Messaging functions
   */

  String sendUSSD(const String& code) {
    sendAT(GF("+CMGF=1"));
    waitResponse();
    sendAT(GF("+CSCS=\"HEX\""));
    waitResponse();
    sendAT(GF("+CUSD=1,\""), code, GF("\""));
    if (waitResponse() != 1) {
      return "";
    }
    if (waitResponse(10000L, GF(GSM_NL "+CUSD:")) != 1) {
      return "";
    }
    stream.readStringUntil('"');
    String hex = stream.readStringUntil('"');
    stream.readStringUntil(',');
    int dcs = stream.readStringUntil('\n').toInt();

    if (dcs == 15) {
      return TinyGsmDecodeHex8bit(hex);
    } else if (dcs == 72) {
      return TinyGsmDecodeHex16bit(hex);
    } else {
      return hex;
    }
  }
  bool sendSMS(const String& number, const String& text) {
    sendAT(GF("+CMGF=1"));
    waitResponse();
    //Set GSM 7 bit default alphabet (3GPP TS 23.038)
    sendAT(GF("+CSCS=\"GSM\""));
    waitResponse();
    sendAT(GF("+CMGS=\""), number, GF("\""));
    if (waitResponse(GF(">")) != 1) {
      return false;
    }
    stream.print(text);
    stream.write((char)0x1A);
    stream.flush();
    return waitResponse(60000L) == 1;
  }
  int newMessageInterrupt(String interrupt){
    int Start=interrupt.indexOf(',');
    int Stop=interrupt.indexOf('\n',Start);
    int index=interrupt.substring(Start+1,Stop-1).toInt();
    return index;
  }
  String readSMS(int index, const bool changeStatusToRead = true){
    //sendAT(GF("+CMGF=1"));waitResponse();  
///	Serial.println("AT+CMGR="+String(index));
//    sendAT(GF("+CMGR="), index, GF(","), static_cast<const uint8_t>(!changeStatusToRead)); 
    sendAT(GF("+CMGR="),index); 
    String h="";
    streamSkipUntil('\n');
    streamSkipUntil('\n');
    h=stream.readStringUntil('\n');
	Serial.println(h);
    return h;
  }
int newMessageIndex(bool mode) {
    // Set SMS format to text mode
    sendAT(GF("+CMGF=1"));
    // Read unread messages
    sendAT(GF("+CMGL=\"REC UNREAD\""));
    String h = stream.readString();
    Serial.println(h);
    int i;
    if (mode) {
        i = h.indexOf("+CMGL: ");
    } else {
        i = h.lastIndexOf("+CMGL: ");
    }
    
    int index = h.substring(i + 7, i + 9).toInt();
    if (index <= 0) return 0;
    return index;
}
  bool emptySMSBuffer(){
    sendAT(GF("+CMGF=1"));
    waitResponse(); 
    sendAT(GF("+CMGDA=\"DEL ALL\""));
    return waitResponse(60000L) == 1;
  }
  String getSenderID(int index, const bool changeStatusToRead = true){
    //sendAT(GF("+CMGF=1"));waitResponse(); 
    //sendAT(GF("+CMGR="), index, GF(","), static_cast<const uint8_t>(changeStatusToRead)); 
    sendAT(GF("+CMGR="),index); 

    String h="";
    streamSkipUntil('"');    
	streamSkipUntil('"');
    streamSkipUntil('"');/////////////////////////streamSkipUntil('"');
    h=stream.readStringUntil('"');
    stream.readString();
    return h;
  }
  bool sendSMS_UTF16(const String& number, const void* text, size_t len) {
    sendAT(GF("+CMGF=1"));
    waitResponse();
    sendAT(GF("+CSCS=\"HEX\""));
    waitResponse();
    sendAT(GF("+CSMP=17,167,0,8"));
    waitResponse();

    sendAT(GF("+CMGS=\""), number, GF("\""));
    if (waitResponse(GF(">")) != 1) {
      return false;
    }

    uint16_t* t = (uint16_t*)text;
    for (size_t i=0; i<len; i++) {
      uint8_t c = t[i] >> 8;
      if (c < 0x10) { stream.print('0'); }
      stream.print(c, HEX);
      c = t[i] & 0xFF;
      if (c < 0x10) { stream.print('0'); }
      stream.print(c, HEX);
    }
    stream.write((char)0x1A);
    stream.flush();
    return waitResponse(60000L) == 1;
  }



/////////////
  /*
   * SIM card functions
   */
 protected:
  // Gets the CCID of a sim card via AT+CCID
  String getSimCCIDImpl() {
    sendAT(GF("+CICCID"));
    if (waitResponse(GF(GSM_NL "+ICCID:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  /*
   * Phone Call functions
   */
 protected:
  bool callAnswerImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool callNumberImpl(const String& number) TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool callHangupImpl() TINY_GSM_ATTR_NOT_IMPLEMENTED;
  bool dtmfSendImpl(char cmd,
                    int  duration_ms = 100) TINY_GSM_ATTR_NOT_IMPLEMENTED;

  /*
   * Messaging functions
   */
 protected:
  // Follows all messaging functions per template

  /*
   * GSM Location functions
   */
 protected:
  // Can return a GSM-based location from CLBS as per the template

  /*
   * GPS/GNSS/GLONASS location functions
   */
 protected:
  // enable GPS
  bool enableGPSImpl() {
    sendAT(GF("+CGPS=1,1"));
    if (waitResponse() != 1) { return false; }
    return true;
  }

  bool disableGPSImpl() {
    sendAT(GF("+CGPS=0"));
    if (waitResponse() != 1) { return false; }
    return true;
  }

  // get the RAW GPS output
  String getGPSrawImpl() {
    sendAT(GF("+CGNSSINFO"));
    if (waitResponse(GF(GSM_NL "+CGNSSINFO:")) != 1) { return ""; }
    String res = stream.readStringUntil('\n');
    waitResponse();
    res.trim();
    return res;
  }

  // get GPS informations (MODIFIED BY SIMKARD TO GET RESULTS INSTANTLY WHILE QUERYING)
  bool getGPSImpl(int* fixmode, int* gps_vs, int* glonass_vs, int* beidou_vs, float* lat, float* lon, float* speed = 0, float* alt = 0,
                  int* vsat = 0, int* usat = 0, float* accuracy = 0,
                  int* year = 0, int* month = 0, int* day = 0, int* hour = 0,
                  int* minute = 0, int* second = 0) {
    // init variables
	String GPS_str_GNSSINFO;
	int GPS_str_length			= 0;
	int GPS_str_index_start		= 0;
	int GPS_str_index_end		= 0;
    float ilat					= 0;
    char  north;
    float ilon					= 0;
    char  east;
    int   iyear					= 0;
    float isecondWithSS			= 0;
	
	GPS_str_GNSSINFO.concat(getGPSrawImpl());
	// If data available :	"2,05,03,00,4546.722312,N,00452.604912,E,271021,114122.0,169.0,0.0,,1.9,1.7,0.9"
	// Or :					"2,05,04,00,4546.721098,N,00452.595570,E,281021,101848.0,183.1,0.0,,1.6,1.3,0.9"
	// If not :				",,,,,,,,,,,,,,," at best or ""
	
	if (GPS_str_GNSSINFO.length() > 20){
		for (int i = 0 ; i < 30 ; i++){
			if (i == 0){
				GPS_str_length = GPS_str_GNSSINFO.length();
				GPS_str_index_end = GPS_str_GNSSINFO.indexOf(',');
				*fixmode = (GPS_str_GNSSINFO.substring(GPS_str_index_start, GPS_str_index_end)).toInt();
			} else {
				GPS_str_index_start = GPS_str_index_end + 1;
				GPS_str_index_end = GPS_str_GNSSINFO.indexOf(',', GPS_str_index_start);
					   if (i == 1){		*gps_vs = (GPS_str_GNSSINFO.substring(GPS_str_index_start, GPS_str_index_end)).toInt();									// Valid GPS sats
				} else if (i == 2){		*glonass_vs = (GPS_str_GNSSINFO.substring(GPS_str_index_start, GPS_str_index_end)).toInt();								// Valid GLONASS sats
				} else if (i == 3){		*beidou_vs = (GPS_str_GNSSINFO.substring(GPS_str_index_start, GPS_str_index_end)).toInt();								// Valid BEIDOU sats
				} else if (i == 4){		ilat = (GPS_str_GNSSINFO.substring(GPS_str_index_start, GPS_str_index_end)).toFloat();									// Latitude 1/2
				} else if (i == 5){		*lat = (floor(ilat / 100) + fmod(ilat, 100.) / 60) * (GPS_str_GNSSINFO.charAt(GPS_str_index_start) == 'N' ? 1 : -1);	// Latitude 2/2
				} else if (i == 6){		ilon = (GPS_str_GNSSINFO.substring(GPS_str_index_start, GPS_str_index_end)).toFloat();									// Longitude 1/2
				} else if (i == 7){		*lon = (floor(ilon / 100) + fmod(ilon, 100.) / 60) * (GPS_str_GNSSINFO.charAt(GPS_str_index_start) == 'E' ? 1 : -1);	// Longitude 2/2
				} else if (i == 8){																																// Date
					*day = (GPS_str_GNSSINFO.substring(GPS_str_index_start, GPS_str_index_start + 2)).toInt();
					*month = (GPS_str_GNSSINFO.substring(GPS_str_index_start + 2, GPS_str_index_start + 4)).toInt();
					iyear = (GPS_str_GNSSINFO.substring(GPS_str_index_start + 4, (GPS_str_index_end))).toInt();
					if (iyear < 2000) {	*year = iyear += 2000;
					} else {			*year = iyear;
					}
				} else if (i == 9){																																// Time
					*hour = (GPS_str_GNSSINFO.substring(GPS_str_index_start, GPS_str_index_start + 2)).toInt();
					*minute = (GPS_str_GNSSINFO.substring(GPS_str_index_start + 2, GPS_str_index_start + 4)).toInt();
					isecondWithSS = (GPS_str_GNSSINFO.substring(GPS_str_index_start + 4, (GPS_str_index_end))).toFloat();
					*second = static_cast<int>(isecondWithSS);
				} else if (i == 10){	*alt = (GPS_str_GNSSINFO.substring(GPS_str_index_start, GPS_str_index_end)).toFloat();									// Altitude
				} else if (i == 11){	*speed = (GPS_str_GNSSINFO.substring(GPS_str_index_start, GPS_str_index_end)).toFloat();								// Speed
				} else if (i == 13){	*accuracy = (GPS_str_GNSSINFO.substring(GPS_str_index_start, GPS_str_index_end)).toFloat();								// PDOP
				}
				if (GPS_str_index_end == -1){
					return true;
				}
			}
		}
	}
    return false;
  }


  /**
   *  CGNSSMODE: <gnss_mode>,<dpo_mode>
   *  This command is used to configure GPS, GLONASS, BEIDOU and QZSS support
   * mode. 0 : GLONASS 1 : BEIDOU 2 : GALILEO 3 : QZSS dpo_mode: 1 enable , 0
   * disable
   */
  String setGNSSModeImpl(uint8_t mode, bool dpo) {
    String res;
    sendAT(GF("+CGNSSMODE="), mode, ",", dpo);
    if (waitResponse(10000L, res) != 1) { return ""; }
    res.replace(GSM_NL, "");
    res.trim();
    return res;
  }

  uint8_t getGNSSModeImpl() {
    sendAT(GF("+CGNSSMODE?"));
    if (waitResponse(GF(GSM_NL "+CGNSSMODE:")) != 1) { return 0; }
    return stream.readStringUntil(',').toInt();
  }


  /*
   * Time functions
   */
 protected:
  // Can follow the standard CCLK function in the template

  /*
   * NTP server functions
   */
  // Can sync with server using CNTP as per template

  /*
   * Battery functions
   */
 protected:
  // returns volts, multiply by 1000 to get mV
  uint16_t getBattVoltageImpl() {
    sendAT(GF("+CBC"));
    if (waitResponse(GF(GSM_NL "+CBC:")) != 1) { return 0; }

    // get voltage in VOLTS
    float voltage = streamGetFloatBefore('\n');
    // Wait for final OK
    waitResponse();
    // Return millivolts
    uint16_t res = voltage * 1000;
    return res;
  }

  int8_t getBattPercentImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  uint8_t getBattChargeStateImpl() TINY_GSM_ATTR_NOT_AVAILABLE;

  bool getBattStatsImpl(uint8_t& chargeState, int8_t& percent,
                        uint16_t& milliVolts) {
    chargeState = 0;
    percent     = 0;
    milliVolts  = getBattVoltage();
    return true;
  }

  /*
   * Temperature functions
   */
 protected:
  // get temperature in degree celsius
  uint16_t getTemperatureImpl() {
    sendAT(GF("+CPMUTEMP"));
    if (waitResponse(GF(GSM_NL "+CPMUTEMP:")) != 1) { return 0; }
    // return temperature in C
    uint16_t res = streamGetIntBefore('\n');
    // Wait for final OK
    waitResponse();
    return res;
  }

  /*
   * Client related functions
   */
 protected:
  bool modemConnect(const char* host, uint16_t port, uint8_t mux,
                    bool ssl = false, int timeout_s = 15) {
    if (ssl) { DBG("SSL not yet supported on this module!"); }
    // Make sure we'll be getting data manually on this connection
    sendAT(GF("+CIPRXGET=1"));
    if (waitResponse() != 1) { return false; }

    // Establish a connection in multi-socket mode
    uint32_t timeout_ms = ((uint32_t)timeout_s) * 1000;
    sendAT(GF("+CIPOPEN="), mux, ',', GF("\"TCP"), GF("\",\""), host, GF("\","),
           port);
    // The reply is OK followed by +CIPOPEN: <link_num>,<err> where <link_num>
    // is the mux number and <err> should be 0 if there's no error
    if (waitResponse(timeout_ms, GF(GSM_NL "+CIPOPEN:")) != 1) { return false; }
    uint8_t opened_mux    = streamGetIntBefore(',');
    uint8_t opened_result = streamGetIntBefore('\n');
    if (opened_mux != mux || opened_result != 0) return false;
    return true;
  }

  int16_t modemSend(const void* buff, size_t len, uint8_t mux) {
    sendAT(GF("+CIPSEND="), mux, ',', (uint16_t)len);
    if (waitResponse(GF(">")) != 1) { return 0; }
    stream.write(reinterpret_cast<const uint8_t*>(buff), len);
    stream.flush();
    if (waitResponse(GF(GSM_NL "+CIPSEND:")) != 1) { return 0; }
    streamSkipUntil(',');  // Skip mux
    streamSkipUntil(',');  // Skip requested bytes to send
    // TODO(?):  make sure requested and confirmed bytes match
    return streamGetIntBefore('\n');
  }

  size_t modemRead(size_t size, uint8_t mux) {
    if (!sockets[mux]) return 0;
#ifdef TINY_GSM_USE_HEX
    sendAT(GF("+CIPRXGET=3,"), mux, ',', (uint16_t)size);
    if (waitResponse(GF("+CIPRXGET:")) != 1) { return 0; }
#else
    sendAT(GF("+CIPRXGET=2,"), mux, ',', (uint16_t)size);
    if (waitResponse(GF("+CIPRXGET:")) != 1) { return 0; }
#endif
    streamSkipUntil(',');  // Skip Rx mode 2/normal or 3/HEX
    streamSkipUntil(',');  // Skip mux/cid (connecion id)
    int16_t len_requested = streamGetIntBefore(',');
    //  ^^ Requested number of data bytes (1-1460 bytes)to be read
    int16_t len_confirmed = streamGetIntBefore('\n');
    // ^^ The data length which not read in the buffer
    for (int i = 0; i < len_requested; i++) {
      uint32_t startMillis = millis();
#ifdef TINY_GSM_USE_HEX
      while (stream.available() < 2 &&
             (millis() - startMillis < sockets[mux]->_timeout)) {
        TINY_GSM_YIELD();
      }
      char buf[4] = {
          0,
      };
      buf[0] = stream.read();
      buf[1] = stream.read();
      char c = strtol(buf, NULL, 16);
#else
      while (!stream.available() &&
             (millis() - startMillis < sockets[mux]->_timeout)) {
        TINY_GSM_YIELD();
      }
      char c = stream.read();
#endif
      sockets[mux]->rx.put(c);
    }
    // DBG("### READ:", len_requested, "from", mux);
    // sockets[mux]->sock_available = modemGetAvailable(mux);
    sockets[mux]->sock_available = len_confirmed;
    waitResponse();
    return len_requested;
  }

  size_t modemGetAvailable(uint8_t mux) {
    if (!sockets[mux]) return 0;
    sendAT(GF("+CIPRXGET=4,"), mux);
    size_t result = 0;
    if (waitResponse(GF("+CIPRXGET:")) == 1) {
      streamSkipUntil(',');  // Skip mode 4
      streamSkipUntil(',');  // Skip mux
      result = streamGetIntBefore('\n');
      waitResponse();
    }
    // DBG("### Available:", result, "on", mux);
    if (!result) { sockets[mux]->sock_connected = modemGetConnected(mux); }
    return result;
  }

  bool modemGetConnected(uint8_t mux) {
    // Read the status of all sockets at once
    sendAT(GF("+CIPCLOSE?"));
    if (waitResponse(GF("+CIPCLOSE:")) != 1) {
      // return false;  // TODO:  Why does this not read correctly?
    }
    for (int muxNo = 0; muxNo < TINY_GSM_MUX_COUNT; muxNo++) {
      // +CIPCLOSE:<link0_state>,<link1_state>,...,<link9_state>
      bool muxState = stream.parseInt();
      if (sockets[muxNo]) { sockets[muxNo]->sock_connected = muxState; }
    }
    waitResponse();  // Should be an OK at the end
    if (!sockets[mux]) return false;
    return sockets[mux]->sock_connected;
  }

  /*
   * Utilities
   */
 public:
  // TODO(vshymanskyy): Optimize this!
  int8_t waitResponse(uint32_t timeout_ms, String& data,
                      GsmConstStr r1 = GFP(GSM_OK),
                      GsmConstStr r2 = GFP(GSM_ERROR),
#if defined TINY_GSM_DEBUG
                      GsmConstStr r3 = GFP(GSM_CME_ERROR),
                      GsmConstStr r4 = GFP(GSM_CMS_ERROR),
#else
                      GsmConstStr r3 = NULL, GsmConstStr r4 = NULL,
#endif
                      GsmConstStr r5 = NULL) {
    /*String r1s(r1); r1s.trim();
    String r2s(r2); r2s.trim();
    String r3s(r3); r3s.trim();
    String r4s(r4); r4s.trim();
    String r5s(r5); r5s.trim();
    DBG("### ..:", r1s, ",", r2s, ",", r3s, ",", r4s, ",", r5s);*/
    data.reserve(64);
    uint8_t  index       = 0;
    uint32_t startMillis = millis();
    do {
      TINY_GSM_YIELD();
      while (stream.available() > 0) {
        TINY_GSM_YIELD();
        int8_t a = stream.read();
        if (a <= 0) continue;  // Skip 0x00 bytes, just in case
        data += static_cast<char>(a);
        if (r1 && data.endsWith(r1)) {
          index = 1;
          goto finish;
        } else if (r2 && data.endsWith(r2)) {
          index = 2;
          goto finish;
        } else if (r3 && data.endsWith(r3)) {
#if defined TINY_GSM_DEBUG
          if (r3 == GFP(GSM_CME_ERROR)) {
            streamSkipUntil('\n');  // Read out the error
          }
#endif
          index = 3;
          goto finish;
        } else if (r4 && data.endsWith(r4)) {
          index = 4;
          goto finish;
        } else if (r5 && data.endsWith(r5)) {
          index = 5;
          goto finish;
        } else if (data.endsWith(GF(GSM_NL "+CIPRXGET:"))) {
          int8_t mode = streamGetIntBefore(',');
          if (mode == 1) {
            int8_t mux = streamGetIntBefore('\n');
            if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
              sockets[mux]->got_data = true;
            }
            data = "";
            // DBG("### Got Data:", mux);
          } else {
            data += mode;
          }
        } else if (data.endsWith(GF(GSM_NL "+RECEIVE:"))) {
          int8_t  mux = streamGetIntBefore(',');
          int16_t len = streamGetIntBefore('\n');
          if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
            sockets[mux]->got_data = true;
            if (len >= 0 && len <= 1024) { sockets[mux]->sock_available = len; }
          }
          data = "";
          // DBG("### Got Data:", len, "on", mux);
        } else if (data.endsWith(GF("+IPCLOSE:"))) {
          int8_t mux = streamGetIntBefore(',');
          streamSkipUntil('\n');  // Skip the reason code
          if (mux >= 0 && mux < TINY_GSM_MUX_COUNT && sockets[mux]) {
            sockets[mux]->sock_connected = false;
          }
          data = "";
          DBG("### Closed: ", mux);
        } else if (data.endsWith(GF("+CIPEVENT:"))) {
          // Need to close all open sockets and release the network library.
          // User will then need to reconnect.
          DBG("### Network error!");
          if (!isGprsConnected()) { gprsDisconnect(); }
          data = "";
        }
      }
    } while (millis() - startMillis < timeout_ms);
  finish:
    if (!index) {
      data.trim();
      if (data.length()) { DBG("### Unhandled:", data); }
      data = "";
    }
    // data.replace(GSM_NL, "/");
    // DBG('<', index, '>', data);
    return index;
  }

  int8_t waitResponse(uint32_t timeout_ms, GsmConstStr r1 = GFP(GSM_OK),
                      GsmConstStr r2 = GFP(GSM_ERROR),
#if defined TINY_GSM_DEBUG
                      GsmConstStr r3 = GFP(GSM_CME_ERROR),
                      GsmConstStr r4 = GFP(GSM_CMS_ERROR),
#else
                      GsmConstStr r3 = NULL, GsmConstStr r4 = NULL,
#endif
                      GsmConstStr r5 = NULL) {
    String data;
    return waitResponse(timeout_ms, data, r1, r2, r3, r4, r5);
  }

  int8_t waitResponse(GsmConstStr r1 = GFP(GSM_OK),
                      GsmConstStr r2 = GFP(GSM_ERROR),
#if defined TINY_GSM_DEBUG
                      GsmConstStr r3 = GFP(GSM_CME_ERROR),
                      GsmConstStr r4 = GFP(GSM_CMS_ERROR),
#else
                      GsmConstStr r3 = NULL, GsmConstStr r4 = NULL,
#endif
                      GsmConstStr r5 = NULL) {
    return waitResponse(1000, r1, r2, r3, r4, r5);
  }

 public:
  Stream& stream;

 protected:
  GsmClientSim7600* sockets[TINY_GSM_MUX_COUNT];
  const char*       gsmNL = GSM_NL;
};

#endif  // SRC_TINYGSMCLIENTSIM7600_H_
