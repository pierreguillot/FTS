
#define VSTCALLBACK
typedef long t_fstPtrInt; /* pointer sized int */
typedef int t_fstInt32; /* 32bit int */


#define FST_UNKNOWN(x) x
#define FST_UNKNOWN_ENUM(x) x = 98765 + __LINE__
#define FST_ENUM_EXP(x, y) x = y
typedef enum {
  audioMasterVersion = 1,

    FST_UNKNOWN_ENUM(audioMasterAutomate),
    FST_UNKNOWN_ENUM(audioMasterBeginEdit),
    FST_UNKNOWN_ENUM(audioMasterCanDo),
    FST_UNKNOWN_ENUM(audioMasterCloseWindow),
    FST_UNKNOWN_ENUM(audioMasterCurrentId),
    FST_UNKNOWN_ENUM(audioMasterEndEdit),
    FST_UNKNOWN_ENUM(audioMasterGetAutomationState),
    FST_UNKNOWN_ENUM(audioMasterGetBlockSize),
    FST_UNKNOWN_ENUM(audioMasterGetCurrentProcessLevel),
    FST_UNKNOWN_ENUM(audioMasterGetDirectory),
    FST_UNKNOWN_ENUM(audioMasterGetInputLatency),
    FST_UNKNOWN_ENUM(audioMasterGetLanguage),
    FST_UNKNOWN_ENUM(audioMasterGetNextPlug),
    FST_UNKNOWN_ENUM(audioMasterGetNumAutomatableParameters),
    FST_UNKNOWN_ENUM(audioMasterGetOutputLatency),
    FST_UNKNOWN_ENUM(audioMasterGetOutputSpeakerArrangement),
    FST_UNKNOWN_ENUM(audioMasterGetParameterQuantization),
    FST_UNKNOWN_ENUM(audioMasterGetPreviousPlug),
    FST_UNKNOWN_ENUM(audioMasterGetProductString),
    FST_UNKNOWN_ENUM(audioMasterGetSampleRate),
    FST_UNKNOWN_ENUM(audioMasterGetTime),
    FST_UNKNOWN_ENUM(audioMasterGetVendorString),
    FST_UNKNOWN_ENUM(audioMasterGetVendorVersion),
    FST_UNKNOWN_ENUM(audioMasterIdle),
    FST_UNKNOWN_ENUM(audioMasterIOChanged),
    FST_UNKNOWN_ENUM(audioMasterNeedIdle),
    FST_UNKNOWN_ENUM(audioMasterOfflineGetCurrentMetaPass),
    FST_UNKNOWN_ENUM(audioMasterOfflineGetCurrentPass),
    FST_UNKNOWN_ENUM(audioMasterOfflineRead),
    FST_UNKNOWN_ENUM(audioMasterOfflineStart),
    FST_UNKNOWN_ENUM(audioMasterOfflineWrite),
    FST_UNKNOWN_ENUM(audioMasterOpenWindow),
    FST_UNKNOWN_ENUM(audioMasterPinConnected),
    FST_UNKNOWN_ENUM(audioMasterProcessEvents),
    FST_UNKNOWN_ENUM(audioMasterSetIcon),
    FST_UNKNOWN_ENUM(audioMasterSetOutputSampleRate),
    FST_UNKNOWN_ENUM(audioMasterSetTime),
    FST_UNKNOWN_ENUM(audioMasterSizeWindow),
    FST_UNKNOWN_ENUM(audioMasterTempoAt),
    FST_UNKNOWN_ENUM(audioMasterUpdateDisplay),
    FST_UNKNOWN_ENUM(audioMasterVendorSpecific),
    FST_UNKNOWN_ENUM(audioMasterWantMidi),
    FST_UNKNOWN_ENUM(audioMasterWillReplaceOrAccumulate)
} t_fstHostOpcode;;
typedef enum {
  FST_ENUM_EXP(effIdentify, 1),

  FST_ENUM_EXP(effSetSampleRate, 2),
  FST_ENUM_EXP(effSetBlockSize, 3),

  FST_ENUM_EXP(effOpen, 4),
    FST_UNKNOWN_ENUM(effClose),
    FST_UNKNOWN_ENUM(effSetProgram),
    FST_UNKNOWN_ENUM(effGetProgram),
    FST_UNKNOWN_ENUM(effSetProgramName),
    FST_UNKNOWN_ENUM(effGetProgramName),
    FST_UNKNOWN_ENUM(effGetParamLabel),
    FST_UNKNOWN_ENUM(effGetParamDisplay),
    FST_UNKNOWN_ENUM(effGetParamName),
    FST_UNKNOWN_ENUM(effMainsChanged),
    FST_UNKNOWN_ENUM(effEditGetRect),
    FST_UNKNOWN_ENUM(effEditOpen),
    FST_UNKNOWN_ENUM(effEditClose),

    FST_UNKNOWN_ENUM(effEditDraw),
    FST_UNKNOWN_ENUM(effEditMouse),
    FST_UNKNOWN_ENUM(effEditSleep),
    FST_UNKNOWN_ENUM(effEditTop),

    FST_UNKNOWN_ENUM(effGetChunk),
    FST_UNKNOWN_ENUM(effSetChunk),
    FST_UNKNOWN_ENUM(effProcessEvents),
    FST_UNKNOWN_ENUM(effCanBeAutomated),
    FST_UNKNOWN_ENUM(effString2Parameter),
    FST_UNKNOWN_ENUM(effGetProgramNameIndexed),
    FST_UNKNOWN_ENUM(effGetInputProperties),
    FST_UNKNOWN_ENUM(effGetOutputProperties),
    FST_UNKNOWN_ENUM(effGetPlugCategory),
    FST_UNKNOWN_ENUM(effSetSpeakerArrangement),
    FST_UNKNOWN_ENUM(effSetBypass),
    FST_UNKNOWN_ENUM(effGetEffectName),
    FST_UNKNOWN_ENUM(effGetProductString),
    FST_UNKNOWN_ENUM(effGetVendorString),
    FST_UNKNOWN_ENUM(effGetVendorVersion),
    FST_UNKNOWN_ENUM(effVendorSpecific),
    FST_UNKNOWN_ENUM(effCanDo),
    FST_UNKNOWN_ENUM(effGetTailSize),
    FST_UNKNOWN_ENUM(effKeysRequired),
    FST_UNKNOWN_ENUM(effGetVstVersion),
    FST_UNKNOWN_ENUM(effGetCurrentMidiProgram),
    FST_UNKNOWN_ENUM(effGetSpeakerArrangement),
    FST_UNKNOWN_ENUM(effSetTotalSampleToProcess),
    FST_UNKNOWN_ENUM(effSetProcessPrecision),
    FST_UNKNOWN_ENUM(effGetNumMidiInputChannels),
    FST_UNKNOWN_ENUM(effGetNumMidiOutputChannels),

    FST_UNKNOWN_ENUM(effConnectInput),
    FST_UNKNOWN_ENUM(effConnectOutput),
    FST_UNKNOWN_ENUM(effEditIdle),
    FST_UNKNOWN_ENUM(effIdle),
    FST_UNKNOWN_ENUM(effShellGetNextPlugin),
    FST_UNKNOWN_ENUM(effStartProcess),
    FST_UNKNOWN_ENUM(effStopProcess),
} t_fstPluginOpcode;
enum {
    FST_UNKNOWN_ENUM(effFlagsCanDoubleReplacing),
    FST_UNKNOWN_ENUM(effFlagsCanReplacing),
    FST_UNKNOWN_ENUM(effFlagsHasEditor),
    FST_UNKNOWN_ENUM(effFlagsIsSynth),
    FST_UNKNOWN_ENUM(effFlagsNoSoundInStop),
    FST_UNKNOWN_ENUM(effFlagsProgramChunks),
} t_fstPluginFlags;
typedef enum {
    FST_UNKNOWN_ENUM(kPlugCategAnalysis),
    FST_UNKNOWN_ENUM(kPlugCategEffect),
    FST_UNKNOWN_ENUM(kPlugCategGenerator),
    FST_UNKNOWN_ENUM(kPlugCategMastering),
    FST_UNKNOWN_ENUM(kPlugCategRestoration),
    FST_UNKNOWN_ENUM(kPlugCategRoomFx),
    FST_UNKNOWN_ENUM(kPlugCategShell),
    FST_UNKNOWN_ENUM(kPlugCategSpacializer),
    FST_UNKNOWN_ENUM(kPlugCategSynth),
    FST_UNKNOWN_ENUM(kPlugSurroundFx)
} VstPlugCategory;

