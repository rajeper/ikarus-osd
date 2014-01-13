//-----------------------------------------------------------------------------
// PluginDLL.h
//-----------------------------------------------------------------------------
#ifndef _PluginDLL_h_
#define _PluginDLL_h_

#pragma pack (push, r1, 1) // Make sure the struct is byte aligned

//-----------------------------------------------------------------------------
// Two main data structures are used. This is the first one:
//
// This data struct is filled by AeroSIM RC with the simulation data, and sent to the plugin
//-----------------------------------------------------------------------------
struct TDataFromAeroSimRC
{
  unsigned short nStructSize;  // size in bytes of TDataFromAeroSimRC

  //---------------------
  // Integration Time
  //---------------------
  float  Simulation_fIntegrationTimeStep;  // integration time step in seconds. This is the simulated time since last call to AeroSIMRC_Plugin_Run()

  //---------------------
  // Channels
  //---------------------
  #define AEROSIMRC_MAX_CHANNELS 39
  float  Channel_afValue_TX[AEROSIMRC_MAX_CHANNELS];   // [-1, 1] channel positions at TX sticks (i.e. raw stick positions)
  float  Channel_afValue_RX[AEROSIMRC_MAX_CHANNELS];   // [-1, 1] channel positions at RX (i.e. after TX mixes)

  // Use the following constants as indexes for the channel arrays
  // The simulator uses internally the channel numbers for Transmitter Mode 2 (regardless of mode selected by user)
  #define CH_AILERON              0
  #define CH_ELEVATOR             1
  #define CH_THROTTLE             2
  #define CH_RUDDER               3
  #define CH_5                    4
  #define CH_6                    5
  #define CH_7                    6
  #define CH_PLUGIN_1            22 // This channel is mapped by user to any real channel number
  #define CH_PLUGIN_2            23 // This channel is mapped by user to any real channel number

  //---------------------
  // OSD
  //---------------------
  // Video buffer for OSD is 512 x 512 pixels, 4 bytes per pixel: R G B A; The first 4 bytes are the Top-Left corner pixel
  // Set OSD_nWindow_DX and OSD_nWindow_DY in struct TDataToAeroSimRC to the actual size to be displayed
  unsigned char *OSD_pVideoBuffer;

  //---------------------
  // Menu
  //---------------------
  // This variable represent the custom menu status. E.g. 0x000001 means that first menu item is ticked
  // Command  menu item bits are set to 1 when selected, but cleared in the next cycle.
  // Checkbox menu item bits remain 1 until unchecked by user, or cleared in TDataToAeroSimRC::Menu_nFlags_MenuItem_New_CheckBox_Status
  unsigned long Menu_nFlags_MenuItem_Status;

  //---------------------
  // Model Initial Position in current scenario
  //---------------------
  float  Scenario_fInitialModelPosX;    float Scenario_fInitialModelPosY;  float Scenario_fInitialModelPosZ; // (m) Model Initial Position on runway
  float  Scenario_fInitialModelHeading; float Scenario_fInitialModelPitch; float Scenario_fInitialModelRoll; // (m) Model Initial Attitude on runway

  //---------------------
  // WayPoints
  // The Description string can be used to add more information to the waypoint such as Altitude, WP Type (Overfly, Landing, CAP), Bearing, etc.
  //---------------------
  float  Scenario_fWPHome_X; float Scenario_fWPHome_Y;  float Scenario_fWPHome_Lat; float Scenario_fWPHome_Long;   const char *Scenario_strWPHome_Description; // (m, deg, string)
  float  Scenario_fWPA_X;    float Scenario_fWPA_Y;     float Scenario_fWPA_Lat;    float Scenario_fWPA_Long;      const char *Scenario_strWPA_Description;    // (m, deg, string)
  float  Scenario_fWPB_X;    float Scenario_fWPB_Y;     float Scenario_fWPB_Lat;    float Scenario_fWPB_Long;      const char *Scenario_strWPB_Description;    // (m, deg, string)
  float  Scenario_fWPC_X;    float Scenario_fWPC_Y;     float Scenario_fWPC_Lat;    float Scenario_fWPC_Long;      const char *Scenario_strWPC_Description;    // (m, deg, string)
  float  Scenario_fWPD_X;    float Scenario_fWPD_Y;     float Scenario_fWPD_Lat;    float Scenario_fWPD_Long;      const char *Scenario_strWPD_Description;    // (m, deg, string)

