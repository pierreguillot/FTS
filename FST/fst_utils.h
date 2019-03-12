#ifndef FST_FST_UTILS_H_
#define FST_FST_UTILS_H_

#include "fst.h"

#ifdef _WIN32
# include <windows.h>
#else
# include <dlfcn.h>
#endif

#include <stdio.h>
#include <string>

#include <unistd.h>
void fstpause(float duration=1.0) {
  usleep(duration * 1000000);
}

static void print_hex(void*ptr, size_t length) {
  printf("DATA@%p [%d]", ptr, length);
  unsigned char* data = (unsigned char*)ptr;
  if(data) {
    for(size_t i=0; i<length; i++) {
      if(!(i%16))printf("\n\t");
      if(!(i% 8))printf(" ");
      printf(" %02X", *data++);
    }
  }
  printf("\n");
}

static void dump_data(const char*basename, const void*data, size_t length) {
  const char*ptr = (const char*)data;
  std::string filename = std::string(basename);
  filename+=".bin";
  FILE*f = fopen(filename.c_str(), "w");
  for(size_t i=0; i<length; i++) {
    fprintf(f, "%c", *ptr++);
  }
  fclose(f);
}

template <class inttype>
static void print_binary(inttype data, const char*suffix="") {
  size_t bits = sizeof(data)*8;
  while(bits--)
    printf("%d", (data>>bits)&0x1);
  printf("%s", suffix);
}

#include <iostream>
template <class type>
static void print_numbers(type*data, size_t length) {
  while(length--) {
    std::cout << " " << *data++;
    if(!(length % 16))  std::cout << std::endl;
  }
  std::cout << std::endl;
}


