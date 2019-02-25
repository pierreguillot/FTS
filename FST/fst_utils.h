#ifndef FST_FST_UTILS_H_
#define FST_FST_UTILS_H_

#include "fst.h"
#include <stdio.h>
#include <string>

#if __x86_64__ /* urgh, does not catch other 64bit systems */
# define FMT_PINT "llu"
# define FMT_PINTX "llX"
#else
# define FMT_PINT "u"
# define FMT_PINTX "X"
#endif



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

void dump_data(const char*basename, const void*data, size_t length) {
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

#define FST_UTILS__OPCODESTR(x)                 \
  case x:                                       \
    if(x>98765)break;                           \
    snprintf(output, length, "%s", #x);         \
    output[length-1] = 0;                       \
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
static char*hostCode2string(size_t opcode, char*output, size_t length) {
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

  printf("\n\tflags="); print_binary(eff->flags);
#define FST_UTILS__FLAG(x) if(effFlags##x)                  \
    {if(effFlags##x & eff->flags)printf("\n\t      %s", #x);}   \
  else printf("\n\t      ???%s???", #x)

  FST_UTILS__FLAG(HasEditor);
  FST_UTILS__FLAG(IsSynth);
  FST_UTILS__FLAG(CanDoubleReplacing);
  FST_UTILS__FLAG(CanReplacing);
  FST_UTILS__FLAG(NoSoundInStop);
  FST_UTILS__FLAG(ProgramChunks);

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
static void print_event(VstEvent*ev) {
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
    printf("note: length=%d\toffset=%d\tvelocity=%d\tdetune=%d\n",
      mev->noteLength,
      mev->noteOffset,
      mev->noteOffVelocity,
      mev->detune);
  printf("MIDI: %02x %02x %02x %02x\n"
      , mev->midiData[0]
      , mev->midiData[1]
      , mev->midiData[2]
      , mev->midiData[3]);
  } else if (ev->type == kVstSysExType) {
    VstMidiSysexEvent*sev = (VstMidiSysexEvent*)ev;
    printf(" [%d]\n", sizeof(VstMidiSysexEvent));

    printf("\ttype=%d\n", sev->type);
    printf("\tbyteSize=%d\n\tdeltaFrames=%d\n", sev->byteSize, sev->deltaFrames);
    printf("\tflags=%d\treserved=%llu\t%llu\n",
        sev->flags, sev->resvd1, sev->resvd2);
    printf("\tSysEx %d bytes @ %p\n\t", sev->dumpBytes, sev->sysexDump);
    unsigned char*data=(unsigned char*)sev->sysexDump;
    for(int i=0; i<sev->dumpBytes; i++)
      printf(" %02x", *data++);
    printf("\n");
  }
  print_hex(ev, 64);
}

static void print_events(VstEvents*evs) {
  printf("%d VstEvents @ %p\n", evs?evs->numEvents:0, evs);
  if(!evs)return;
  for(int i=0; i<evs->numEvents; i++) {
    print_event(evs->events[i]);
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
  FST_UTILS__VTI_d(vti, flags);
  FST_UTILS__VTI_d(vti, smpteFrameRate);
  FST_UTILS__VTI_d(vti, smpteOffset);

  FST_UTILS__VTI_d(vti, currentBar);
  FST_UTILS__VTI_x(vti, magic);
  printf("\n");
}



#endif /* FST_FST_UTILS_H_ */