typedef enum {
    FST_UNKNOWN_ENUM(kSpeakerArr102),
    FST_UNKNOWN_ENUM(kSpeakerArr30Cine),
    FST_UNKNOWN_ENUM(kSpeakerArr30Music),
    FST_UNKNOWN_ENUM(kSpeakerArr31Cine),
    FST_UNKNOWN_ENUM(kSpeakerArr31Music),
    FST_UNKNOWN_ENUM(kSpeakerArr40Cine),
    FST_UNKNOWN_ENUM(kSpeakerArr40Music),
    FST_UNKNOWN_ENUM(kSpeakerArr41Cine),
    FST_UNKNOWN_ENUM(kSpeakerArr41Music),
    FST_UNKNOWN_ENUM(kSpeakerArr50),
    FST_UNKNOWN_ENUM(kSpeakerArr51),
    FST_UNKNOWN_ENUM(kSpeakerArr60Cine),
    FST_UNKNOWN_ENUM(kSpeakerArr60Music),
    FST_UNKNOWN_ENUM(kSpeakerArr61Cine),
    FST_UNKNOWN_ENUM(kSpeakerArr61Music),
    FST_UNKNOWN_ENUM(kSpeakerArr70Cine),
    FST_UNKNOWN_ENUM(kSpeakerArr70Music),
    FST_UNKNOWN_ENUM(kSpeakerArr71Cine),
    FST_UNKNOWN_ENUM(kSpeakerArr71Music),
    FST_UNKNOWN_ENUM(kSpeakerArr80Cine),
    FST_UNKNOWN_ENUM(kSpeakerArr80Music),
    FST_UNKNOWN_ENUM(kSpeakerArr81Cine),
    FST_UNKNOWN_ENUM(kSpeakerArr81Music),
    FST_UNKNOWN_ENUM(kSpeakerArrEmpty),
    FST_UNKNOWN_ENUM(kSpeakerArrMono),
    FST_UNKNOWN_ENUM(kSpeakerArrStereo),
    FST_UNKNOWN_ENUM(kSpeakerArrStereoCenter),
    FST_UNKNOWN_ENUM(kSpeakerArrStereoCLfe),
    FST_UNKNOWN_ENUM(kSpeakerArrStereoSide),
    FST_UNKNOWN_ENUM(kSpeakerArrStereoSurround),
    FST_UNKNOWN_ENUM(kSpeakerArrUserDefined),
    FST_UNKNOWN_ENUM(kSpeakerC),
    FST_UNKNOWN_ENUM(kSpeakerL),
    FST_UNKNOWN_ENUM(kSpeakerLc),
    FST_UNKNOWN_ENUM(kSpeakerLfe),
    FST_UNKNOWN_ENUM(kSpeakerLfe2),
    FST_UNKNOWN_ENUM(kSpeakerLs),
    FST_UNKNOWN_ENUM(kSpeakerR),
    FST_UNKNOWN_ENUM(kSpeakerRc),
    FST_UNKNOWN_ENUM(kSpeakerRs),
    FST_UNKNOWN_ENUM(kSpeakerS),
    FST_UNKNOWN_ENUM(kSpeakerSl),
    FST_UNKNOWN_ENUM(kSpeakerSr),
    FST_UNKNOWN_ENUM(kSpeakerTfc),
    FST_UNKNOWN_ENUM(kSpeakerTfl),
    FST_UNKNOWN_ENUM(kSpeakerTfr),
    FST_UNKNOWN_ENUM(kSpeakerTm),
    FST_UNKNOWN_ENUM(kSpeakerTrc),
    FST_UNKNOWN_ENUM(kSpeakerTrl),
    FST_UNKNOWN_ENUM(kSpeakerTrr)
} t_fstSpeakerArrangementType;
enum {
    FST_UNKNOWN_ENUM(kVstAutomationReading),
    FST_UNKNOWN_ENUM(kVstAutomationWriting),
    FST_UNKNOWN_ENUM(kVstBarsValid),
    FST_UNKNOWN_ENUM(kVstCyclePosValid),
    FST_UNKNOWN_ENUM(kVstMaxProductStrLen),
    FST_UNKNOWN_ENUM(kVstMaxVendorStrLen),
    FST_UNKNOWN_ENUM(kVstNanosValid),
    FST_UNKNOWN_ENUM(kVstPinIsStereo),
    FST_UNKNOWN_ENUM(kVstPinUseSpeaker),
    FST_UNKNOWN_ENUM(kVstPpqPosValid),
    FST_UNKNOWN_ENUM(kVstProcessPrecision32),
    FST_UNKNOWN_ENUM(kVstProcessPrecision64),
    FST_UNKNOWN_ENUM(kVstSmpte239fps),
    FST_UNKNOWN_ENUM(kVstSmpte24fps),
    FST_UNKNOWN_ENUM(kVstSmpte25fps),
    FST_UNKNOWN_ENUM(kVstSmpte2997dfps),
    FST_UNKNOWN_ENUM(kVstSmpte2997fps),
    FST_UNKNOWN_ENUM(kVstSmpte30dfps),
    FST_UNKNOWN_ENUM(kVstSmpte30fps),
    FST_UNKNOWN_ENUM(kVstSmpte599fps),
    FST_UNKNOWN_ENUM(kVstSmpte60fps),
    FST_UNKNOWN_ENUM(kVstSmpteValid),
    FST_UNKNOWN_ENUM(kVstTempoValid),
    FST_UNKNOWN_ENUM(kVstTimeSigValid),
    FST_UNKNOWN_ENUM(kVstTransportChanged),
    FST_UNKNOWN_ENUM(kVstTransportCycleActive),
    FST_UNKNOWN_ENUM(kVstTransportPlaying),
    FST_UNKNOWN_ENUM(kVstTransportRecording)
};

