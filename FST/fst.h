#ifndef FST_fst_h_
#define FST_fst_h_

#ifdef __GNUC__
# define FST_DEPRECATE_UNKNOWN(x) x __attribute__ ((deprecated))
#else
# define FST_DEPRECATE_UNKNOWN(x) x
#endif

#ifdef FST_DONT_DEPRECATE_UNKNOWN
# undef  FST_DEPRECATE_UNKNOWN
# define FST_DEPRECATE_UNKNOWN(x) x
#endif

#define FST_UNKNOWN(x) x
#define FST_ENUM(x, y) x = y
#define FST_ENUM_EXPERIMENTAL(x, y) FST_DEPRECATE_UNKNOWN(x) = y
#define FST_ENUM_UNKNOWN(x) FST_DEPRECATE_UNKNOWN(x) = 100000 + __LINE__

#define kVstVersion FST_UNKNOWN(2400)

#define VSTCALLBACK
typedef long t_fstPtrInt; /* pointer sized int */
typedef int t_fstInt32; /* 32bit int */

typedef enum {
  FST_ENUM(audioMasterAutomate, 0), // IN:index, IN:fvalue, return 0
  FST_ENUM(audioMasterVersion, 1), // return 2400
  FST_ENUM_EXPERIMENTAL(audioMasterCurrentId, 2), // return shellUIDToCreate
  FST_ENUM_EXPERIMENTAL(audioMasterWantMidi, 6), // return 0
  FST_ENUM(audioMasterGetTime, 7), // return (VstTimeInfo*)
  FST_ENUM(audioMasterProcessEvents, 8), //IN:ptr(VstEvents*), return 0
  FST_ENUM(audioMasterTempoAt, 10), // IN:ivalue, return (10000*BPM)
  FST_ENUM(audioMasterGetSampleRate, 16), // return sampleRate
  FST_ENUM(audioMasterGetBlockSize, 17), // return blockSize
  FST_ENUM_EXPERIMENTAL(audioMasterGetCurrentProcessLevel, 23), // return (!isRealtime)*4
  FST_ENUM(audioMasterGetVendorString, 32), //OUT:ptr(char[kVstMaxVendorStrLen]), return ptr
  FST_ENUM(audioMasterGetProductString, 33), //OUT:ptr(char[kVstMaxProductStrLen]), return ptr
  FST_ENUM(audioMasterGetVendorVersion, 34), // return 0x0101
  FST_ENUM(audioMasterCanDo, 37), //IN:ptr(char*), return *ptr in {"sendVstEvents", "sizeWindow",...}

  // 13: sending latency??

  FST_ENUM_UNKNOWN(audioMasterBeginEdit), // IN:index, return 0
  FST_ENUM_UNKNOWN(audioMasterEndEdit), //IN:index, return 0

  FST_ENUM_UNKNOWN(audioMasterCloseWindow), //?, return 0
  FST_ENUM_UNKNOWN(audioMasterOpenWindow), //?, return 0
  FST_ENUM_UNKNOWN(audioMasterSizeWindow), // IN:index(width), IN:value(height), return 1
  FST_ENUM_UNKNOWN(audioMasterSetIcon), //?, return 0
  FST_ENUM_UNKNOWN(audioMasterUpdateDisplay), // return 0

  FST_ENUM_UNKNOWN(audioMasterGetParameterQuantization), //?, return 0
  FST_ENUM_UNKNOWN(audioMasterGetNumAutomatableParameters),
  FST_ENUM_UNKNOWN(audioMasterGetAutomationState), // return {unsupported=0, off, read, write, readwrite}

  FST_ENUM_UNKNOWN(audioMasterGetInputLatency), //?, return 0
  FST_ENUM_UNKNOWN(audioMasterGetOutputLatency), //?, return 0

  FST_ENUM_UNKNOWN(audioMasterGetDirectory), // return (char*)plugindirectory
  FST_ENUM_UNKNOWN(audioMasterGetLanguage), //?, return 0

  FST_ENUM_UNKNOWN(audioMasterGetOutputSpeakerArrangement), //?, return 0

  FST_ENUM_UNKNOWN(audioMasterOfflineGetCurrentMetaPass), //?, return 0
  FST_ENUM_UNKNOWN(audioMasterOfflineGetCurrentPass), //?, return 0
  FST_ENUM_UNKNOWN(audioMasterOfflineRead), //?, return 0
  FST_ENUM_UNKNOWN(audioMasterOfflineStart), //?, return 0
  FST_ENUM_UNKNOWN(audioMasterOfflineWrite), //?, return 0

  FST_ENUM_UNKNOWN(audioMasterGetNextPlug), //?, return 0
  FST_ENUM_UNKNOWN(audioMasterGetPreviousPlug), //?, return 0

  FST_ENUM_UNKNOWN(audioMasterIdle), // return 0
  FST_ENUM_UNKNOWN(audioMasterNeedIdle), // return 0

  FST_ENUM_UNKNOWN(audioMasterIOChanged), // return 0
  FST_ENUM_UNKNOWN(audioMasterPinConnected), //IN:index, IN:ivalue(isOutput), return isValidChannel

  FST_ENUM_UNKNOWN(audioMasterSetOutputSampleRate),
  FST_ENUM_UNKNOWN(audioMasterSetTime), //?, return 0

  FST_ENUM_UNKNOWN(audioMasterWillReplaceOrAccumulate), //?, return 0

  FST_ENUM_UNKNOWN(audioMasterVendorSpecific), //?, return 0

  #warning document audioMaster*FileSelector
  FST_ENUM_UNKNOWN(audioMasterOpenFileSelector),
  FST_ENUM_UNKNOWN(audioMasterCloseFileSelector),

  FST_ENUM_UNKNOWN(fst_audioMasterLast)
} t_fstHostOpcode;;
typedef enum {
  FST_ENUM_EXPERIMENTAL(effOpen, 0), // return 0
  FST_ENUM_EXPERIMENTAL(effClose, 1), // return 0
  FST_ENUM(effSetProgram, 2), // IN:ivalue, return 0
  FST_ENUM(effGetProgram, 3), // return current_program
  FST_ENUM(effSetProgramName, 4), //IN:ptr(char*), return 0
  FST_ENUM(effGetProgramName, 5), // OUT:ptr(char[24]), return 0

  /* JUCE says that kVstMaxParamStrLen is 8, but hosts allow a bit more (24) */
  FST_ENUM(effGetParamLabel, 6), // OUT:ptr(char[8]), return 0
  FST_ENUM(effGetParamDisplay, 7), // OUT:ptr(char[8]), return 0
  FST_ENUM(effGetParamName, 8), // OUT:ptr(char[8]), return 0
  FST_ENUM(effSetSampleRate, 10), //IN:fvalue, return 0
  FST_ENUM(effSetBlockSize, 11), // IN:ivalue, return 0
  FST_ENUM_EXPERIMENTAL(effMainsChanged, 12), // IN:ivalue, return 0;  (handleResumeSuspend)

  FST_ENUM(effEditGetRect, 13), //OUT:ptr(ERect*), return ptr
  FST_ENUM(effEditOpen, 14),
  FST_ENUM(effEditClose, 15), // return 0


  FST_ENUM(effIdentify, 22), // return ByteOrder::bigEndianInt ("NvEf") 1316373862 or 1715828302
  FST_ENUM(effGetChunk, 23), // IN:index, OUT:ptr(void*), return size
  FST_ENUM(effSetChunk, 24), // IN:index, IN:ivalue(size), IN:ptr(void*), return 0

  FST_ENUM(effProcessEvents, 25), // IN:ptr(VstEvents*), return ((bool)MidiProcessed

  FST_ENUM(effGetProgramNameIndexed, 29), // IN:index, OUT:ptr(char[24], return (hasProg#)
  FST_ENUM(effGetInputProperties, 33), //IN:index, OUT:ptr(VstPinProperties*), return 1|0
  FST_ENUM(effGetOutputProperties, 34), //IN:index, OUT:ptr(VstPinProperties*), return 1|0


  FST_ENUM(effGetEffectName, 45), // OUT:ptr(char[64]), return 1
  FST_ENUM(effGetVendorString, 47), // OUT:ptr(char[64]), return 1
  FST_ENUM(effGetProductString, 48), // OUT:ptr(char[64]), return 1
  FST_ENUM(effGetVendorVersion, 49), // return version

  FST_ENUM(effCanDo, 51), //IN:ptr(char*), returns 0|1|-1
  FST_ENUM(effGetVstVersion, 58), // return kVstVersion
  FST_ENUM_EXPERIMENTAL(effKeysRequired, 59), // return ((bool)KeyboardFocusRequireq

  FST_ENUM_UNKNOWN(effEditDraw),
  FST_ENUM_UNKNOWN(effEditMouse),
  FST_ENUM_UNKNOWN(effEditSleep),
  FST_ENUM_UNKNOWN(effEditTop),
  FST_ENUM_UNKNOWN(effEditIdle),

  FST_ENUM_UNKNOWN(effCanBeAutomated), // (can parameter# be automated) IN:index, return 0
  FST_ENUM_UNKNOWN(effString2Parameter), // IN:index, IN:ptr(char*), return (hasParam#)

  FST_ENUM_UNKNOWN(effGetPlugCategory), // return category

  FST_ENUM_UNKNOWN(effSetSpeakerArrangement),
  FST_ENUM_UNKNOWN(effGetSpeakerArrangement), // OUT:ivalue(VstSpeakerArrangement*in) OUT:ptr(VstSpeakerArrangement*out), return (!(hasAUX || isMidi))

  FST_ENUM_UNKNOWN(effGetCurrentMidiProgram), // return -1
  FST_ENUM_UNKNOWN(effGetNumMidiInputChannels), // return 16*isMidi
  FST_ENUM_UNKNOWN(effGetNumMidiOutputChannels), // return 16*isMidi

  FST_ENUM_UNKNOWN(effSetBypass), //IN:ivalue, return 0
  FST_ENUM_UNKNOWN(effGetTailSize), // return audiotailInSamples
  FST_ENUM_UNKNOWN(effSetProcessPrecision), //IN:ivalue(kVstProcessPrecision64,..), return !isProcessing
  FST_ENUM_UNKNOWN(effSetTotalSampleToProcess), // return ivalue
  FST_ENUM_UNKNOWN(effStartProcess),
  FST_ENUM_UNKNOWN(effStopProcess),

  FST_ENUM_UNKNOWN(effConnectInput),
  FST_ENUM_UNKNOWN(effConnectOutput),

  FST_ENUM_UNKNOWN(effIdle),
  FST_ENUM_UNKNOWN(effShellGetNextPlugin),

#warning document origin of eff*SetProgram
  FST_ENUM_UNKNOWN(effBeginSetProgram),
  FST_ENUM_UNKNOWN(effEndSetProgram),

  FST_ENUM_UNKNOWN(effVendorSpecific) //
} t_fstPluginOpcode;
#warning document VstAEffectFlags
typedef enum {
  FST_ENUM(effFlagsHasEditor,          (1<<0)),
  FST_ENUM(effFlagsCanReplacing,       (1<<4)),
  FST_ENUM(effFlagsProgramChunks,      (1<<5)),
  FST_ENUM(effFlagsIsSynth,            (1<<8)),
  FST_ENUM(effFlagsNoSoundInStop,      (1<<9)),
  FST_ENUM(effFlagsCanDoubleReplacing, (1<<12)),
} VstAEffectFlags;
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
enum { /* VstTimeInfo.flags */
  FST_ENUM(kVstTransportChanged,     (1<<0)),
  FST_ENUM(kVstTransportPlaying,     (1<<1)),
  FST_ENUM(kVstTransportCycleActive, (1<<2)),
  FST_ENUM(kVstTransportRecording,   (1<<3)),

