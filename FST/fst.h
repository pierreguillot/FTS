#ifndef FST_fst_h_
#define FST_fst_h_

#define FST_UNKNOWN(x) x
#define FST_ENUM(x, y) x = y
#define FST_ENUM_EXPERIMENTAL(x, y) x = y
#define FST_ENUM_UNKNOWN(x) x = 98765 + __LINE__

#define kVstVersion FST_UNKNOWN(2400)

#define VSTCALLBACK
typedef long t_fstPtrInt; /* pointer sized int */
typedef int t_fstInt32; /* 32bit int */

typedef enum {
  FST_ENUM(audioMasterAutomate, 0),
  FST_ENUM(audioMasterVersion, 1),
  FST_ENUM(audioMasterGetVendorString, 32),
  FST_ENUM(audioMasterGetProductString, 33),
  FST_ENUM(audioMasterGetVendorVersion, 34),

  // 13: sending latency??
  FST_ENUM_UNKNOWN(audioMasterBeginEdit),
  FST_ENUM_UNKNOWN(audioMasterCanDo),
  FST_ENUM_UNKNOWN(audioMasterCloseWindow),
  FST_ENUM_UNKNOWN(audioMasterCurrentId),
  FST_ENUM_UNKNOWN(audioMasterEndEdit),
  FST_ENUM_UNKNOWN(audioMasterGetAutomationState),
  FST_ENUM_UNKNOWN(audioMasterGetBlockSize),
  FST_ENUM_UNKNOWN(audioMasterGetCurrentProcessLevel),
  FST_ENUM_UNKNOWN(audioMasterGetDirectory),
  FST_ENUM_UNKNOWN(audioMasterGetInputLatency),
  FST_ENUM_UNKNOWN(audioMasterGetLanguage),
  FST_ENUM_UNKNOWN(audioMasterGetNextPlug),
  FST_ENUM_UNKNOWN(audioMasterGetNumAutomatableParameters),
  FST_ENUM_UNKNOWN(audioMasterGetOutputLatency),
  FST_ENUM_UNKNOWN(audioMasterGetOutputSpeakerArrangement),
  FST_ENUM_UNKNOWN(audioMasterGetParameterQuantization),
  FST_ENUM_UNKNOWN(audioMasterGetPreviousPlug),
  FST_ENUM_UNKNOWN(audioMasterGetSampleRate),
  FST_ENUM_UNKNOWN(audioMasterGetTime),
  FST_ENUM_UNKNOWN(audioMasterIdle),
  FST_ENUM_UNKNOWN(audioMasterIOChanged),
  FST_ENUM_UNKNOWN(audioMasterNeedIdle),
  FST_ENUM_UNKNOWN(audioMasterOfflineGetCurrentMetaPass),
  FST_ENUM_UNKNOWN(audioMasterOfflineGetCurrentPass),
  FST_ENUM_UNKNOWN(audioMasterOfflineRead),
  FST_ENUM_UNKNOWN(audioMasterOfflineStart),
  FST_ENUM_UNKNOWN(audioMasterOfflineWrite),
  FST_ENUM_UNKNOWN(audioMasterOpenWindow),
  FST_ENUM_UNKNOWN(audioMasterPinConnected),
  FST_ENUM_UNKNOWN(audioMasterProcessEvents),
  FST_ENUM_UNKNOWN(audioMasterSetIcon),
  FST_ENUM_UNKNOWN(audioMasterSetOutputSampleRate),
  FST_ENUM_UNKNOWN(audioMasterSetTime),
  FST_ENUM_UNKNOWN(audioMasterSizeWindow),
  FST_ENUM_UNKNOWN(audioMasterTempoAt),
  FST_ENUM_UNKNOWN(audioMasterUpdateDisplay),
  FST_ENUM_UNKNOWN(audioMasterVendorSpecific),
  FST_ENUM_UNKNOWN(audioMasterWantMidi),
  FST_ENUM_UNKNOWN(audioMasterWillReplaceOrAccumulate)
} t_fstHostOpcode;;
typedef enum {
  FST_ENUM_EXPERIMENTAL(effSetProgram, 2),
  FST_ENUM_EXPERIMENTAL(effGetProgramName, 5),

  FST_ENUM(effGetParamLabel, 6),
  FST_ENUM(effGetParamDisplay, 7),
  FST_ENUM(effGetParamName, 8),
  FST_ENUM(effSetBlockSize, 11),
  FST_ENUM(effSetSampleRate, 12),

  FST_ENUM_EXPERIMENTAL(effIdentify, 0),
  FST_ENUM_EXPERIMENTAL(effOpen, 1),

  FST_ENUM_UNKNOWN(effClose),
  FST_ENUM_UNKNOWN(effGetProgram),
  FST_ENUM_UNKNOWN(effSetProgramName),
  FST_ENUM_UNKNOWN(effMainsChanged),
  FST_ENUM_UNKNOWN(effEditGetRect),
  FST_ENUM_UNKNOWN(effEditOpen),
  FST_ENUM_UNKNOWN(effEditClose),

  FST_ENUM_UNKNOWN(effEditDraw),
  FST_ENUM_UNKNOWN(effEditMouse),
  FST_ENUM_UNKNOWN(effEditSleep),
  FST_ENUM_UNKNOWN(effEditTop),

  FST_ENUM_UNKNOWN(effGetChunk),
  FST_ENUM_UNKNOWN(effSetChunk),
  FST_ENUM_UNKNOWN(effProcessEvents),
  FST_ENUM_UNKNOWN(effCanBeAutomated),
  FST_ENUM_UNKNOWN(effString2Parameter),
  FST_ENUM_UNKNOWN(effGetProgramNameIndexed),
  FST_ENUM_UNKNOWN(effGetInputProperties),
  FST_ENUM_UNKNOWN(effGetOutputProperties),
  FST_ENUM_UNKNOWN(effGetPlugCategory),
  FST_ENUM_UNKNOWN(effSetSpeakerArrangement),
  FST_ENUM_UNKNOWN(effSetBypass),
  FST_ENUM_UNKNOWN(effGetEffectName),
  FST_ENUM_UNKNOWN(effGetProductString),
  FST_ENUM_UNKNOWN(effGetVendorString),
  FST_ENUM_UNKNOWN(effGetVendorVersion),
  FST_ENUM_UNKNOWN(effVendorSpecific),
  FST_ENUM_UNKNOWN(effCanDo),
  FST_ENUM_UNKNOWN(effGetTailSize),
  FST_ENUM_UNKNOWN(effKeysRequired),
  FST_ENUM_UNKNOWN(effGetVstVersion),
  FST_ENUM_UNKNOWN(effGetCurrentMidiProgram),
  FST_ENUM_UNKNOWN(effGetSpeakerArrangement),
  FST_ENUM_UNKNOWN(effSetTotalSampleToProcess),
  FST_ENUM_UNKNOWN(effSetProcessPrecision),
  FST_ENUM_UNKNOWN(effGetNumMidiInputChannels),
  FST_ENUM_UNKNOWN(effGetNumMidiOutputChannels),

  FST_ENUM_UNKNOWN(effConnectInput),
  FST_ENUM_UNKNOWN(effConnectOutput),
  FST_ENUM_UNKNOWN(effEditIdle),
  FST_ENUM_UNKNOWN(effIdle),
  FST_ENUM_UNKNOWN(effShellGetNextPlugin),
  FST_ENUM_UNKNOWN(effStartProcess),
  FST_ENUM_UNKNOWN(effStopProcess),
} t_fstPluginOpcode;
enum {
  FST_ENUM(effFlagsHasEditor, (1<<1)),
  FST_ENUM(effFlagsIsSynth, (1<<9)),
  FST_ENUM_UNKNOWN(effFlagsCanDoubleReplacing),
  FST_ENUM_UNKNOWN(effFlagsCanReplacing),
  FST_ENUM_UNKNOWN(effFlagsNoSoundInStop),
  FST_ENUM_UNKNOWN(effFlagsProgramChunks),
} t_fstPluginFlags;
typedef enum {
  FST_ENUM_UNKNOWN(kPlugCategAnalysis),
  FST_ENUM_UNKNOWN(kPlugCategEffect),
  FST_ENUM_UNKNOWN(kPlugCategGenerator),
  FST_ENUM_UNKNOWN(kPlugCategMastering),
  FST_ENUM_UNKNOWN(kPlugCategRestoration),
  FST_ENUM_UNKNOWN(kPlugCategRoomFx),
  FST_ENUM_UNKNOWN(kPlugCategShell),
  FST_ENUM_UNKNOWN(kPlugCategSpacializer),
  FST_ENUM_UNKNOWN(kPlugCategSynth),
  FST_ENUM_UNKNOWN(kPlugSurroundFx)
} VstPlugCategory;

