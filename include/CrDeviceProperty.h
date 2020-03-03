#ifndef CRDEVICEPROPERTY_H
#define CRDEVICEPROPERTY_H

#include "CrDefines.h"

#pragma warning (push)
#pragma warning (disable: 4480)	// warning C4480: 'specifying underlying type for enum' are not treated as standard in VS 2010.

#ifdef CR_SDK_EXPORTS
#define SCRSDK_API __declspec(dllexport)
#else
#define SCRSDK_API __declspec(dllimport)
#endif

namespace SCRSDK
{
enum CrDevicePropertyCode
{
	CrDeviceProperty_Undefined 			= 0,

	CrDeviceProperty_S1,
	CrDeviceProperty_AEL,
	CrDeviceProperty_FEL,
	CrDeviceProperty_AFL,
	CrDeviceProperty_AWBL,

	CrDeviceProperty_FNumber 			= 0x0100,
	CrDeviceProperty_ExposureBiasCompensation,
	CrDeviceProperty_FlashCompensation,
	CrDeviceProperty_ShutterSpeed,
	CrDeviceProperty_IsoSensitivity,
	CrDeviceProperty_ExposureProgramMode,
	CrDeviceProperty_FileType,
	CrDeviceProperty_JpegQuality,
	CrDeviceProperty_WhiteBalance,
	CrDeviceProperty_FocusMode,
	CrDeviceProperty_MeteringMode,
	CrDeviceProperty_FlashMode,
	CrDeviceProperty_reserved1,
	CrDeviceProperty_reserved2,
	CrDeviceProperty_DriveMode,
	CrDeviceProperty_DRO,
	CrDeviceProperty_ImageSize,
	CrDeviceProperty_AspectRatio,
	CrDeviceProperty_reserved3,
	CrDeviceProperty_FocusArea,
	CrDeviceProperty_reserved4,
	CrDeviceProperty_Colortemp,
	CrDeviceProperty_ColorTuningAB,
	CrDeviceProperty_ColorTuningGM,
	
	CrDeviceProperty_LiveViewDisplayEffect,
	CrDeviceProperty_StillImageStoreDestination,
	CrDeviceProperty_PriorityKeySettings,
	CrDeviceProperty_reserved5,
	CrDeviceProperty_reserved6,
	CrDeviceProperty_DateTime_Settings,
	CrDeviceProperty_NearFar,
	CrDeviceProperty_reserved7,
	CrDeviceProperty_AF_Area_Position,
	CrDeviceProperty_reserved8,
	CrDeviceProperty_reserved9,
	
	CrDeviceProperty_S2 = 0x0500,
	CrDeviceProperty_reserved10,
	CrDeviceProperty_reserved11,
	CrDeviceProperty_reserved12,
	CrDeviceProperty_reserved13,
	CrDeviceProperty_reserved14,
	CrDeviceProperty_Still_Image_Trans_Size,
	CrDeviceProperty_reserved15,
	CrDeviceProperty_LiveView_Image_Quality,
	CrDeviceProperty_reserved16,
	CrDeviceProperty_reserved17,
	
	CrDeviceProperty_GetOnly = 0x0700,

	CrDeviceProperty_SnapshotInfo,
	CrDeviceProperty_BatteryRemain,
	CrDeviceProperty_BatteryLevel,
	CrDeviceProperty_EstimatePictureSize,
	CrDeviceProperty_reserved18,	
	CrDeviceProperty_LiveViewStatus,
	CrDeviceProperty_FocusIndication,
	CrDeviceProperty_MediaSLOT1_Status,
	CrDeviceProperty_MediaSLOT1_RemainingNumber,
	CrDeviceProperty_MediaSLOT1_RemainingTime,
	CrDeviceProperty_reserved19,
	CrDeviceProperty_reserved20,	
	CrDeviceProperty_MediaSLOT2_Status,
	CrDeviceProperty_reserved21,	
	CrDeviceProperty_MediaSLOT2_RemainingNumber,
	CrDeviceProperty_MediaSLOT2_RemainingTime,
	CrDeviceProperty_reserved22,	
	CrDeviceProperty_reserved23,	
	CrDeviceProperty_reserved24,	
	CrDeviceProperty_reserved25,	
	CrDeviceProperty_LiveView_Area,
	CrDeviceProperty_reserved26,	
	CrDeviceProperty_reserved27,	
	CrDeviceProperty_reserved28,	
	CrDeviceProperty_reserved29,	
	