  FST_ENUM(kVstNanosValid   , (1<< 8)),
  FST_ENUM(kVstPpqPosValid  , (1<< 9)),
  FST_ENUM(kVstTempoValid   , (1<<10)),
  FST_ENUM(kVstBarsValid    , (1<<11)),
  FST_ENUM(kVstCyclePosValid, (1<<12)),
  FST_ENUM(kVstTimeSigValid , (1<<13)),
  FST_ENUM(kVstSmpteValid   , (1<<14)),
  FST_ENUM(kVstClockValid   , (1<<15))
};
enum {
/* 197782 is where the array passed at opcode:33 overflows */
  FST_ENUM_EXPERIMENTAL(kVstMaxProductStrLen, 197782),
  FST_ENUM_EXPERIMENTAL(kVstMaxVendorStrLen, 197782),
  FST_ENUM_EXPERIMENTAL(kVstMaxLabelLen, 64),
  FST_ENUM_EXPERIMENTAL(kVstMaxShortLabelLen, 8),

  FST_ENUM_UNKNOWN(kVstAutomationReading),
  FST_ENUM_UNKNOWN(kVstAutomationWriting),

  FST_ENUM_UNKNOWN(kVstPinIsActive),
  FST_ENUM_UNKNOWN(kVstPinIsStereo),
  FST_ENUM_UNKNOWN(kVstPinUseSpeaker),

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
};

