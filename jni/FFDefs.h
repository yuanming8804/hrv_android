//  ffdefs.h
//
//  Revisions:
//
//	11-20-03: Original - HES
//



#if !defined ( __FFdefs_h__ )
#define __FFdefs_h__



#define	defRegVersion	6	// default registry key version - with v2.0.6
// ***

#define MIN_SECS_REPEAT_DIALOG		3600	// threshold for dialog at end of session time for optional repeat

// ***

#define defRegAverageSamples           ("AverageSamples")

#define defRegPpgMultiplier            ("PpgMultiplier")
#define defRegIbiMultiplier            ("IbiMultiplier")

#define defRegAgcOn                    ("AgcOn")
#define defRegAgcPowerIfOff            ("AgcPowerLevelIfOff")
#define defRegAgcAmpLimitHigh          ("AgcAmpLimitHigh")
#define defRegAgcAmpLimitLow           ("AgcAmpLimitLow")
#define defRegAgcLastPowerLevel        ("AgcLastPowerLevel")
#define defRegAmpMaxMinValues          ("AgcAmpMaxMinValues")

#define defRegFirThreshold             ("FirThreshold")
#define defRegFirHysteresis            ("FirHysteresis")

// Values used by sensor, previously in Datacollector.h, but also used to init the registry

// In the current use, these define strings are used in the registry use getprofile functions, and provide default values,
// if the REGISTRY is empty. This is likely compiler dependent.

#define defNumAverageSensorSamples      (      8 )

#define defPpgMultiplier                (   8.0f )
#define defPpgMultiplierStr             (  "8.0" )

#define defIbiMultiplier                (   4.0f )
#define defIbiMultiplierStr             (  "4.0" )

#define defFirThreshold                 (     51 )
#define defFirHysteresis                (      4 )

#define defFirNumFilters                (     10 )

/**
** Ibi Interval limits
**
** Under normal conditions, the IBI should not be out of this
** range:
**
**       defIbiIntervalMin < IBI < defIbiIntervalMax
**
** defIbiIntervalMin is equivalent to a heart rate of 160.
** defIbiIntervalMax is equivalent to a heart rate of  30.
**
*/

//#define defIbiIntervalMin               (  98 ) //* original from Fred - really 612 BPM
//#define defIbiIntervalMax               ( 520 ) //* 30

//** wide gate
#define defIbiIntervalMin              (   80 ) //* 300 ==> 200 BPM does not work!
#define defIbiIntervalMax              ( 3000 ) //*  20 BPM

//	#define NO_USB_IBI_LIMIT	// sets the USB driver IBI limiting

// Report sizes

// Sizes of the buffers for the input and output reports
// for communicating with USB Human Interface Devices (HIDs).

#define defUsbHidReportSizeInput     ( 10 + 6 )
#define defUsbHidReportSizeOutput    (  4 + 12 )

// Defines for Automatic Gain Control (AGC)
// AGC can be on or off (controlled by the registry).
#define  defAgcOn                (   1 )
#define  defAgcOff               (   0 )



// AGC has 3 power levels.
// If AGC is off, then the power level specified in the
// registry will be the level set.
#define  defAgcPowerLevelLow     (   0 )
#define  defAgcPowerLevelMed     (   1 )
#define  defAgcPowerLevelHigh    (   2 )



// The AGC code will lower the power if the amplitude exceeds
// the high limit, and raise the power if the amplitude exceeds
// the low limit.
#define  defAgcAmpLimitHigh      ( 700 ) // 2bc hex
#define  defAgcAmpLimitLow       ( 200 ) // from 440d/190 hex - Tom Poliquin - 2-4-04

#define	USB_VENDOR_ID			0x10C4	// ODI USB Sensor
#define	USB_PRODUCT_ID			0x0001

// Modify Serial Port Behavior
#define COM_LOW				("Com Low")	// sets beginning port to search
#define COM_HIGH 			("Com High")  // sets end port to search, currently limited to 9!!!