#define FST_UTILS__OPCODESTR(x)                   \
  case x:                                         \
  if(x>100000)                                    \
    snprintf(output, length, "%d[%s?]", x, #x);   \
  else                                            \
    snprintf(output, length, "%s[%d]", #x, x);    \
  output[length-1] = 0;                           \
  return output

static char*effCode2string(size_t opcode, char*output, size_t length) {
  switch(opcode) {
    FST_UTILS__OPCODESTR(effCanBeAutomated);
    FST_UTILS__OPCODESTR(effCanDo);
    FST_UTILS__OPCODESTR(effClose);
    FST_UTILS__OPCODESTR(effConnectInput);
    FST_UTILS__OPCODESTR(effConnectOutput);
    FST_UTILS__OPCODESTR(effEditClose);
    FST_UTILS__OPCODESTR(effEditDraw);
    FST_UTILS__OPCODESTR(effEditGetRect);
    FST_UTILS__OPCODESTR(effEditIdle);
    FST_UTILS__OPCODESTR(effEditMouse);
    FST_UTILS__OPCODESTR(effEditOpen);
    FST_UTILS__OPCODESTR(effEditSleep);
    FST_UTILS__OPCODESTR(effEditTop);
    FST_UTILS__OPCODESTR(effGetChunk);
    FST_UTILS__OPCODESTR(effGetCurrentMidiProgram);
    FST_UTILS__OPCODESTR(effGetEffectName);
    FST_UTILS__OPCODESTR(effGetInputProperties);
    FST_UTILS__OPCODESTR(effGetNumMidiInputChannels);
    FST_UTILS__OPCODESTR(effGetNumMidiOutputChannels);
    FST_UTILS__OPCODESTR(effGetOutputProperties);
    FST_UTILS__OPCODESTR(effGetParamDisplay);
    FST_UTILS__OPCODESTR(effGetParamLabel);
    FST_UTILS__OPCODESTR(effGetParamName);
    FST_UTILS__OPCODESTR(effGetPlugCategory);
    FST_UTILS__OPCODESTR(effGetProductString);
    FST_UTILS__OPCODESTR(effGetProgram);
    FST_UTILS__OPCODESTR(effGetProgramName);
    FST_UTILS__OPCODESTR(effGetProgramNameIndexed);
    FST_UTILS__OPCODESTR(effGetSpeakerArrangement);
    FST_UTILS__OPCODESTR(effGetTailSize);
    FST_UTILS__OPCODESTR(effGetVendorString);
    FST_UTILS__OPCODESTR(effGetVendorVersion);
    FST_UTILS__OPCODESTR(effGetVstVersion);
    FST_UTILS__OPCODESTR(effIdentify);
    FST_UTILS__OPCODESTR(effIdle);
    FST_UTILS__OPCODESTR(effKeysRequired);
    FST_UTILS__OPCODESTR(effMainsChanged);
    FST_UTILS__OPCODESTR(effOpen);
    FST_UTILS__OPCODESTR(effProcessEvents);
    FST_UTILS__OPCODESTR(effSetBlockSize);
    FST_UTILS__OPCODESTR(effSetBypass);
    FST_UTILS__OPCODESTR(effSetChunk);
    FST_UTILS__OPCODESTR(effSetProcessPrecision);
    FST_UTILS__OPCODESTR(effSetProgram);
    FST_UTILS__OPCODESTR(effSetProgramName);
    FST_UTILS__OPCODESTR(effSetSampleRate);
    FST_UTILS__OPCODESTR(effSetSpeakerArrangement);
    FST_UTILS__OPCODESTR(effSetTotalSampleToProcess);
    FST_UTILS__OPCODESTR(effShellGetNextPlugin);
    FST_UTILS__OPCODESTR(effStartProcess);
    FST_UTILS__OPCODESTR(effStopProcess);
    FST_UTILS__OPCODESTR(effString2Parameter);
    FST_UTILS__OPCODESTR(effVendorSpecific);
  default: break;
  }
  snprintf(output, length, "%d", opcode);
  return output;
}
static char*hostCode2string(t_fstPtrInt opcode, char*output, size_t length) {
  switch(opcode) {
    FST_UTILS__OPCODESTR(audioMasterAutomate);
    FST_UTILS__OPCODESTR(audioMasterVersion);
    FST_UTILS__OPCODESTR(audioMasterGetVendorString);
    FST_UTILS__OPCODESTR(audioMasterGetProductString);
    FST_UTILS__OPCODESTR(audioMasterGetVendorVersion);
    FST_UTILS__OPCODESTR(audioMasterBeginEdit);
    FST_UTILS__OPCODESTR(audioMasterEndEdit);
    FST_UTILS__OPCODESTR(audioMasterCanDo);
    FST_UTILS__OPCODESTR(audioMasterCloseWindow);
    FST_UTILS__OPCODESTR(audioMasterCurrentId);
    FST_UTILS__OPCODESTR(audioMasterGetAutomationState);
    FST_UTILS__OPCODESTR(audioMasterGetBlockSize);
    FST_UTILS__OPCODESTR(audioMasterGetCurrentProcessLevel);
    FST_UTILS__OPCODESTR(audioMasterGetDirectory);
    FST_UTILS__OPCODESTR(audioMasterGetInputLatency);
    FST_UTILS__OPCODESTR(audioMasterGetLanguage);
    FST_UTILS__OPCODESTR(audioMasterGetNextPlug);
    FST_UTILS__OPCODESTR(audioMasterGetNumAutomatableParameters);
    FST_UTILS__OPCODESTR(audioMasterGetOutputLatency);
    FST_UTILS__OPCODESTR(audioMasterGetOutputSpeakerArrangement);
    FST_UTILS__OPCODESTR(audioMasterGetParameterQuantization);
    FST_UTILS__OPCODESTR(audioMasterGetPreviousPlug);
    FST_UTILS__OPCODESTR(audioMasterGetSampleRate);
    FST_UTILS__OPCODESTR(audioMasterGetTime);
    FST_UTILS__OPCODESTR(audioMasterIdle);
    FST_UTILS__OPCODESTR(audioMasterIOChanged);
    FST_UTILS__OPCODESTR(audioMasterNeedIdle);
    FST_UTILS__OPCODESTR(audioMasterOfflineGetCurrentMetaPass);
    FST_UTILS__OPCODESTR(audioMasterOfflineGetCurrentPass);
    FST_UTILS__OPCODESTR(audioMasterOfflineRead);
    FST_UTILS__OPCODESTR(audioMasterOfflineStart);
    FST_UTILS__OPCODESTR(audioMasterOfflineWrite);
    FST_UTILS__OPCODESTR(audioMasterOpenWindow);
    FST_UTILS__OPCODESTR(audioMasterPinConnected);
    FST_UTILS__OPCODESTR(audioMasterProcessEvents);
    FST_UTILS__OPCODESTR(audioMasterSetIcon);
    FST_UTILS__OPCODESTR(audioMasterSetOutputSampleRate);
    FST_UTILS__OPCODESTR(audioMasterSetTime);
    FST_UTILS__OPCODESTR(audioMasterSizeWindow);
    FST_UTILS__OPCODESTR(audioMasterTempoAt);
    FST_UTILS__OPCODESTR(audioMasterUpdateDisplay);
    FST_UTILS__OPCODESTR(audioMasterVendorSpecific);
    FST_UTILS__OPCODESTR(audioMasterWantMidi);
    FST_UTILS__OPCODESTR(audioMasterWillReplaceOrAccumulate);
  default: break;
  }
  snprintf(output, length, "%d", opcode);
  return output;
}

static int effKnown(t_fstPtrInt opcode) {
  if(opcode>=100000)
    return 0;
  switch(opcode) {
    case effCanBeAutomated:
    case effCanDo:
    case effClose:
    case effConnectInput:
    case effConnectOutput:
    case effEditClose:
    case effEditDraw:
    case effEditGetRect:
    case effEditIdle:
    case effEditMouse:
    case effEditOpen:
    case effEditSleep:
    case effEditTop:
    case effGetChunk:
    case effGetCurrentMidiProgram:
    case effGetEffectName:
    case effGetInputProperties:
    case effGetNumMidiInputChannels:
    case effGetNumMidiOutputChannels:
    case effGetOutputProperties:
    case effGetParamDisplay:
    case effGetParamLabel:
    case effGetParamName:
    case effGetPlugCategory:
    case effGetProductString:
    case effGetProgram:
    case effGetProgramName:
    case effGetProgramNameIndexed:
    case effGetSpeakerArrangement:
    case effGetTailSize:
    case effGetVendorString:
    case effGetVendorVersion:
    case effGetVstVersion:
    case effIdentify:
    case effIdle:
    case effKeysRequired:
    case effMainsChanged:
    case effOpen:
    case effProcessEvents:
    case effSetBlockSize:
    case effSetBypass:
    case effSetChunk:
    case effSetProcessPrecision:
    case effSetProgram:
    case effSetProgramName:
    case effSetSampleRate:
    case effSetSpeakerArrangement:
    case effSetTotalSampleToProcess:
    case effShellGetNextPlugin:
    case effStartProcess:
    case effStopProcess:
    case effString2Parameter:
    case effVendorSpecific:
      return 1;
  default: break;
  }
  return 0;
}

static int hostKnown(t_fstPtrInt opcode) {
  if(opcode>=100000)
    return 0;
  switch(opcode) {
    case audioMasterAutomate:
    case audioMasterVersion:
    case audioMasterCurrentId:
    case audioMasterWantMidi:
    case audioMasterGetTime:
    case audioMasterProcessEvents:
    case audioMasterTempoAt:
    case audioMasterGetSampleRate:
    case audioMasterSizeWindow:
    case audioMasterGetBlockSize:
    case audioMasterGetCurrentProcessLevel:
    case audioMasterGetVendorString:
    case audioMasterGetProductString:
    case audioMasterGetVendorVersion:
    case audioMasterCanDo:
    case audioMasterBeginEdit:
    case audioMasterEndEdit:
    case audioMasterCloseWindow:
    case audioMasterOpenWindow:
    case audioMasterSetIcon:
    case audioMasterUpdateDisplay:
    case audioMasterGetParameterQuantization:
    case audioMasterGetNumAutomatableParameters:
    case audioMasterGetAutomationState:
    case audioMasterGetInputLatency:
    case audioMasterGetOutputLatency:
    case audioMasterGetDirectory:
    case audioMasterGetLanguage:
    case audioMasterGetOutputSpeakerArrangement:
    case audioMasterOfflineGetCurrentMetaPass:
    case audioMasterOfflineGetCurrentPass:
    case audioMasterOfflineRead:
    case audioMasterOfflineStart:
    case audioMasterOfflineWrite:
    case audioMasterGetNextPlug:
    case audioMasterGetPreviousPlug:
    case audioMasterIdle:
    case audioMasterNeedIdle:
    case audioMasterIOChanged:
    case audioMasterPinConnected:
    case audioMasterSetOutputSampleRate:
    case audioMasterSetTime:
    case audioMasterWillReplaceOrAccumulate:
    case audioMasterVendorSpecific:
      return 1;
  default: break;
  }
  return 0;
}

static void print_aeffect(AEffect*eff) {
  printf("AEffect @ %p", eff);
  if(!eff)return;
  if(eff->magic != 0x56737450)
    printf("\n\tmagic=0x%X", eff->magic);
  else
    printf("\n\tmagic=VstP");
  printf("\n\tdispatcherCB=%p", eff->dispatcher);
  printf("\n\tprocessCB=%p", eff->process);
  printf("\n\tgetParameterCB=%p", eff->getParameter);
  printf("\n\tsetParameterCB=%p", eff->setParameter);

  printf("\n\tnumPrograms=%d", eff->numPrograms);
  printf("\n\tnumParams=%d", eff->numParams);
  printf("\n\tnumInputs=%d", eff->numInputs);
  printf("\n\tnumOutputs=%d", eff->numOutputs);

  int flags = eff->flags;
  printf("\n\tflags="); print_binary(flags);
#define FST_UTILS__FLAG(x) if(effFlags##x) {                      \
    if(effFlags##x & flags)printf("\n\t      %s", #x);            \
    flags &= ~effFlags##x;                                        \
  }                                                               \
  else printf("\n\t      ???%s???", #x)

  FST_UTILS__FLAG(HasEditor);
  FST_UTILS__FLAG(IsSynth);
  FST_UTILS__FLAG(CanDoubleReplacing);
  FST_UTILS__FLAG(CanReplacing);
  FST_UTILS__FLAG(NoSoundInStop);
  FST_UTILS__FLAG(ProgramChunks);
  if(flags) {
    printf("\n\t      ");
    print_binary(flags);
  }

  printf("\n\tresvd1=0x%X", eff->resvd1);
  printf("\n\tresvd2=0x%X", eff->resvd2);
  printf("\n\tinitialDelay=%d", eff->initialDelay);

  printf("\n\tobject=%p", eff->object);
  printf("\n\tuniqueID=%d", eff->uniqueID);
  printf("\n\tversion=%d", eff->version);

  printf("\n\tprocessReplacingCB=%p", eff->processReplacing);
  printf("\n\tprocessDoubleReplacingCB=%p", eff->processDoubleReplacing);
  printf("\n\n");
}
static void print_event(VstEvent*ev, int hexdump) {
  printf("VstEvent @ %p", ev);
  if(!ev) {
    printf(" [%d]\n", sizeof(VstEvent));
    return;
  }
  if(ev->type == kVstMidiType) {
    VstMidiEvent*mev = (VstMidiEvent*)ev;
    printf(" [%d]\n", sizeof(VstMidiEvent));

    printf("\ttype=%d\n", mev->type);
    printf("\tbyteSize=%d\n\tdeltaFrames=%d\n", mev->byteSize, mev->deltaFrames);
    printf("\tMIDI: %02x %02x %02x %02x\n"
           , mev->midiData[0]
           , mev->midiData[1]
           , mev->midiData[2]
           , mev->midiData[3]);
    printf("\tnote: length=%d\toffset=%d\tvelocity=%d\tdetune=%d\n",
      mev->noteLength,
      mev->noteOffset,
      mev->noteOffVelocity,
      mev->detune);
  } else if (ev->type == kVstSysExType) {
    VstMidiSysexEvent*sev = (VstMidiSysexEvent*)ev;
    printf(" [%d]\n", sizeof(VstMidiSysexEvent));

    printf("\ttype=%d\n", sev->type);
    printf("\tbyteSize=%d\n\tdeltaFrames=%d\n", sev->byteSize, sev->deltaFrames);
    printf("\tSysEx %d bytes @ %p\n\t", sev->dumpBytes, sev->sysexDump);
    unsigned char*data=(unsigned char*)sev->sysexDump;
    for(int i=0; i<sev->dumpBytes; i++)
      printf(" %02x", *data++);
    printf("\n");
    printf("\tflags=%d\treserved=%lu\t%lu\n",
        sev->flags, sev->resvd1, sev->resvd2);
  }
  if(hexdump)
    print_hex(ev, 64);
}

static void print_events(VstEvents*evs, int hexdump=0) {
  printf("%d VstEvents @ %p\n", evs?evs->numEvents:0, evs);
  if(!evs)return;
  for(int i=0; i<evs->numEvents; i++) {
    print_event(evs->events[i], hexdump);
  }
}

static void print_erect(ERect*rect) {
  printf("ERect[%p]", rect);
  if(rect)
    printf(" = %d|%d - %d|%d", rect->top, rect->left, rect->bottom, rect->right);
  printf("\n");
}
static void print_pinproperties(VstPinProperties*vpp) {
  printf("VstPinProperties @ %p", vpp);
  if(!vpp) {
    printf("\n");
    return;
  }
  printf("\nlabel     : '%.*s'", 64, vpp->label);
  printf("\nlshortabel: '%.*s'",  8, vpp->shortLabel);
  printf("\narrangtype: %d", vpp->arrangementType);
  printf("\nflags     : %d", vpp->flags);
  printf("\n");
}

static void print_timeinfo(VstTimeInfo*vti) {
  printf("VstTimeInfo @ %p", vti);
  if(!vti) {
    printf("\n");
    return;
  }
#define FST_UTILS__VTI_g(ti, x) printf("\n\t%s: %g", #x, ti->x)
#define FST_UTILS__VTI_d(ti, x) printf("\n\t%s: %d", #x, ti->x)
#define FST_UTILS__VTI_x(ti, x) printf("\n\t%s: 0x%X", #x, ti->x)
  FST_UTILS__VTI_g(vti, samplePos);
  FST_UTILS__VTI_g(vti, sampleRate);
  FST_UTILS__VTI_g(vti, nanoSeconds);
  printf("\t= %gsec", (vti->nanoSeconds * 0.000000001));
  FST_UTILS__VTI_g(vti, ppqPos);
  FST_UTILS__VTI_g(vti, tempo);
  FST_UTILS__VTI_g(vti, barStartPos);
  FST_UTILS__VTI_g(vti, cycleStartPos);
  FST_UTILS__VTI_g(vti, cycleEndPos);
  FST_UTILS__VTI_d(vti, timeSigNumerator);
  FST_UTILS__VTI_d(vti, timeSigDenominator);
  FST_UTILS__VTI_x(vti, flags);
  int flags = vti->flags;
#define FST_UTILS__VTI_FLAGS(ti, f) do {flags &= ~f; if(ti->flags & f)printf("\n\t\t%s", #f);} while (0)
  FST_UTILS__VTI_FLAGS(vti, kVstTransportChanged);
  FST_UTILS__VTI_FLAGS(vti, kVstTransportPlaying);
  FST_UTILS__VTI_FLAGS(vti, kVstTransportCycleActive);
  FST_UTILS__VTI_FLAGS(vti, kVstTransportRecording);
  FST_UTILS__VTI_FLAGS(vti, kVstNanosValid);
  FST_UTILS__VTI_FLAGS(vti, kVstPpqPosValid);
  FST_UTILS__VTI_FLAGS(vti, kVstTempoValid);
  FST_UTILS__VTI_FLAGS(vti, kVstBarsValid);
  FST_UTILS__VTI_FLAGS(vti, kVstCyclePosValid);
  FST_UTILS__VTI_FLAGS(vti, kVstTimeSigValid);
  FST_UTILS__VTI_FLAGS(vti, kVstSmpteValid);
  FST_UTILS__VTI_FLAGS(vti, kVstClockValid);
  if(flags)printf("\n\t\tremainder: 0x%04X", flags);

  FST_UTILS__VTI_d(vti, smpteFrameRate);
  FST_UTILS__VTI_d(vti, smpteOffset);

  FST_UTILS__VTI_d(vti, currentBar);
  FST_UTILS__VTI_x(vti, magic);
  printf("\n");
}

static VstEvents*create_vstevents(const unsigned char midi[4]) {
  VstEvents*ves = (VstEvents*)calloc(1, sizeof(VstEvents)+sizeof(VstEvent*));
  VstMidiEvent*ve=(VstMidiEvent*)calloc(1, sizeof(VstMidiEvent));
  ves->numEvents = 1;
  ves->events[0]=(VstEvent*)ve;
  ve->type = kVstMidiType;
  ve->byteSize = sizeof(VstMidiEvent);
  for(size_t i=0; i<4; i++)
    ve->midiData[i] = midi[i];

  return ves;
}


typedef AEffect* (t_fstMain)(AEffectDispatcherProc);
static
t_fstMain* fstLoadPlugin(const char* filename) {
  t_fstMain*vstfun = 0;
#ifdef _WIN32
  HINSTANCE handle = LoadLibrary(filename);
  printf("loading %s as %p\n", filename, handle);
  if(!handle){printf("\tfailed!\n"); return 0; }
  if(!vstfun)vstfun=(t_fstMain*)GetProcAddress(handle, "VSTPluginMain");
  if(!vstfun)vstfun=(t_fstMain*)GetProcAddress(handle, "main");
  if(!vstfun)FreeLibrary(handle);
#else
  void*handle = dlopen(filename, RTLD_NOW | RTLD_GLOBAL);
  printf("loading %s as %p\n", filename, handle);
  if(!handle){printf("\t%s\n", dlerror()); return 0; }
  if(!vstfun)vstfun=(t_fstMain*)dlsym(handle, "VSTPluginMain");
  if(!vstfun)vstfun=(t_fstMain*)dlsym(handle, "main");
  if(!vstfun)dlclose(handle);
#endif
  printf("loaded '%s' @ %p: %p\n", filename, handle, vstfun);
  fstpause(1.);
  return vstfun;
}

#endif /* FST_FST_UTILS_H_ */