typedef enum {
  FST_ENUM_EXPERIMENTAL(kVstMidiType, 1),
  FST_ENUM_EXPERIMENTAL(kVstSysExType, 6)
} t_fstEventType;


/* deltaFrames: used by JUCE as "timestamp" (to sort events) */
#define FSTEVENT_COMMON t_fstEventType type; int byteSize; int deltaFrames
typedef struct VstEvent_ {
  FSTEVENT_COMMON;
} VstEvent;

typedef struct VstMidiEvent_ {
  FSTEVENT_COMMON; // @0x00-0x0b
  /* FIXXXME: unknown member order and size */
  /* REAPER: sets everything to 0
   * JMZ: size is set to occupy 12bytes (on amd64) for now
   */
  FST_UNKNOWN(short) noteLength;
  FST_UNKNOWN(short) noteOffset;
  FST_UNKNOWN(int) detune;
  FST_UNKNOWN(int) noteOffVelocity;
#if 0
  /*
   * REAPER/JUCE suggest a varsized array, although it must always hold 4 bytes
   * REAPER: byteSize=24
   * JUCE: byteSize=sizeof(VstMidiEvent)
   * JUCE: if(numBytes<=4)memcpy(midiData,...,numBytes)
   */
  unsigned char midiData[];
#else
  unsigned char midiData[4]; // @0x18
#endif
} FST_UNKNOWN(VstMidiEvent);
typedef struct VstMidiSysexEvent_ {
  FSTEVENT_COMMON;
  /* FIXXXME: unknown member order */
  int pad;
  int dumpBytes; // size of sysexDump
  FST_UNKNOWN(int) flags; //?
  FST_UNKNOWN(t_fstPtrInt) resvd1; //?
  char*sysexDump; // heap allocated memory for sysex-data
  FST_UNKNOWN(t_fstPtrInt) resvd2; //?
} FST_UNKNOWN(VstMidiSysexEvent);