typedef enum {
    FST_UNKNOWN_ENUM(kVstMidiType),
    FST_UNKNOWN_ENUM(kVstSysExType)
} t_fstEventType;


#define FSTEVENT_COMMON t_fstEventType type; int byteSize; int deltaFrames
typedef struct VstEvent_ {
 FSTEVENT_COMMON;
} VstEvent;

typedef struct VstMidiEvent_ {
 FSTEVENT_COMMON;
 /* FIXXXME: unknown member order */
 FST_UNKNOWN(int) noteLength;
 FST_UNKNOWN(int) noteOffset;
 FST_UNKNOWN(int) detune;
 FST_UNKNOWN(int) noteOffVelocity;
 char midiData[4];
} VstMidiEvent;
typedef struct VstMidiSysexEvent_ {
 FSTEVENT_COMMON;
 /* FIXXXME: unknown member order */
  char*sysexDump;
  FST_UNKNOWN(int) dumpBytes;
  FST_UNKNOWN(int) flags;
  FST_UNKNOWN(int) resvd1, resvd2;
} VstMidiSysexEvent;

typedef struct VstEvents_ {
  int numEvents;
  VstEvent**events;
} VstEvents;

typedef struct VstSpeakerProperties_ {
  FST_UNKNOWN(int) type;
} VstSpeakerProperties;