typedef enum {
  FST_ENUM_UNKNOWN(kSpeakerArr102),
  FST_ENUM_UNKNOWN(kSpeakerArr30Cine),
  FST_ENUM_UNKNOWN(kSpeakerArr30Music),
  FST_ENUM_UNKNOWN(kSpeakerArr31Cine),
  FST_ENUM_UNKNOWN(kSpeakerArr31Music),
  FST_ENUM_UNKNOWN(kSpeakerArr40Cine),
  FST_ENUM_UNKNOWN(kSpeakerArr40Music),
  FST_ENUM_UNKNOWN(kSpeakerArr41Cine),
  FST_ENUM_UNKNOWN(kSpeakerArr41Music),
  FST_ENUM_UNKNOWN(kSpeakerArr50),
  FST_ENUM_UNKNOWN(kSpeakerArr51),
  FST_ENUM_UNKNOWN(kSpeakerArr60Cine),
  FST_ENUM_UNKNOWN(kSpeakerArr60Music),
  FST_ENUM_UNKNOWN(kSpeakerArr61Cine),
  FST_ENUM_UNKNOWN(kSpeakerArr61Music),
  FST_ENUM_UNKNOWN(kSpeakerArr70Cine),
  FST_ENUM_UNKNOWN(kSpeakerArr70Music),
  FST_ENUM_UNKNOWN(kSpeakerArr71Cine),
  FST_ENUM_UNKNOWN(kSpeakerArr71Music),
  FST_ENUM_UNKNOWN(kSpeakerArr80Cine),
  FST_ENUM_UNKNOWN(kSpeakerArr80Music),
  FST_ENUM_UNKNOWN(kSpeakerArr81Cine),
  FST_ENUM_UNKNOWN(kSpeakerArr81Music),
  FST_ENUM_UNKNOWN(kSpeakerArrEmpty),
  FST_ENUM_UNKNOWN(kSpeakerArrMono),
  FST_ENUM_UNKNOWN(kSpeakerArrStereo),
  FST_ENUM_UNKNOWN(kSpeakerArrStereoCenter),
  FST_ENUM_UNKNOWN(kSpeakerArrStereoCLfe),
  FST_ENUM_UNKNOWN(kSpeakerArrStereoSide),
  FST_ENUM_UNKNOWN(kSpeakerArrStereoSurround),
  FST_ENUM_UNKNOWN(kSpeakerArrUserDefined),
  FST_ENUM_UNKNOWN(kSpeakerC),
  FST_ENUM_UNKNOWN(kSpeakerL),
  FST_ENUM_UNKNOWN(kSpeakerLc),
  FST_ENUM_UNKNOWN(kSpeakerLfe),
  FST_ENUM_UNKNOWN(kSpeakerLfe2),
  FST_ENUM_UNKNOWN(kSpeakerLs),
  FST_ENUM_UNKNOWN(kSpeakerR),
  FST_ENUM_UNKNOWN(kSpeakerRc),
  FST_ENUM_UNKNOWN(kSpeakerRs),
  FST_ENUM_UNKNOWN(kSpeakerS),
  FST_ENUM_UNKNOWN(kSpeakerSl),
  FST_ENUM_UNKNOWN(kSpeakerSr),
  FST_ENUM_UNKNOWN(kSpeakerTfc),
  FST_ENUM_UNKNOWN(kSpeakerTfl),
  FST_ENUM_UNKNOWN(kSpeakerTfr),
  FST_ENUM_UNKNOWN(kSpeakerTm),
  FST_ENUM_UNKNOWN(kSpeakerTrc),
  FST_ENUM_UNKNOWN(kSpeakerTrl),
  FST_ENUM_UNKNOWN(kSpeakerTrr)
} t_fstSpeakerArrangementType;
enum {
/* 197782 is where the array passed at opcode:33 overflows */
  FST_ENUM_EXPERIMENTAL(kVstMaxProductStrLen, 197782),
  FST_ENUM_EXPERIMENTAL(kVstMaxVendorStrLen, 197782),
  FST_ENUM_EXPERIMENTAL(kVstMaxLabelLen, 100),
  FST_ENUM_EXPERIMENTAL(kVstMaxShortLabelLen, 100),