	CrDeviceProperty_MaxVal	= 0x1000,
};

enum CrLiveViewPropertyCode
{
	CrLiveViewProperty_AF_Area_Position = CrDevicePropertyCode::CrDeviceProperty_AF_Area_Position,

	CrLiveViewProperty_LiveViewUndefined = CrDevicePropertyCode::CrDeviceProperty_MaxVal,
	CrLiveViewProperty__LiveViewMaxVal = 0x2000,
};

// =========================== S1, AEL, FEL, AFL, AWBL ===========================
enum CrLockIndicator : CrInt16u
{
	CrLockIndicator_Unknown = 0x0000,
	CrLockIndicator_Unlocked,
	CrLockIndicator_Locked,
};
// ===============================================================================


enum CrPropValueSet : CrInt16
{
	CrPropValueMinus1 = -1,
	CrPropValuePlus1 = 1,
};

// FNumber
// type: CrDataType_UInt16
// value = F number * 100
enum CrFnumberSet : CrInt16u
{
	CrFnumber_Unknown = 0xFFFE, // Display "--"
	CrFnumber_Nothing = 0xFFFF, // Nothing to display	
};

// ExposureBiasCompensation
// type: CrDataType_UInt16
// value: compensation value * 1000

// FlashCompensation
// type: CrDataType_UInt16
// value = compensation value * 1000

// ShutterSpeed
// type: CrDataType_UInt32
// value = low-order word / high-order word
enum CrShutterSpeedSet : CrInt32u
{
	CrShutterSpeed_Bulb = 0x00000000,
	CrShutterSpeed_Nothing = 0xFFFFFFFF, // Nothing to display
};

// IsoSensitivity
// type: CrDataType_UInt32
// value: bit 28-31 ISO mode (CrISOMode), bit 0-27 ISO value
enum CrISOMode : CrInt32u
{
	CrISO_Normal = 0x0,	// ISO setting Normal
	CrISO_MultiFrameNR = 0x1,	// Multi Frame NR
	CrISO_MultiFrameNR_High = 0x2,	// Multi Frame NR High
	CrISO_AUTO = 0xFFFFFF,
};
// ===============================================================================

// ExposureProgramMode
enum CrExposureProgram : CrInt32u
{
	CrExposure_M_Manual				= 0x00000001,
	CrExposure_P_Auto,
	CrExposure_A_AperturePriority,
	CrExposure_S_ShutterSpeedPriority,
	CrExposure_Portrait,
	CrExposure_Auto					= 0x00008000,
	CrExposure_Auto_Plus,
	CrExposure_P_A,
	CrExposure_P_S,
	CrExposure_Sprots_Action,
	CrExposure_Sunset,
	CrExposure_Night,
	CrExposure_Landscape,
	CrExposure_Macro,
	CrExposure_HandheldTwilight,
	CrExposure_NightPortrait,
	CrExposure_AntiMotionBlur,
	CrExposure_Pet,
	CrExposure_Gourmet,
	CrExposure_Fireworks,
	CrExposure_HighSensitivity,
	CrExposure_MemoryRecall				= 0x00008020,
	CrExposure_ContinuousPriority_AE_8pics		= 0x00008031,
	CrExposure_ContinuousPriority_AE_10pics,
	CrExposure_ContinuousPriority_AE_12pics,
	CrExposure_3D_SweepPanorama			= 0x00008040,
	CrExposure_SweepPanorama,
	CrExposure_Movie_P				= 0x00008050,
	CrExposure_Movie_A,
	CrExposure_Movie_S,
	CrExposure_Movie_M,
	CrExposure_Movie_Auto,
	CrExposure_Movie_HiFrameRate_P,
	CrExposure_Movie_HiFrameRate_A,
	CrExposure_Movie_HiFrameRate_S,
	CrExposure_Movie_HiFrameRate_M,
	CrExposure_Movie_SQMotion_P,
	CrExposure_Movie_SQMotion_A,
	CrExposure_Movie_SQMotion_S,
	CrExposure_Movie_SQMotion_M,
	CrExposure_HiFrameRate_P			= 0x00008080,
	CrExposure_HiFrameRate_A,
	CrExposure_HiFrameRate_S,
	CrExposure_HiFrameRate_M,
	CrExposure_SQMotion_P,
	CrExposure_SQMotion_A,
	CrExposure_SQMotion_S,
	CrExposure_SQMotion_M,
};

// FileType
enum CrFileType : CrInt16u
{
	CrFileType_Jpeg = 0x0001,
	CrFileType_Raw,
	CrFileType_RawJpeg,
};

// JpegQuality
enum CrJpegQuality : CrInt16u
{
	CrJpegQuality_Unknown  = 0x0000,
	CrJpegQuality_ECO      = 0x0001,
	CrJpegQuality_Standard = 0x0002,
	CrJpegQuality_Fine     = 0x0003,
	CrJpegQuality_ExFine   = 0x0004,
};

// WhiteBalance
enum CrWhiteBalanceSetting : CrInt16u
{
	CrWhiteBalance_AWB				= 0x0000,
	CrWhiteBalance_Underwater_Auto,
	CrWhiteBalance_Daylight				= 0x0011,
	CrWhiteBalance_Shadow,
	CrWhiteBalance_Cloudy,
	CrWhiteBalance_Tungsten,
	CrWhiteBalance_Fluorescent			= 0x0020,
	CrWhiteBalance_Fluorescent_WarmWhite,
	CrWhiteBalance_Fluorescent_CoolWhite,
	CrWhiteBalance_Fluorescent_DayWhite,
	CrWhiteBalance_Fluorescent_Daylight,
	CrWhiteBalance_Flush				= 0x0030,
	CrWhiteBalance_ColorTemp			= 0x0100,
	CrWhiteBalance_Custom_1,
	CrWhiteBalance_Custom_2,
	CrWhiteBalance_Custom_3,
	CrWhiteBalance_Custom,
};

// FocusMode
enum CrFocusMode : CrInt16u
{
	CrFocus_MF					= 0x0001,
	CrFocus_AF_S,
	CrFocus_AF_C,
	CrFocus_AF_A,
	CrFocus_AF_D,
	CrFocus_DMF,
	CrFocus_PF,
};

// MeteringMode
enum CrMeteringMode : CrInt16u
{
	CrMetering_Average				= 0x0001,
	CrMetering_CenterWeightedAverage,
	CrMetering_MultiSpot,
	CrMetering_CenterSpot,
	CrMetering_Multi,
	CrMetering_CenterWeighted,
	CrMetering_EntireScreenAverage,
	CrMetering_Spot_Standard,
	CrMetering_Spot_Large,
	CrMetering_HighLightWeighted,
};

// FlashMode
enum CrFlashMode : CrInt16u
{
	CrFlash_Auto					= 0x0001,
	CrFlash_Off,
	CrFlash_Fill,
	CrFlaxh_ExternalSync,
	CrFlaxh_SlowSync,
	CrFlaxh_RearSync,
};

// DriveMode
enum CrDriveMode : CrInt32u
{
	CrDrive_Single					= 0x00000001,
	CrDrive_Continuous_Hi				= 0x00010001,
	CrDrive_Continuous_Hi_Plus,
	CrDrive_Continuous_Hi_Live,
	CrDrive_Continuous_Lo,
	CrDrive_Continuous,
	CrDrive_Continuous_SpeedPriority,
	CrDrive_Continuous_Mid,
	CrDrive_Continuous_Mid_Live,
	CrDrive_Continuous_Lo_Live,
	CrDrive_SingleBurstShooting_lo			= 0x00011001,
	CrDrive_SingleBurstShooting_mid,
	CrDrive_SingleBurstShooting_hi,
	CrDrive_Timelapse				= 0x00020001,
	CrDrive_Timer_2s				= 0x00030001,
	CrDrive_Timer_5s,
	CrDrive_Timer_10s,
	CrDrive_Continuous_Bracket_03Ev_3pics		= 0x00040301,
	CrDrive_Continuous_Bracket_03Ev_5pics,
	CrDrive_Continuous_Bracket_03Ev_9pics,
	CrDrive_Continuous_Bracket_05Ev_3pics,
	CrDrive_Continuous_Bracket_05Ev_5pics,
	CrDrive_Continuous_Bracket_05Ev_9pics,
	CrDrive_Continuous_Bracket_07Ev_3pics,
	CrDrive_Continuous_Bracket_07Ev_5pics,
	CrDrive_Continuous_Bracket_07Ev_9pics,
	CrDrive_Continuous_Bracket_10Ev_3pics,
	CrDrive_Continuous_Bracket_10Ev_5pics,
	CrDrive_Continuous_Bracket_10Ev_9pics,
	CrDrive_Continuous_Bracket_20Ev_3pics,
	CrDrive_Continuous_Bracket_20Ev_5pics,
	CrDrive_Continuous_Bracket_30Ev_3pics,
	CrDrive_Continuous_Bracket_30Ev_5pics,
	CrDrive_Single_Bracket_03Ev_3pics		= 0x00050001,
	CrDrive_Single_Bracket_03Ev_5pics,
	CrDrive_Single_Bracket_03Ev_9pics,
	CrDrive_Single_Bracket_05Ev_3pics,
	CrDrive_Single_Bracket_05Ev_5pics,
	CrDrive_Single_Bracket_05Ev_9pics,
	CrDrive_Single_Bracket_07Ev_3pics,
	CrDrive_Single_Bracket_07Ev_5pics,
	CrDrive_Single_Bracket_07Ev_9pics,
	CrDrive_Single_Bracket_10Ev_3pics,
	CrDrive_Single_Bracket_10Ev_5pics,
	CrDrive_Single_Bracket_10Ev_9pics,
	CrDrive_Single_Bracket_20Ev_3pics,
	CrDrive_Single_Bracket_20Ev_5pics,
	CrDrive_Single_Bracket_30Ev_3pics,
	CrDrive_Single_Bracket_30Ev_5pics,
	CrDrive_WB_Bracket_Lo				= 0x00060001,
	CrDrive_WB_Bracket_Hi,
	CrDrive_DRO_Bracket_Lo				= 0x00070001,
	CrDrive_DRO_Bracket_Hi,
	CrDrive_Continuous_Timer_3pics			= 0x00080001,
	CrDrive_Continuous_Timer_5pics,
	CrDrive_Continuous_Timer_2s_3pics,
	CrDrive_Continuous_Timer_2s_5pics,
	CrDrive_Continuous_Timer_5s_3pics,
	CrDrive_Continuous_Timer_5s_5pics,
	CrDrive_LPF_Bracket				= 0x10000001,
	CrDrive_RemoteCommander,
	CrDrive_MirrorUp,
	CrDrive_SelfPortrait_1,
	CrDrive_SelfPortrait_2,
};

// DRO
enum CrDRangeOptimizer : CrInt16u
{
	CrDRangeOptimizer_Off				= 0x0000,
	CrDRangeOptimizer_On,
	CrDRangeOptimizer_Plus				= 0x0010,
	CrDRangeOptimizer_Plus_Manual_1,
	CrDRangeOptimizer_Plus_Manual_2,
	CrDRangeOptimizer_Plus_Manual_3,
	CrDRangeOptimizer_Plus_Manual_4,
	CrDRangeOptimizer_Plus_Manual_5,
	CrDRangeOptimizer_Auto				= 0x0020,
	CrDRangeOptimizer_HDR_Auto			= 0x0030,
	CrDRangeOptimizer_HDR_10Ev,
	CrDRangeOptimizer_HDR_20Ev,
	CrDRangeOptimizer_HDR_30Ev,
	CrDRangeOptimizer_HDR_40Ev,
	CrDRangeOptimizer_HDR_50Ev,
	CrDRangeOptimizer_HDR_60Ev,
};

// ImageSize
enum CrImageSize: CrInt16u
{
	CrImageSize_L	= 0x0001,
	CrImageSize_M,
	CrImageSize_S,
	CrImageSize_VGA,
};

// AspectRatio
enum CrAspectRatioIndex : CrInt16u
{
	CrAspectRatio_3_2	= 0x0001,
	CrAspectRatio_16_9,
	CrAspectRatio_4_3,
	CrAspectRatio_1_1,
};

// FocusArea
enum CrFocusArea : CrInt16u
{
	CrFocusArea_Unknown				= 0x0000,
	CrFocusArea_Wide,
	CrFocusArea_Zone,
	CrFocusArea_Center,
	CrFocusArea_Flexible_Spot_S,
	CrFocusArea_Flexible_Spot_M,
	CrFocusArea_Flexible_Spot_L,
	CrFocusArea_Expand_Flexible_Spot,
	CrFocusArea_Flexible_Spot,
	CrFocusArea_Lock_AF_Wide,
	CrFocusArea_Lock_AF_Zone,
	CrFocusArea_Lock_AF_Center,
	CrFocusArea_Lock_AF_Flexible_Spot_S,
	CrFocusArea_Lock_AF_Flexible_Spot_M,
	CrFocusArea_Lock_AF_Flexible_Spot_L,
	CrFocusArea_Lock_AF_Expand_Flexible_Spot,
	CrFocusArea_Lock_AF_Flexible_Spot,
	CrFocusArea_Tracking_Wide,
	CrFocusArea_Tracking_Zone,
	CrFocusArea_Tracking_Center,
	CrFocusArea_Tracking_Flexible_Spot_S,
	CrFocusArea_Tracking_Flexible_Spot_M,
	CrFocusArea_Tracking_Flexible_Spot_L,
	CrFocusArea_Tracking_Expand_Flexible_Spot,
	CrFocusArea_Tracking_Flexible_Spot,
};

// Colortemp
// type unsigned long
// value = color temp (K) step 100

enum CrColortemp : CrInt16u
{
	CrColortemp_Min = 0x0000,
	CrColortemp_Max = 0xFFFF,
};

// ColorTuningAB
// type signed short
// value < 0 : A value 0.25 step. Min -36 (eg. -4=A1)
// value > 0 : B value 0.25 step. Max 36 (eg. 4=B1)

// ColorTuningGM
// type signed short
// value < 0 : M value 0.25 step. Min -36 (eg. -4=M1)
// value > 0 : G value 0.25 step. Max 36 (eg. 4=G1)
	
enum CrColorTuning : CrInt8u
{
	CrColorTuning_Min = 0x00,
	CrColorTuning_Max = 0xFF,
};


// LiveViewDisplayEffect
enum CrLiveViewDisplayEffect : CrInt16u
{
	CrLiveViewDisplayEffect_Unknown = 0x0000,
	CrLiveViewDisplayEffect_ON,
	CrLiveViewDisplayEffect_OFF,
};

// StillImageStoreDestination
enum CrStillImageStoreDestination : CrInt16u
{
	CrStillImageStoreDestination_HostPC = 0x0001,
	CrStillImageStoreDestination_MemoryCard,
	CrStillImageStoreDestination_HostPCAndMemoryCard,
};

// Near/Far
enum CrNearFarVal : CrInt16u
{
	CrNearFar_None = 0x0000,
	CrNearFar_PLUS_S = 0x0101,
	CrNearFar_PLUS_M = 0x0103,
	CrNearFar_PLUS_L = 0x0107,
	CrNearFar_MINUS_S = 0x0201,
	CrNearFar_MINUS_M = 0x0203,
	CrNearFar_MINUS_L = 0x0207,
};

// Buffered image file count
// type unsigned short
// value number of buffered image files.

// Battery Residual Quantity
// type unsigned short
// 0~100: percentage of the battery residual quantity.
static const CrInt16u CrBatteryRemain_Untaken = 0xFFFF;

// Battery LV
enum CrBatteryLevel : CrInt32u
{
	CrBatteryLevel_PreEndBattery	= 0x00000001,
	CrBatteryLevel_1_4,								// Level 1/4
	CrBatteryLevel_2_4,
	CrBatteryLevel_3_4,
	CrBatteryLevel_4_4,
	CrBatteryLevel_1_3,								// Level 1/3
	CrBatteryLevel_2_3,
	CrBatteryLevel_3_3,
	CrBatteryLevel_USBPowerSupply = 0x00010000,	// USB Power Supply
	CrBatteryLevel_PreEnd_PowerSupply,
	CrBatteryLevel_1_4_PowerSupply,					// Level 1/4 with USB Power Supply
	CrBatteryLevel_2_4_PowerSupply,
	CrBatteryLevel_3_4_PowerSupply,
	CrBatteryLevel_4_4_PowerSupply,
	CrBatteryLevel_Fake = 0xFFFFFFFD,				// Fake
};

// Estimated Maximum File Size
// type unsigned long
// value Byte

enum CrWhiteBalanceInitialize : CrInt16u
{
	CrWhiteBalance_Initialized = 0x0001,	// Initialized PTP=0x01
};

// LiveView status
enum CrLiveViewStatus: CrInt16u
{
	CrLiveView_NotSupport				= 0x0000,	// Live view is not supported
	CrLiveView_Disable,								// Live view is supported, but can't get LV image
	CrLiveView_Enable,								// Live view is supported, application can get LV image
};

// Focus Indicator
enum CrFocusIndicator : CrInt32u
{
	CrFocusIndicator_Unlocked = 0x00000001, // Hide
	CrFocusIndicator_Focused_AF_S = 0x00000102, // Åú Show
	CrFocusIndicator_NotFocused_AF_S = 0x00000202, // Åú Blink
	CrFocusIndicator_Focused_AF_C = 0x00000103, // ((Åú)) Show
	CrFocusIndicator_NotFocused_AF_C = 0x00000203, // ((Åú)) Show
	CrFocusIndicator_TrackingSubject_AF_C = 0x00000303, // (( )) Show
};

// MediaSLOT_Status
enum CrSlotStatus : CrInt16u
{
	CrSlotStatus_OK = 0x0000,
	CrSlotStatus_NoCard,
	CrSlotStatus_CardError,
	CrSlotStatus_RecognizingOrLockedError
};

enum CrPriorityKeySettings : CrInt16u
{
	CrPriorityKey_CameraPosition = 0x0001,
	CrPriorityKey_PCRemote = 0x0002,
};
    
enum CrFocusFrameType : CrInt16u
{
    CrFocusFrameType_Unknown,
};
 
enum CrFocusFrameState : CrInt16u
{
    CrFocusFrameState_Unknown,
};

enum CrFrameInfoType : CrInt16u
{
    CrFrameInfoType_Unknown,
    CrFrameInfoType_FocusFrameInfo,
	CrFrameInfoType_Magnifier_Position,
	CrFrameInfoType_Select_CustomWB_Range,
};
    
enum CrPropertyEnableFlag : CrInt16
{
	CrEnableValue_NotSupported = -1,
	CrEnableValue_False = 0,
	CrEnableValue_True,
	CrEnableValue_DisplayOnly,
	CrEnableValue_SetOnly,
};

enum CrPropertyVariableFlag : CrInt16u
{
	CrEnableValue_Invalid,
	CrEnableValue_Invariable,
	CrEnableValue_Variable,
};

enum CrPropertyStillImageTransSize : CrInt16u
{
	CrPropertyStillImageTransSize_Original,
	CrPropertyStillImageTransSize_SmallSizeJPEG,
};

enum CrPropertyLiveViewImageQuality : CrInt16u
{
	CrPropertyLiveViewImageQuality_Low,
	CrPropertyLiveViewImageQuality_High,
};

class SCRSDK_API CrDeviceProperty
{
public:
	CrDeviceProperty();
	~CrDeviceProperty();