//Defines instead of strings for various applications - not valid in countries
#define OLD_DB_NAME					("C:\\Program Files\\Planetary\\Freeze-Framer\\FF.MDB")	// Legacy for DB update

//** Change for localization
#define	DEFAULT_DB_NAME				("Data\\ff2.mdb")

#define FF10_DB_NAME				("FF.MDB")   // not used
#define FF20_DB_NAME				("FF2.MDB")

#define CFG_FILENAME				("FF.CFG")

#define DEMO_WAV_FILENAME			("DEMOWAVE.DAT")
#define EXPORT_FILENAME				("EXPORT.DAT")

#define FILTERCHALLPATH				("\\*.par")
#define LOW_CHALL_FILENAME			("\\Low.par")
#define NORMAL_CHALL_FILENAME		("\\Normal.par")
#define HIGH_CHALL_FILENAME			("\\High.par")
#define SUPER_CHALL_FILENAME		("\\Highest.par")

//** Change for localization
#define DEFAULT_INSTALL_PATH		("C:\\Program Files\\Haofeng Digital\\SPCS")
#define DEFAULT_DATA_PATH			("C:\\Program Files\\Haofeng Digital\\SPCS")	// old style, not on Win NT

#define FILTERAUDIOPATH				("\\Audio\\*.*")
#define FILTERSOUNDPATH				("\\Sound\\*.*")				// duration impacts delays, see audio.cpp
#define MEDIUMSOUNDPATH				("\\Sound\\Medium Sound1.wav")	// Med_Sound1.wav 1.98 sec., Med_Sound2.wav		1.95 sec.
#define HIGHSOUNDPATH				("\\Sound\\High Sound1.wav")		// High_Sound1.wav	1.52 sec., High_Sound2.wav 1.24 sec.
#define HUNDREDSOUNDPATH			("\\Sound\\100 Points.wav")		// Hundred sound 7.81 sec.

#define ARTIFACTSOUND				("\\Sound\\Artifact.wav")			// Artifact sound 0.17 sec.
#define CLICKSOUND					("\\Sound\\Click.wav")			// Click sound 0.04 sec

#define TRACKSOUND					("\\Audio\\SoundTrack.mp3")

#define FNAME_COACH					("Coach.exe")								// Default
#define DEFAULT_COACH_PATH			("\\Content\\Applications\\QSGENG.pdf")	// used relative to INSTALL_PATH

#define FNAME_QSG					("QSGENG.pdf")				// Default
#define DEFAULT_QSG_PATH			("\\Documentation\\QSGENG.pdf")	// used relative to INSTALL_PATH

#define FNAME_QSG_EN				("QSGENG.pdf")				// English
#define DEFAULT_QSG_PATH_EN			("\\Documentation\\QSGENG.pdf")	// used relative to INSTALL_PATH

#define FNAME_QSG_JA				("QSGJAP.pdf")				// Japanese
#define DEFAULT_QSG_PATH_JA			("\\Documentation\\QSGJAP.pdf")	// used relative to INSTALL_PATH

#define FNAME_DEFAULT_HELP			("emENG.chm")					// default help file in English

#define CSIDL_COMMON_APPDATA        0x0023        // All Users\Application Data
#define DELIMITER		'\\'	// delimiter between directory segments
#define DEV_DELIMITER	':'		// end of search, device root
#define	SUCCESS			1

#define NULL_STRING					""
// Install types
#define	ALL_USERS_COMMON_DATA		1	//	NULL is undetermined
#define	ALL_USERS_PRIVATE_DATA		2
#define	ALL_USERS_NETWORK_DATA		3
#define	LOCAL_USER_PRIVATE_DATA		4	//	DEFAULT
#define	LOCAL_USER_NETWORK_DATA		5

// Registry Sections
#define	REG_KEY_FP_SETTINGS		("Software\\Haofeng Digital\\SPCS\\Settings")
#define REG_KEY_COMPANY			("Haofeng Digital")
#define REG_KEY_PRODUCT			("SPCS")