  FST_ENUM_UNKNOWN(kVstAutomationReading),
  FST_ENUM_UNKNOWN(kVstAutomationWriting),
  FST_ENUM_UNKNOWN(kVstBarsValid),
  FST_ENUM_UNKNOWN(kVstClockValid),
  FST_ENUM_UNKNOWN(kVstCyclePosValid),
  FST_ENUM_UNKNOWN(kVstNanosValid),
  FST_ENUM_UNKNOWN(kVstPinIsActive),
  FST_ENUM_UNKNOWN(kVstPinIsStereo),
  FST_ENUM_UNKNOWN(kVstPinUseSpeaker),
  FST_ENUM_UNKNOWN(kVstPpqPosValid),
  FST_ENUM_UNKNOWN(kVstProcessPrecision32),
  FST_ENUM_UNKNOWN(kVstProcessPrecision64),

  FST_ENUM_UNKNOWN(kVstSmpte239fps),
  FST_ENUM_UNKNOWN(kVstSmpte24fps),
  FST_ENUM_UNKNOWN(kVstSmpte249fps),
  FST_ENUM_UNKNOWN(kVstSmpte25fps),
  FST_ENUM_UNKNOWN(kVstSmpte2997dfps),
  FST_ENUM_UNKNOWN(kVstSmpte2997fps),
  FST_ENUM_UNKNOWN(kVstSmpte30dfps),
  FST_ENUM_UNKNOWN(kVstSmpte30fps),
  FST_ENUM_UNKNOWN(kVstSmpte599fps),
  FST_ENUM_UNKNOWN(kVstSmpte60fps),
  FST_ENUM_UNKNOWN(kVstSmpteFilm16mm),
  FST_ENUM_UNKNOWN(kVstSmpteFilm35mm),
  FST_ENUM_UNKNOWN(kVstSmpteValid),
  FST_ENUM_UNKNOWN(kVstTempoValid),
  FST_ENUM_UNKNOWN(kVstTimeSigValid),
  FST_ENUM_UNKNOWN(kVstTransportChanged),
  FST_ENUM_UNKNOWN(kVstTransportCycleActive),
  FST_ENUM_UNKNOWN(kVstTransportPlaying),
  FST_ENUM_UNKNOWN(kVstTransportRecording)
};