	CrDeviceProperty(const CrDeviceProperty& ref);

	CrDeviceProperty& operator =(const CrDeviceProperty& ref);

	void Alloc(const CrInt32u size, const CrInt32u getSetSize);

	bool IsGetEnableCurrentValue();

	bool IsSetEnableCurrentValue();

	void SetCode(CrInt32u code);
	CrInt32u GetCode();

	void SetValueType(CrDataType type);
	CrDataType GetValueType();

	void SetPropertyEnableFlag(CrPropertyEnableFlag flag);
	CrPropertyEnableFlag GetPropertyEnableFlag();

	void SetPropertyVariableFlag(CrPropertyVariableFlag flag);
	CrPropertyVariableFlag GetPropertyVariableFlag();

	void SetCurrentValue(CrInt64u value);
	CrInt64u GetCurrentValue();

	void SetCurrentStr(CrInt16u* str);
	CrInt16u* GetCurrentStr();

	void SetValueSize(CrInt32u size);
	CrInt32u GetValueSize();

	void SetValues(CrInt8u* value);
	CrInt8u* GetValues();

	void SetSetValueSize(CrInt32u size);
	CrInt32u GetSetValueSize();

	void SetSetValues(CrInt8u* value);
	CrInt8u* GetSetValues();

private:
	CrInt32u code;
	CrDataType valueType;
	CrPropertyEnableFlag enableFlag;
	CrPropertyVariableFlag variableFlag;
	CrInt64u currentValue;
	CrInt16u* currentStr;
	CrInt32u valuesSize;
	CrInt8u* values;
	CrInt32u getSetValuesSize;
	CrInt8u* getSetValues;
};

class SCRSDK_API CrLiveViewProperty
{
public:
	CrLiveViewProperty();
	~CrLiveViewProperty();
	CrLiveViewProperty(const CrLiveViewProperty& ref);
	CrLiveViewProperty& operator =(const CrLiveViewProperty& ref);

