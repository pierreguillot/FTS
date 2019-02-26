#include "fst.h"
#include "fst_utils.h"

#include <cstdio>
#include <cstring>
#include <iostream>

static AEffectDispatcherProc dispatch = 0;
static int curProgram = 0;

static float parameters[3];
static ERect editorBounds = {0, 0, 320, 240};

static char chunk[] = "This is the chunk for the FstPlugin.";

static VstEvents*s_ves;
static VstMidiSysexEvent s_sysex;
static VstMidiEvent s_midi;
static unsigned char s_sysexDump[] = {0xF0, 0x01, 0x02, 0x03, 0x04, 0x03, 0x02, 0x01, 0xF7};
static unsigned char s_midiDump[] = {0x80, 0x40, 0x0, 0};


void print_struct7(AEffect* effect) {
#if 0
  auto *str = (double*)dispatch(effect, 7, 0, 65024, 0, 0.);
  for(size_t i=0; i<96/sizeof(*str); i++)
    std::cout << " " << str[i];
  std::cout << std::endl;
#else
  auto *vti = (VstTimeInfo*)dispatch(effect, 7, 0, 65024, 0, 0.);
  print_timeinfo(vti);
#endif
}

t_fstPtrInt dispatch_v (AEffect* effect, int opcode, int index, t_fstPtrInt ivalue, void*ptr, float fvalue) {
  if(effect) {
    char opcodestr[256];
    printf("dispatch4host(%p, %s, %d, %lu, %p, %f)\n",
        effect, hostCode2string(opcode, opcodestr, 255), index, ivalue, ptr, fvalue);
    t_fstPtrInt result = dispatch(effect, opcode, index, ivalue, ptr, fvalue);
    printf("dispatch4host: %lu (0x%lX)\n", result, result);
    return result;
  }
  return 0xBEEF;
}

void test_opcodes(AEffect*effect, size_t toopcode = 100, size_t fromopcode=0) {
  printf("testing host's dispatcher\n");
  for(size_t opcode=fromopcode; opcode<toopcode; opcode++) {
    char buf[1024] = {0};
    snprintf(buf, 1023, "%s", "fudelDudelDa");
    dispatch_v(effect, opcode, 0, 0, buf, 0.f);
#if 0
    if(*buf) {
      printf("\t'%.*s'\n", 1024, buf);
    }
#endif
  }
  printf("tested hosts's dispatcher with opcodes %d..%d\n", fromopcode, toopcode);
}


static void test_opcode6266(AEffect*eff,
    t_fstInt32 opcode, int index,
    t_fstPtrInt ivalue, void* const ptr, float fvalue) {
  //print_hex(ptr, 256);
}

static void test_opcode56(AEffect*eff,
    t_fstInt32 opcode, int index,
    t_fstPtrInt ivalue, void* const ptr, float fvalue) {
  print_hex(ptr, 160);


  char*str = (char*)ptr;
  for(size_t i=0; i<8; i++) {
    *str++ = 64+i%26;
  }
}

/* effProcessEvents: handle MIDI */
static void test_opcode25(AEffect*eff,
    t_fstInt32 opcode, int index,
    t_fstPtrInt ivalue, void* const ptr, float fvalue) {
  unsigned char midi[4] = {0x90, 0x40, 0x7f, 0};
  VstEvents*vse=(VstEvents*)ptr;
  vse = create_vstevents(midi);
  vse->events[0] = (VstEvent*)0x1;  // core-dumps: 8

  dispatch_v(eff, audioMasterProcessEvents, index, ivalue, vse, fvalue);

  return;
  print_events((VstEvents*)ptr);
  return;
  char filename[128];
  print_hex(ptr, 256);
  static int count = 0;
  count++;
  sprintf(filename, "midi/dump.%d", count);
  dump_data(filename, ptr, 256);
}

typedef void (t_fun0)(void);