typedef enum {
  FST_ENUM_UNKNOWN(kVstMidiType),
  FST_ENUM_UNKNOWN(kVstSysExType)
} t_fstEventType;


#define FSTEVENT_COMMON t_fstEventType type; int byteSize; int deltaFrames
typedef struct VstEvent_ {
  FSTEVENT_COMMON;
} FST_UNKNOWN(VstEvent);

typedef struct VstMidiEvent_ {
  FSTEVENT_COMMON;
  /* FIXXXME: unknown member order */
  FST_UNKNOWN(int) noteLength;
  FST_UNKNOWN(int) noteOffset;
  FST_UNKNOWN(int) detune;
  FST_UNKNOWN(int) noteOffVelocity;
  char midiData[4];
} FST_UNKNOWN(VstMidiEvent);
typedef struct VstMidiSysexEvent_ {
  FSTEVENT_COMMON;
  /* FIXXXME: unknown member order */
  char*sysexDump;
  FST_UNKNOWN(int) dumpBytes;
  FST_UNKNOWN(int) flags;
  FST_UNKNOWN(int) resvd1, resvd2;
} FST_UNKNOWN(VstMidiSysexEvent);

typedef struct VstEvents_ {
  int numEvents;
  VstEvent**events;
} FST_UNKNOWN(VstEvents);