	void Alloc(const CrInt32u size);

	bool IsGetEnableCurrentValue();

	void SetCode(CrInt32u code);
	CrInt32u GetCode();

	void SetPropertyEnableFlag(CrPropertyEnableFlag flag);
	CrPropertyEnableFlag GetPropertyEnableFlag();

	void SetFrameInfoType(CrFrameInfoType type);
	CrFrameInfoType GetFrameInfoType();

	void SetValueSize(CrInt32u size);
	CrInt32u GetValueSize();

	void SetValue(CrInt8u* value);
	CrInt8u* GetValue();

private:
        CrInt32u code;
		CrPropertyEnableFlag enableFlag;
        CrFrameInfoType valueType;
		CrInt32u valueSize;
		CrInt8u* value;
};

class CrFocusFrameInfo
{
public:
	CrFocusFrameInfo();
	~CrFocusFrameInfo();

public:
    CrFocusFrameType type;
    CrFocusFrameState state;
    CrInt8u priority;
    CrInt32u xNumerator;
    CrInt32u xDenominator;
    CrInt32u yNumerator;
    CrInt32u yDenominator;
    CrInt32u width;
    CrInt32u height;
};

class CrMagPosInfo
{
public:
	CrMagPosInfo();
	~CrMagPosInfo();

public:
	CrInt32u xNumerator;
	CrInt32u xDenominator;
	CrInt32u yNumerator;
	CrInt32u yDenominator;
    CrInt32u width;
    CrInt32u height;
};

}

#pragma warning (pop)

#endif //CRDEVICEPROPERTY_H