typedef struct VstSpeakerArrangement_ {
  int type;
  int numChannels;
  VstSpeakerProperties*speakers;
} VstSpeakerArrangement;

typedef struct VstTimeInfo_ {
  FST_UNKNOWN(double) tempo;
  FST_UNKNOWN(int) timeSigNumerator;
  FST_UNKNOWN(int) timeSigDenominator;
  FST_UNKNOWN(double) sampleRate;// = rate;
  FST_UNKNOWN(int) samplePos;
  FST_UNKNOWN(int) flags;// = Vst2::kVstNanosValid
  FST_UNKNOWN(double) nanoSeconds;

  FST_UNKNOWN(double) ppqPos; // (double)position.ppqPosition;
  FST_UNKNOWN(double) barStartPos; // (double)ppqPositionOfLastBarStart;
  FST_UNKNOWN(double) cycleStartPos; // (double)ppqLoopStart;
  FST_UNKNOWN(double) cycleEndPos; // (double)ppqLoopEnd;
  FST_UNKNOWN(int) smpteFrameRate; //int32
  FST_UNKNOWN(int) smpteOffset; //int32
} VstTimeInfo;

typedef struct VstPinProperties_ {
    FST_UNKNOWN(int) arrangementType;
    char*label;
    int flags;
} VstPinProperties;


 /* dispatcher(effect, opcode, index, value, ptr, opt) */