typedef struct VstSpeakerProperties_ {
  FST_UNKNOWN(int) type;
} FST_UNKNOWN(VstSpeakerProperties);

typedef struct VstSpeakerArrangement_ {
  int type;
  int numChannels;
  VstSpeakerProperties*speakers;
} FST_UNKNOWN(VstSpeakerArrangement);

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
} FST_UNKNOWN(VstTimeInfo);

typedef struct VstPinProperties_ {
  FST_UNKNOWN(int) arrangementType;
  char*label;
  char*shortLabel;
  int flags;
} FST_UNKNOWN(VstPinProperties);


/* t_fstPtrInt dispatcher(effect, opcode, index, ivalue, ptr, fvalue); */
typedef t_fstPtrInt (*AEffectDispatcherProc)(struct AEffect_*, int, int, t_fstPtrInt, void* const, float);
/* void setParameter(effect, index, fvalue); */
typedef void (*AEffectSetParameterProc)(struct AEffect_*, int, float);
/* float getParameter(effect, index); */
typedef float (*AEffectGetParameterProc)(struct AEffect_*, int);
/* void process(effect, indata, outdata, frames); */
typedef void (*AEffectProcessProc)(struct AEffect_*, float**, float**, int);
/* void processDoubleReplacing(effect, indata, outdata, frames); */
typedef void (*AEffectProcessDoubleProc)(struct AEffect_*, double**, double**, int);

typedef FST_UNKNOWN(AEffectDispatcherProc) audioMasterCallback;

typedef struct AEffect_ {
  t_fstInt32 magic; /* @0 0x56737450, aka 'VstP' */
  /* auto-padding in place */
  AEffectDispatcherProc dispatcher; // (AEffect*, Vst2::effClose, 0, 0, 0, 0);
  AEffectProcessProc process;
  AEffectSetParameterProc setParameter; // (Aeffect*, int, float)
  AEffectGetParameterProc getParameter; // float(Aeffect*, int)

  t_fstInt32 numPrograms;
  t_fstInt32 numParams;
  t_fstInt32 numInputs;
  t_fstInt32 numOutputs;

  FST_UNKNOWN(t_fstPtrInt) flags; //??
  FST_UNKNOWN(t_fstPtrInt) FST_UNKNOWN(resvd1); //??
  FST_UNKNOWN(t_fstPtrInt) FST_UNKNOWN(resvd2); //??
  FST_UNKNOWN(t_fstInt32) FST_UNKNOWN(initialDelay); //??
  char pad2[8];

  float float1;
  void* object;
  t_fstPtrInt pad3;
  t_fstInt32 uniqueID; // @112
  t_fstInt32 version;

  FST_UNKNOWN(AEffectProcessProc)  FST_UNKNOWN(processReplacing);
  FST_UNKNOWN(AEffectProcessDoubleProc)  FST_UNKNOWN(processDoubleReplacing);
} FST_UNKNOWN(AEffect);

typedef struct ERect_ {
  int left;
  int right;
  int top;
  int bottom;
} FST_UNKNOWN(ERect);


#endif /* FST_fst_h_ */