static void test_opcode42(AEffect*eff,
    t_fstInt32 opcode, int index,
    t_fstPtrInt ivalue, void* const ptr, float fvalue) {
  /* effGetSpeakerArrangement or effGetSpeakerArrangement */
  /* ptr and ivalue point to VstSpeakerArrangement* */
  VstSpeakerArrangement*iarr = (VstSpeakerArrangement*)ivalue;
  VstSpeakerArrangement*oarr = (VstSpeakerArrangement*)ptr;
  print_hex(iarr, 16);
  print_hex(oarr, 16);
  printf("JMZ| %d | 0x%X | %d |\n", iarr->type, iarr->type, iarr->numChannels);
  printf("JMZ| %d | 0x%X | %d |\n", oarr->type, oarr->type, oarr->numChannels);
  fflush(stdout);
  t_fun0*f=0;
#ifdef NUM_INPUTS
  /* crash */
  f();
#endif
}



#define PRINTEFFCASE(x) \
  case x:               \
  if(x>98765)                                                           \
    printf("FstClient::dispatcher(%p, %d, %d, %d, %p, %f)\n", eff, x, index, ivalue, ptr, fvalue); \
  else                                                                  \
    printf("FstClient::dispatcher(%p, %s, %d, %d, %p, %f)\n", eff, #x, index, ivalue, ptr, fvalue); \
  break;

static bool dispatcher_printEff(AEffect*eff,
    t_fstInt32 opcode, int index,
    t_fstPtrInt ivalue, void* const ptr, float fvalue) {
  switch(opcode) {
  default:
    printf("FstClient::dispatcher(%p, %d, %d, %d, %p, %f)...\n", eff, opcode, index, ivalue, ptr, fvalue);
    break;
  PRINTEFFCASE(effCanBeAutomated);
  PRINTEFFCASE(effCanDo);
  PRINTEFFCASE(effClose);
  PRINTEFFCASE(effConnectInput);
  PRINTEFFCASE(effConnectOutput);
  PRINTEFFCASE(effEditClose);
  PRINTEFFCASE(effEditDraw);
  PRINTEFFCASE(effEditGetRect);
  PRINTEFFCASE(effEditIdle);
  PRINTEFFCASE(effEditMouse);
  PRINTEFFCASE(effEditOpen);
  PRINTEFFCASE(effEditSleep);
  PRINTEFFCASE(effEditTop);
  PRINTEFFCASE(effGetChunk);
  PRINTEFFCASE(effGetCurrentMidiProgram);
  PRINTEFFCASE(effGetEffectName);
  PRINTEFFCASE(effGetInputProperties);
  PRINTEFFCASE(effGetNumMidiInputChannels);
  PRINTEFFCASE(effGetNumMidiOutputChannels);
  PRINTEFFCASE(effGetOutputProperties);
  PRINTEFFCASE(effGetParamDisplay);
  PRINTEFFCASE(effGetParamLabel);
  PRINTEFFCASE(effGetParamName);
  PRINTEFFCASE(effGetPlugCategory);
  PRINTEFFCASE(effGetProductString);
  PRINTEFFCASE(effGetProgram);
  PRINTEFFCASE(effGetProgramName);
  PRINTEFFCASE(effGetProgramNameIndexed);
  PRINTEFFCASE(effGetSpeakerArrangement);
  PRINTEFFCASE(effGetTailSize);
  PRINTEFFCASE(effGetVendorString);
  PRINTEFFCASE(effGetVendorVersion);
  PRINTEFFCASE(effGetVstVersion);
  PRINTEFFCASE(effIdentify);
  PRINTEFFCASE(effIdle);
  PRINTEFFCASE(effKeysRequired);
  PRINTEFFCASE(effMainsChanged);
  PRINTEFFCASE(effOpen);
  PRINTEFFCASE(effProcessEvents);
  PRINTEFFCASE(effSetBlockSize);
  PRINTEFFCASE(effSetBypass);
  PRINTEFFCASE(effSetChunk);
  PRINTEFFCASE(effSetProcessPrecision);
  PRINTEFFCASE(effSetProgram);
  PRINTEFFCASE(effSetProgramName);
  PRINTEFFCASE(effSetSampleRate);
  PRINTEFFCASE(effSetSpeakerArrangement);
  PRINTEFFCASE(effSetTotalSampleToProcess);
  PRINTEFFCASE(effShellGetNextPlugin);
  PRINTEFFCASE(effStartProcess);
  PRINTEFFCASE(effStopProcess);
  PRINTEFFCASE(effString2Parameter);
  PRINTEFFCASE(effVendorSpecific);
  case 53:
    /* REAPER calls this permanently */
    //printf("53...\n");
    //print_struct7(eff);
    return false;
    break;
  }
  return true;
}
static void print_ptr4opcode(t_fstInt32 opcode, void*const ptr) {
  if(!ptr)return;
  char*str = (char*)ptr;
  switch(opcode) {
  default: break;
  case effEditOpen:
    /* 'ptr' is a window-id, we cannot print it */
    return;
  case effGetParamName: case effGetParamDisplay: case effGetParamLabel:
    return;
  }
  if(str && *str)
    printf("\tFtsClient::dispatcher(ptr='%.*s')\n", 64, str);
  //if(str)print_hex(str, 96);
}
static t_fstPtrInt dispatcher(AEffect*eff, t_fstInt32 opcode, int index, t_fstPtrInt ivalue, void* const ptr, float fvalue) {
  if(!dispatcher_printEff(eff, opcode, index, ivalue, ptr, fvalue))return 0;
  print_ptr4opcode(opcode, ptr);
  switch(opcode) {
  default: break;
#if 1
  case 42:
    test_opcode42(eff, opcode, index, ivalue, ptr, fvalue);
    return 0;
#endif
#if 0
  case 56:
    test_opcode56(eff, opcode, index, ivalue, ptr, fvalue);
    return 1;
#endif
  case 25:
    test_opcode25(eff, opcode, index, ivalue, ptr, fvalue);
    return 1;
  case 66: case 62:
    test_opcode6266(eff, opcode, index, ivalue, ptr, fvalue);
    return 0;
  case effEditGetRect:
    *((ERect**)ptr) = &editorBounds;
    return (t_fstPtrInt)&editorBounds;
  case effGetChunk:
    {
      char**strptr=(char**)ptr;
      *strptr=chunk;
    }
    //printf("getChunk: %d bytes @ %p\n", sizeof(chunk), chunk);
    return sizeof(chunk);
  case effSetProgram:
    //printf("setting program to %d\n", ivalue);
    curProgram = ivalue;
    return 1;
  case effGetProgramName:
    snprintf((char*)ptr, 32, "FstProgram%d", curProgram);
    //printf("JMZ:setting program-name to %s\n", (char*)ptr);
    return 1;
  case effGetParamLabel:
    snprintf((char*)ptr, 32, "°");
    return 0;
  case effGetParamName:
    if(index>=sizeof(parameters))
      index=sizeof(parameters);
    snprintf((char*)ptr, 32, "rotation%c", index+88);
    return 0;
  case effGetParamDisplay:
    if(index>=sizeof(parameters))
      index=sizeof(parameters);
    snprintf((char*)ptr, 32, "%+03d", int((parameters[index]-0.5)*360+0.5));
    return 0;
  case effCanDo:
    do {
      if(strcmp((char*)ptr, "receiveVstEvents"))
        return 1;
      if(strcmp((char*)ptr, "receiveVstMidiEvents"))
        return 1;
      if(strcmp((char*)ptr, "sendVstEvents"))
        return 1;
      if(strcmp((char*)ptr, "sendVstMidiEvents"))
        return 1;
    } while(0);
    return 0;
  case 12:
      printf("opcode:12\n");
      dispatch_v(eff, 23, 0, 0, 0, 0.);
#if 0
      do {
        static bool first=true;
        if(first) {
          test_opcodes(eff, 50);
        } else {
          auto *str = (char*)dispatch_v(eff, 7, 0, 65024, 0, 0.);
          char filename[128];
          static int icount = 0;
          snprintf(filename, 127, "./testdump.%d", icount);
          printf("OUTFILE[%d]: %s\n", icount, filename);
          icount++;
          dump_data(filename, str, 512);
        }
        first=false;
      } while(0);
#endif
    dispatch_v(eff, 6, 0, 1, 0, 0.);
  }
  //printf("FstClient::dispatcher(%p, %d, %d, %d, %p, %f)\n", eff, opcode, index, ivalue, ptr, fvalue);
  //printf("JMZ\n");

  return 0;
}

static void setParameter(AEffect*eff, int index, float value) {
  //printf("FstClient::setParameter(%p)[%d] -> %f\n", eff, index, value);
  if(index>=sizeof(parameters))
    index=sizeof(parameters);
  parameters[index] = value;

}
static float getParameter(AEffect*eff, int index) {
  if(index>=sizeof(parameters))
    index=sizeof(parameters);
  //printf("FstClient::getParameter(%p)[%d] <- %f\n", eff, index, parameters[index]);
  return parameters[index];
}
static void process(AEffect*eff, float**indata, float**outdata, int sampleframes) {
  //printf("FstClient::process(%p, %p, %p, %d) -> %f\n", eff, indata, outdata, sampleframes, indata[0][0]);
}
static void processReplacing(AEffect*eff, float**indata, float**outdata, int sampleframes) {
  //printf("FstClient::process1(%p, %p, %p, %d) -> %f\n", eff, indata, outdata, sampleframes, indata[0][0]);
}
static void processDoubleReplacing(AEffect*eff, double**indata, double**outdata, int sampleframes) {
  //printf("FstClient::process2(%p, %p, %p, %d) -> %g\n", eff, indata, outdata, sampleframes, indata[0][0]);
}


extern "C"
AEffect*VSTPluginMain(AEffectDispatcherProc dispatch4host) {
  dispatch = dispatch4host;
  printf("FstPlugin::main(%p)\n", dispatch4host);
  for(size_t i=0; i<sizeof(parameters); i++)
    parameters[i] = 0.5;

  AEffect* eff = new AEffect;
  memset(eff, 0, sizeof(AEffect));
  eff->magic = 0x56737450;
  eff->dispatcher = dispatcher;
  eff->process = process;
  eff->getParameter = getParameter;
  eff->setParameter = setParameter;

  eff->numPrograms = 5;
  eff->numParams = 3;
#ifdef NUM_INPUTS
  eff->numInputs  = NUM_INPUTS;
  eff->numOutputs = NUM_INPUTS+1;
#else
  eff->numInputs  = 1;
  eff->numOutputs = 2;
#endif
  eff->float1 = 1.;
  eff->object = eff;
  eff->uniqueID = 0xf00d;
  eff->version = 666;

  eff->flags |= effFlagsProgramChunks;
  eff->flags |= effFlagsCanReplacing;
  //eff->flags |= (1<< 9);
  eff->flags |= effFlagsCanDoubleReplacing;

  eff->flags |= effFlagsHasEditor;

  eff->processReplacing = processReplacing;
  eff->processDoubleReplacing = processDoubleReplacing;
  print_aeffect(eff);

  for(size_t i = 0; i<64; i++) {
    char buf[512] = {0};
    t_fstPtrInt res = dispatch(0, i, 0, 0, buf, 0);
    if(*buf)
      printf("\t'%.*s'\n", 512, buf);
    if(res)
      printf("\treturned %d\n", res);
  }
  char buf[512] = {0};
  dispatch(eff, audioMasterGetProductString, 0, 0, buf, 0.f);
  printf("masterProduct: %s\n", buf);

  s_ves = (VstEvents*)calloc(1, sizeof(VstEvents)+2*sizeof(VstEvent*));
  s_ves->numEvents = 2;
  s_ves->events[0] = (VstEvent*)&s_midi;
  s_ves->events[1] = (VstEvent*)&s_sysex;

  memset(&s_midi, 0, sizeof(s_midi));
  memset(&s_sysex, 0, sizeof(s_sysex));

  s_midi.type = kVstMidiType;
  s_midi.byteSize = sizeof(s_midi);
  s_midi.deltaFrames = 1;
  for(size_t i=0; i<4; i++)
    s_midi.midiData[i] = s_midiDump[i];

  s_sysex.type = kVstSysExType;
  s_sysex.byteSize = sizeof(s_sysex);
  s_sysex.deltaFrames = 0;
  s_sysex.dumpBytes = sizeof(s_sysexDump);
  s_sysex.sysexDump = (char*)s_sysexDump;

  print_events(s_ves);


  return eff;
}