typedef t_fstPtrInt (t_fstEffectDispatcher)(struct AEffect_*, int, int, t_fstPtrInt, void* const, float);
typedef void (t_fstEffectSetParameter)(struct AEffect_*, int, float);
typedef float (t_fstEffectGetParameter)(struct AEffect_*, int);
typedef void (t_fstEffectProcess)(struct AEffect_*, float**indata, float**outdata, int sampleframes);
typedef void (t_fstEffectProcessInplace)(struct AEffect_*, float**indata, float**outdata, int sampleframes);
typedef void (t_fstEffectProcessInplaceDbl)(struct AEffect_*, double**indata, double**outdata, int sampleframes);

typedef FST_UNKNOWN(t_fstEffectDispatcher*) audioMasterCallback;

typedef struct AEffect_ {
  t_fstInt32 magic; /* @0 0x56737450, aka 'VstP' */
  char padding[4]; // always 0
  t_fstEffectDispatcher* dispatcher; // (AEffect*, Vst2::effClose, 0, 0, 0, 0);
  t_fstEffectProcess* process;
  t_fstEffectGetParameter* getParameter; // float(Aeffect*, int)
  t_fstEffectSetParameter* setParameter; // (Aeffect*, int, float)

  t_fstInt32 numPrograms;
  t_fstInt32 numParams;
  t_fstInt32 numInputs;
  t_fstInt32 numOutputs;

  FST_UNKNOWN(t_fstPtrInt) flags; //??
  FST_UNKNOWN(t_fstPtrInt) resvd1; //??
  FST_UNKNOWN(t_fstPtrInt) resvd2; //??
  FST_UNKNOWN(t_fstPtrInt) initialDelay; //??

  FST_UNKNOWN(t_fstPtrInt) pad2; //?? this holds a (float)1.0f value
  FST_UNKNOWN(void*) object; // FIXXXME
  FST_UNKNOWN(t_fstPtrInt) pad3; //??

  t_fstInt32 uniqueID; // @112
  FST_UNKNOWN(t_fstInt32) version; //??

  t_fstEffectProcessInplace* processReplacing;
  t_fstEffectProcessInplaceDbl* processDoubleReplacing;

} AEffect;

typedef struct ERect_ {
  int left;
  int right;
  int top;
  int bottom;
} ERect;