  //---------------------
  // Model data
  //---------------------
  float  Model_fPosX;      float Model_fPosY;       float Model_fPosZ;    // m      Model absolute position in scenario (X=Right, Y=Front, Z=Up)
  float  Model_fVelX;      float Model_fVelY;       float Model_fVelZ;    // m/s    Model velocity
  float  Model_fAngVelX;   float Model_fAngVelY;    float Model_fAngVelZ; // rad/s  Model angular velocity (useful to implement gyroscopes)
  float  Model_fAccelX;    float Model_fAccelY;     float Model_fAccelZ;  // m/s/s  Model acceleration (useful to implement accelerometers)

  double Model_fLatitude; double Model_fLongitude;   // deg    Model Position in Lat/Long coordinates

  float  Model_fHeightAboveTerrain;            // m

  float  Model_fHeading;                       // rad [-PI,   PI  ] 0 = North, PI/2 = East, PI = South, - PI/2 = West
  float  Model_fPitch;                         // rad [-PI/2, PI/2] Positive pitch when nose up
  float  Model_fRoll;                          // rad [-PI,   PI  ] Positive roll when right wing Up

  // Wind
  float  Model_fWindVelX;   float Model_fWindVelY;   float Model_fWindVelZ;    // m/s   Velocity of the wind (with gusts) at model position (useful to compute air vel)

  // Engine/Motor Revs per minute
  float  Model_fEngine1_RPM;
  float  Model_fEngine2_RPM;
  float  Model_fEngine3_RPM;
  float  Model_fEngine4_RPM;

  // Battery (electric models)
  float  Model_fBatteryVoltage;          // V
  float  Model_fBatteryCurrent;          // A
  float  Model_fBatteryConsumedCharge;   // Ah
  float  Model_fBatteryCapacity;         // Ah

  // Fuel (gas & jet models)
  float  Model_fFuelConsumed;            // l
  float  Model_fFuelTankCapacity;        // l
};



//-----------------------------------------------------------------------------
// This is the second main data structure.
//
// This data struct is filled by the plugin and returned to AeroSIM RC
//-----------------------------------------------------------------------------
struct TDataToAeroSimRC
{
  //---------------------
  // Size in bytes of struct TDataToAeroSimRC (this is filled by AeroSIM RC)
  //---------------------
  unsigned short nStructSize;

  //---------------------
  // The plugin can compose a string that will be displayed in AeroSIM RC for debugging purposes
  // Edit file config2.txt and set I_AM_A_PLUGIN_DEVELOPER to 1 to enable the menu option to display this string
  // Once enabled, this option can be found in the plugin submenu at the Menu Top Bar
  //---------------------
  const char    *Debug_pucOnScreenInfoText;

  //---------------------
  // Channels
  //---------------------
  //  TX channel (i.e. stick positions) before mixes in the simulated TX
  unsigned char  Channel_abOverride_TX[AEROSIMRC_MAX_CHANNELS];  // true to override the value of this channel
  float          Channel_afNewValue_TX[AEROSIMRC_MAX_CHANNELS];  // new value for TX sticks

  // RX channels as received at RX, including simulated TX mixes
  unsigned char  Channel_abOverride_RX[AEROSIMRC_MAX_CHANNELS];  // true to override the value of this channel
  float          Channel_afNewValue_RX[AEROSIMRC_MAX_CHANNELS];  // new value for RX channels

  //---------------------
  // Model data to be overridden
  //---------------------
  float Model_fNewPosX;     float Model_fNewPosY;     float Model_fNewPosZ;    // m    (X = right, Y = front, Z = up)
  float Model_fNewVelX;     float Model_fNewVelY;     float Model_fNewVelZ;    // m/s
  float Model_fNewAngVelX;  float Model_fNewAngVelY;  float Model_fNewAngVelZ; // rad/s

  float Model_fNewHeading; // rad
  float Model_fNewPitch;   // rad
  float Model_fNewRoll;    // rad

  enum EOverrideFlags
  {
    OVR_POS     = 0x00000001,
    OVR_VEL     = 0x00000002,
    OVR_ANG_VEL = 0x00000004,
    OVR_HPR     = 0x00000008,  // Heading, Pitch and Roll
  };

  // Used to specify which data is to be overridden. E.g. Model_nOverrideFlags = OVR_POS | OVR_VEL
  unsigned long  Model_nOverrideFlags;

