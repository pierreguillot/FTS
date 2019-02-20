#include "fst.h"
#include "../print_aeffect.h"

#include <cstdio>
#include <cstring>

static AEffectDispatcherProc dispatch = 0;
static int curProgram = 0;

static float parameters[3];


static char chunk[] = "This is the chunk for the FstPlugin.";


t_fstPtrInt dispatch_v (AEffect* effect, int opcode, int index, t_fstPtrInt ivalue, void*ptr, float fvalue) {
  if(effect) {
    char opcodestr[256];
    printf("dispatch4host(%p, %s, %d, %llu, %p, %f)\n",
        effect, hostCode2string(opcode, opcodestr, 255), index, ivalue, ptr, fvalue);
    t_fstPtrInt result = dispatch(effect, opcode, index, ivalue, ptr, fvalue);
    printf("dispatch4host: %llu (0x%llX)\n", result, result);
    return result;
  }
  return 0xBEEF;
}
#define PRINTEFFCASE(x) \
  case x:               \
  if(x>98765)                                                           \
    printf("FstClient::dispatcher(%p, %d, %d, %d, %p, %f)\n", eff, x, index, value, object, opt); \
  else                                                                  \
    printf("FstClient::dispatcher(%p, %s, %d, %d, %p, %f)\n", eff, #x, index, value, object, opt); \
  break;
static t_fstPtrInt dispatcher(AEffect*eff, t_fstInt32 opcode, int index, t_fstPtrInt value, void* const object, float opt) {
  switch(opcode) {
  default:
    printf("FstClient::dispatcher(%p, %d, %d, %d, %p, %f)...\n", eff, opcode, index, value, object, opt);
    break;
#if 1
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
#endif
  case 53:
    /* REAPER calls this permanently */
    return 0;
    break;
  }
  if(object) {
    char*str = (char*)object;
    printf("\t'%.*s'\n", 512, str);
  }
  switch(opcode) {
  default: break;
  case effGetChunk:
    {
      char**ptr=(char**)object;
      *ptr=chunk;
    }
    //printf("getChunk: %d bytes @ %p\n", sizeof(chunk), chunk);
    return sizeof(chunk);
  case effSetProgram:
    //printf("setting program to %d\n", value);
    curProgram = value;
    return 1;
  case effGetProgramName:
    snprintf((char*)object, 32, "FstProgram%d", curProgram);
    //printf("setting program-name to %s\n", (char*)object);
    return 0;
  case effGetParamLabel:
    snprintf((char*)object, 32, "°");
    return 0;
  case effGetParamName:
    if(index>=sizeof(parameters))
      index=sizeof(parameters);
    snprintf((char*)object, 32, "rotation%c", index+88);
    return 0;
  case effGetParamDisplay:
    if(index>=sizeof(parameters))
      index=sizeof(parameters);
    snprintf((char*)object, 32, "%+03d", int((parameters[index]-0.5)*360+0.5));
    return 0;
  case effCanDo:
    return 0;
  }
  //printf("FstClient::dispatcher(%p, %d, %d, %d, %p, %f)\n", eff, opcode, index, value, object, opt);
  printf("\n");

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
  //printf("FstClient::process(%p, %p, %p, %d\n", eff, indata, outdata, sampleframes);
}
static void processReplacing(AEffect*eff, float**indata, float**outdata, int sampleframes) {
  //printf("FstClient::process'(%p, %p, %p, %d\n", eff, indata, outdata, sampleframes);
}
static void processDoubleReplacing(AEffect*eff, double**indata, double**outdata, int sampleframes) {
  //printf("FstClient::process2(%p, %p, %p, %d\n", eff, indata, outdata, sampleframes);
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
  eff->numInputs = 2;
  eff->numOutputs = 2;
  eff->float1 = 1.;
  eff->object = eff;
  eff->uniqueID = 0xdead;
  eff->version = 666;

  eff->flags |= effFlagsProgramChunks;

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
  return eff;
}