typedef struct VstEvents_ {
  int numEvents;
  FST_UNKNOWN(t_fstPtrInt pad);
  VstEvent*events[];
} FST_UNKNOWN(VstEvents);

typedef struct VstSpeakerProperties_ {
  FST_UNKNOWN(int) type;
} FST_UNKNOWN(VstSpeakerProperties);

typedef struct VstSpeakerArrangement_ {
  int type;
  int numChannels;
  VstSpeakerProperties speakers[];
} FST_UNKNOWN(VstSpeakerArrangement);

typedef struct VstTimeInfo_ {
  double samplePos; //OK
  double sampleRate;// = rate; //OK
  double nanoSeconds; //OK
  /* ppq: Pulses Per Quaternote */
  double ppqPos; // (double)position.ppqPosition; //OK
  double tempo; //OK
  double barStartPos; // (double)ppqPositionOfLastBarStart; //OK
  double cycleStartPos; // (double)ppqLoopStart; //OK
  double cycleEndPos; // (double)ppqLoopEnd; //OK
  int timeSigNumerator; //OK
  int timeSigDenominator; //OK

  int FST_UNKNOWN(currentBar), FST_UNKNOWN(magic); // we just made these fields up, as their values seem to be neither flags nor smtp*

#warning document origin samplesToNextClock
  /* this used to be 'pad' */
  FST_UNKNOWN(int) samplesToNextClock;//?

  FST_UNKNOWN(int) flags;// = Vst2::kVstNanosValid //?
  FST_UNKNOWN(int) smpteFrameRate; //int32 //?
  FST_UNKNOWN(int) smpteOffset; //int32 //?
} FST_UNKNOWN(VstTimeInfo);

typedef struct VstPinProperties_ {
  char label[64];
  FST_UNKNOWN(int) flags; //?
  FST_UNKNOWN(int) arrangementType; //?
  char shortLabel[8];
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
  FST_UNKNOWN(t_fstInt32) FST_UNKNOWN(initialDelay); //??; latency in samples
  char pad2[8];

  float float1;
  void* object;
  #warning document AEffect.user
  void*user;
  t_fstInt32 uniqueID; // @112
  t_fstInt32 version;

  AEffectProcessProc processReplacing;
  AEffectProcessDoubleProc processDoubleReplacing;
} FST_UNKNOWN(AEffect);

typedef struct ERect_ {
  short top;
  short left;
  short bottom;
  short right;
} ERect;

#warning TODO: documentation
typedef t_fstPtrInt VstIntPtr;
typedef t_fstInt32 VstInt32;

const int kEffectMagic=0x56737450;

#define CCONST(a,b,c,d) ((((unsigned char)a)<<24) + (((unsigned char)b)<<16) + (((unsigned char)c)<<8) + ((unsigned char)d))

#endif /* FST_fst_h_ */