  //---------------------
  // Menu Check Box update from plugin
  //---------------------
  unsigned long  Menu_nFlags_MenuItem_New_CheckBox_Status; // Allows to change the status of Menu Items of type CheckBox (see example 'Reset')

  //---------------------
  // OSD Data
  //---------------------
  // Set to true to show the OSD. This saves the time required to render an empty OSD when nothing is to be displayed.
  unsigned char  OSD_bShow;
  
  // Set to true if the OSD video buffer was written with new data. This saves the time required to upload the new image to the graphics card.
  unsigned char  OSD_bHasChanged;
  
  // Defines the window size in the VideoBufferOSD to be drawn (from top-left corner of the 512x512 video buffer)
  unsigned short OSD_nWindow_DX;
  unsigned short OSD_nWindow_DY;

  // 1.0 to expand OSD to the size of the screen (while keeping tha spect ratio).
  // Use 0.9 to leave a nice gap between the OSD and the edges of the window.
  float          OSD_fScale;
};


//-----------------------------------------------------------------------------
// Additional data structs for customising the Top Bar Menu
//
// Top Menu Bar: User defined Menu Items to set flags and trigger events in the DLL
//-----------------------------------------------------------------------------
struct TPluginMenuItem
{
  #define MIT_COMMAND    0x00000001   // Command Menu Item. Click once to execute command.
  #define MIT_CHECKBOX   0x00000002   // Menu Item with checkbox. Click to check/uncheck box.
  #define MIT_SEPARATOR  0x00000080   // Line used to separate menu items into groups (placed below menu item)
  
  unsigned long  eType;               // Menu Item Type. Valid values are MIT_xxx defined above

  char          *strName;             // NULL or "" for menu item not in use
};


struct TPluginInit
{
  unsigned long  nStructSize;         // Size in bytes of TPluginMenu

  char          *strMenuTitle;        // Custom menu title

  #define MAX_DLL_USER_MENU_ITEMS   16
  TPluginMenuItem  atMenuItem[MAX_DLL_USER_MENU_ITEMS];  // Custom menu items

  const char    *strPluginFolder;   // Path to the plugin folder
  const char    *strOutputFolder;   // Writeable folder where new files can be created (to write configuration data, etc.)
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define AeroSIMRC_DLL_EXPORT  extern "C" __declspec(dllexport)

//-----------------------------------------------------------------------------
// Handling Compatibility with future AeroSIM RC versions
//-----------------------------------------------------------------------------
//
// In newer versions of AeroSIM RC, the structs TDataFromAeroSimRC and TDataToAeroSimRC
// may be extended with new data.
//
// The new data will always be appended at the end of the previous struct version.
//
// This insures backward compatibility for plugins created with older versions of AeroSIM RC
//
// AeroSIM RC will prevent loading a plugin which struct sizes are larger than the struct sizes of the installed AeroSIM RC.
//
//-----------------------------------------------------------------------------
AeroSIMRC_DLL_EXPORT inline void AeroSIMRC_Plugin_ReportStructSizes(unsigned long *pnSizeOf_TDataFromAeroSimRC,
                                                                    unsigned long *pnSizeOf_TDataToAeroSimRC,
                                                                    unsigned long *pnSizeOf_TPluginInit)
{
  // This function is already written for you. No need to modify it.
  *pnSizeOf_TDataFromAeroSimRC = sizeof(TDataFromAeroSimRC );
  *pnSizeOf_TDataToAeroSimRC   = sizeof(TDataToAeroSimRC   );
  *pnSizeOf_TPluginInit        = sizeof(TPluginInit        );
}


//-----------------------------------------------------------------------------
//
//   Used to optionally define menu items in the top menu bar.
//
//   Also used to initialise the plugin (e.g. loading data files)
//
//-----------------------------------------------------------------------------
AeroSIMRC_DLL_EXPORT void  AeroSIMRC_Plugin_Init(TPluginInit *pTPluginInit);


//-----------------------------------------------------------------------------
//
//
//   This is the  *HEART*  of the plugin. It is called every simulation cycle.
//
//
//-----------------------------------------------------------------------------
AeroSIMRC_DLL_EXPORT void  AeroSIMRC_Plugin_Run(const TDataFromAeroSimRC  *pTDataFromAeroSimRC,
                                                      TDataToAeroSimRC    *ptDataToAeroSimRC);



#pragma pack (pop, r1)

#endif