#define REG_SECT_CONSTANTS		("Constants")
#define	REG_SECT_SETTINGS		("Settings")

#define REG_KEY_SHELL			("ShellMode")	// currently defined as BOOLEAN TRUE/FALSE (0/1) for New HTML Shell

// #define REG_SECT_GAMES			_T("Games")
// #define REG_SECT_MCURRENT		_T("Games\\MCurrent")
// #define	REG_KEY_CUSTOM_MEADOW	_T("Custom Meadow")
// #define REG_KEY_DATALOG			_T("Datalog Enabled") // mostly used by games

#define REG_KEY_INSTALLPATH		("Install Path")
#define REG_KEY_DATAPATH		("Data Path")
#define REG_KEY_TUTORIALPATH	("Tutorial Path")
#define REG_KEY_COACHPATH		("Coach Path")
#define REG_KEY_STRACKPATH		("Soundtrack Path")
#define REG_KEY_STRACKENABLE	("Soundtrack Enable")

#define REG_KEY_LANG_CHOICE		("Language")	// AUTO or other enumerated language
#define REG_KEY_QSGPATH			("QSG Path")	// default

#define	REG_KEY_INSTALLTYPE		("Install Type")
#define REG_KEY_SENSORTYPE		("Sensor Type")
#define	REG_KEY_VERSION			("Version")
#define REG_KEY_DEBUG			("DEBUG")
#define REG_KEY_LASTUPDATE		("Last Update")
#define REG_KEY_LAST_AGC_LEVEL	("AgcLastPowerLevel")

#define REG_KEY_CURRENT_USER	("Current User")
#define REG_KEY_USER_NAME		("User Name")

#define REG_KEY_URL_REGISTRATION	("URL Registration")
#define	REG_KEY_URL_PRODUCTS	("URL Products")
#define	REG_KEY_URL_SUPPORT		("URL Support")
#define	REG_KEY_URL_BBS			("URL BBS")

#define REG_KEY_NAGWEB			("Lock Web Access")
#define	REG_KEY_TCP_ENABLE		("TCP Enabled")
#define REG_KEY_PPG_ENABLE		("PPG Enabled")
#define REG_KEY_IBI_ENABLE		("IBI Enabled")
#define REG_KEY_FFT_ENABLE		("FFT Enabled")

#define REG_KEY_PORT			("Port")
#define REG_KEY_TCP_DEBUG		("TCP Debug")

#define REG_KEY_CUSTOM_HOMEPAGE	("Homepage")

#define URL_REGISTRATION		("http://www.digitalbamboo.com")
#define URL_PRODUCTS			("http://www.digitalbamboo.com")
#define URL_SUPPORT				("http://www.digitalbamboo.com")
#define URL_BBS					("http://www.digitalbamboo.com")

#define	REG_VAL_SENSORTYPE		("USB")		//default sensor value

#define REG_KEY_LAST_BUTTON		("Last Button")	// function of last button
#define OPEN_TUTORIAL			(0)		// boolean value
#define OPEN_QSG				(1)

#define PATH_COMPANY			("\\Haofeng Digital")
#define PATH_PRODUCT			("\\SPCS")

//	for GAMES - deprecated in V3
#define	REG_KEY_ENABLED			("Enabled")

// all DWORD
#define	REG_KEY_CMD				("CMD")		// read, incoming command
#define REG_SECT_STATUS			("Status")
#define REG_KEY_SESSION_TIME	("STIME")		// session time
#define REG_KEY_STATE			("SSTAT")		// 0=INIT, 1=RUN, 2=STOPPED
#define REG_KEY_SCORE			("S")			// LOW, MEDIUM, HIGH
#define	REG_KEY_HEART_RATE		("HR")		// Heart Rate
#define REG_KEY_ASCORE			("AS")		// 0 - 100
#define REG_KEY_EP				("EP")
#define REG_KEY_IBI				("IBI")
#define	REG_KEY_PULSE			("PPG")
#define REG_KEY_ESCORE			("ES")		// Experimental Extended Score

#endif
