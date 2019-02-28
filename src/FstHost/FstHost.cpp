#include "fst.h"
#include "fst_utils.h"
#include <stdio.h>
#include <dlfcn.h>

#include <string>
#include <string.h>

typedef AEffect* (t_fstMain)(AEffectDispatcherProc);

#include <unistd.h>
void fstpause(float duration=1.0) {
  usleep(duration * 1000000);
}

float db2slider(float f) {
  return f;
}
float slider2db(float f) {
  return f;
}

static size_t curOpCode = -1;

t_fstPtrInt dispatch (AEffect* effect, int opcode, int index, t_fstPtrInt ivalue, void*ptr, float fvalue) {
  if(effect) {
    effect->resvd2 = opcode;
    return effect->dispatcher(effect, opcode, index, ivalue, ptr, fvalue);
  }
  return 0xDEAD;
}
t_fstPtrInt dispatch_v (AEffect* effect, int opcode, int index, t_fstPtrInt ivalue, void*ptr, float fvalue) {
  if(effect) {
    char opcodestr[256];
    printf("AEffect.dispatch(%p, %s, %d, %lu, %p, %f)\n",
        effect, effCode2string(opcode, opcodestr, 255), index, ivalue, ptr, fvalue);
    t_fstPtrInt result = effect->dispatcher(effect, opcode, index, ivalue, ptr, fvalue);
    printf("AEffect.dispatch: %lu (0x%lX)\n", result, result);
    fstpause(0.5);
    return result;
  }
  return 0xDEAD;
}

t_fstPtrInt dispatcher (AEffect* effect, int opcode, int index, t_fstPtrInt value, void*ptr, float opt) {
  char sbuf[256] = {0};
  printf("FstHost::dispatcher[%d]", effect?effect->resvd2:-1);
  printf("(%s, %d, %d, %p, %f)\n",
      hostCode2string(opcode, sbuf, 255),
      index, (int)value,
      ptr, opt);
  if(ptr) {
    char *str=(char*)ptr;
    if(*str) {
      printf("\t'%.*s'\n", 512, str);
    } else
      printf("\t<nil>\n");
  }
  switch(opcode) {
  case 0xDEADBEEF:
    printf("deadbeef...\n");
    return (t_fstPtrInt)db2slider;
  case audioMasterCurrentId:
    return 0xDEAD;
  case audioMasterVersion:
    printf("MasterVersion\n");
    return 2400;
  case audioMasterAutomate:
    printf("automate parameter[%d] to %f\n", index, opt);
    break;
  case audioMasterGetProductString:
    printf("GetProduct?String\n");
#if 0
    for(size_t i=0; i<kVstMaxProductStrLen; i++) {
      ((char*)ptr)[i] = 64+i%60;
    }
#endif
    strncpy((char*)ptr, "FstProduct?", kVstMaxProductStrLen);
    return 1;
  case audioMasterGetVendorString:
    printf("GetVendor?String\n");
    strncpy((char*)ptr, "FstVendor?", kVstMaxVendorStrLen);
    return 1;
  case 42:
    printf("42?\n");
    return 0;
  default:
    printf("\tdyspatcher(%p, %d, %d, %d, %p, %f);\n", effect, opcode, index, value, ptr, opt);
    //printf("(%p, %x, %x, %d, %p, %f);\n", effect, opcode, index, value, ptr, opt);
    do {
      char *str=(char*)ptr;
      if(str && *str) {
        printf("\t'%.*s'\n", 512, str);
      }
    } while(0);
    break;
  }
  return 0;
}

t_fstMain* load_plugin(const char* filename) {
  void*handle = dlopen(filename, RTLD_NOW | RTLD_GLOBAL);
  void*vstfun = 0;
  printf("loading %s as %p\n", filename, handle);
  if(!handle){printf("\t%s\n", dlerror()); return 0; }
  if(!vstfun)vstfun=dlsym(handle, "VSTPluginMain");
  if(!vstfun)vstfun=dlsym(handle, "main");
  if(!vstfun)dlclose(handle);
  printf("loaded '%s' @ %p: %p\n", filename, handle, vstfun);
  fstpause(1.);
  return (t_fstMain*)vstfun;
}

void test_opcode3334(AEffect*effect) {
  size_t opcode = effGetInputProperties;
  VstPinProperties vpp;
  memset(&vpp, 0, sizeof(vpp));
  printf("\ntrying: %d\n", opcode);
  t_fstPtrInt res = dispatch (effect, opcode, 0, 0, &vpp, 0);
  printf("returned %lu\n", res);
  if(res)
    print_pinproperties(&vpp);

  opcode = effGetOutputProperties;
  memset(&vpp, 0, sizeof(vpp));
  printf("\ntrying: %d\n", opcode);
  res = dispatch (effect, opcode, 0, 0, &vpp, 0);
  printf("returned %lu\n", res);
  if(res) {
    print_pinproperties(&vpp);
    print_hex(&vpp, 2*sizeof(vpp));
  }
}

static float test_setParameterS(AEffect*effect, size_t opcode, int index, char*str) {
  dispatch_v(effect, opcode, index, 0, str, 0.f);
  return effect->getParameter(effect, index);
}

static void test_setParameter(AEffect*effect) {
  int index = 0;
  float value = 0.666;
  printf("testing get/set Parameters for %p..................\n", effect);
  effect->setParameter(effect, index, value);
  printf("setParameter(%d, %f)\n", index, value);
  value = effect->getParameter(effect, index);
  printf("getParameter(%d) -> %f\n", index, value);
  for(size_t opcode = 9; opcode < 65536; opcode++) {
    if(effEditOpen==opcode)continue;
    if(effProcessEvents==opcode)continue;
    if(42==opcode)continue;
    if(69==opcode)continue;
    char buf[512];
    snprintf(buf, 511, "%s", "0.2");
    buf[511] = 0;
    printf("\ttesting#%d\n", opcode);
    fflush(stdout);
    value = test_setParameterS(effect, opcode, 0, buf);
    printf("\t#%d: '%s' -> %f\n", opcode, buf, value);
    fflush(stdout);
  }
}

static void test_opcode42(AEffect*effect) {
  printf("testing speaker arrangement\n");
  VstSpeakerArrangement setarr[10], getarr[10];
  for(size_t i=0; i<10; i++) memset(setarr+i, 0, sizeof(VstSpeakerArrangement));
  setarr[0].type = setarr[1].type = 0x1;
  setarr[0].numChannels = 2;
  setarr[1].numChannels = 2;
  dispatch_v(effect, 42, 0, (t_fstPtrInt)(setarr+0), (setarr+1), 0.f);
  print_hex(setarr+0, 8);
  print_hex(setarr+1, 8);

  printf("-----------------------------\n");
  for(size_t opcode=69; opcode<70; opcode++) {
    VstSpeakerArrangement *arrptr[2] = {0,0};
    if(42 == opcode)continue;
    if(effGetEffectName==opcode)continue; if(effGetVendorString==opcode)continue; if(effGetProductString==opcode)continue;
    dispatch_v(effect, opcode, 0, (t_fstPtrInt)(arrptr+0), arrptr+1, 0.f);
    print_hex(arrptr[0], 8);
    print_hex(arrptr[1], 8);
  }
  printf("tested speaker arrangement\n");
}


bool skipOpcode(size_t opcode) {

  switch(opcode) {
  case 1:
#if PLUGIN_JUCE || PLUGIN_DIGITS
    /* Digits plugin & JUCE plugins */
  case 3:
#endif
    //case 4:

    //case 5: /* program name [?] */

    //case 10: /* AM_AudioMan::reset() */
    //case 11: /* blocksize */
    //case 12: /* AM_VST_base::suspend () */
  case 13: /* AM_VST_Editor::getRect 1200 x 600 */
  case 14: /* AM_VST_Editor::open, exits with 1! */
  case 22:
  case 23:
  case 24:
  case 25:
  case 26:
  case 29:
  case 33:
  case 34:
  case 35:
#if PLUGIN_JUCE
    /* JUCE plugins */
  case 42:
  case 44:
#endif
  case 45:
  case 47:  /* AM_VST_base::getVendorString (char* text) */
  case 48:
  case 49:
  case 51:
  case 58:
    //case 59: /* u-he plugin doesnt use key, returns false (VST) or jumps to default key handler (WindowProc) */
  case 63:
  case 69:
  case 71:
  case 72:
    //case 73:
    return true;
  default: break;
  }
  return false;
}

void test_opcodes(AEffect*effect, size_t endopcode=78, size_t startopcode=10) {
  printf("testing dispatcher\n");
#if 0
  for(size_t opcode=0; opcode < 10; opcode++) {
  for(int i=0; i<effect->numPrograms; i++) {
   char buffer[512] = { 0 };
    t_fstPtrInt res = dispatch (effect, opcode, i, i, buffer, i);
    const char*str = (const char*)res;
    printf("program#%d[%d=0x%X]: %.*s\n", i, res, res, 32, str);
    if(*buffer)
      printf("\t'%.*s'\n", 512, buffer);
  }
  }
  return 0;
#endif
  //  endopcode = 0xDEADF00D+1;
  for(size_t i=startopcode; i<endopcode; i++) {
    curOpCode = i;
    if(!(i%65536)) {
      printf("=== mark %d ===\n", i>>16);
      fflush(stdout);
    }
    if (skipOpcode(i)
#ifdef NOSKIP
        && (NOSKIP != i)
#endif
        ) {
      printf("skipping: %d\n", i);
      continue;
    }
    //printf("\ntrying: %d\n", i);
    char buffer[200000] = { 0 };
    snprintf(buffer, 511, "name%d", i);
    t_fstPtrInt res = dispatch (effect, i, 0, 0, buffer, 0);
    if(res || (buffer && *buffer))
        printf("\ntried: %d\n", i);


    if(res) {
      const char*str = (const char*)res;
      printf("\t[%d=0x%X]: %.*s\n", i, res, res, 32, str);
    }
    if(*buffer)
      printf("\tbuffer '%.*s'\n", 512, buffer);
    switch(i) {
    default: break;
    case 4:
      print_hex(buffer, 16);
    }
    fstpause();
  }
  do {
    char buffer[200000] = { 0 };
    t_fstPtrInt res = dispatch (effect, 5, 0, 0, buffer, 0);
    printf("gotProgName: %.*s\n", 20, buffer);
  } while(0);
  printf("tested dispatcher with opcodes %u..%u\n", startopcode, endopcode);
}

bool skipOpcodeJUCE(size_t opcode) {
  switch(opcode) {
  default: break;
  case 2:
  case 3:
  case 4:
  case 5:
  case 13: case 14: case 15:
  case 42:
    return true;
  }
  return false;
}

void test_setChunk(AEffect*effect) {
  t_fstPtrInt data=0;
  t_fstPtrInt size=0;
  int index = 0;
  /* get data */
  size = dispatch(effect, effGetChunk, index, 0, &data, 0.f);
  printf("index#%d: got %d bytes @ 0x%X\n", index, size, data);

  index = 1;
  size = dispatch(effect, effGetChunk, index, 0, &data, 0.f);
  printf("index#%d: got %d bytes @ 0x%X\n", index, size, data);

  index = 0;
  t_fstPtrInt result = dispatch(effect, effSetChunk, index, size, &data, 0.f);
  printf("index#%d: setChunk[%d] returned %lu\n", index, (int)effSetChunk, result);
  size = dispatch(effect, effGetChunk, index, 0, &data, 0.f);
  printf("index#%d: got %d bytes @ 0x%X\n", index, size, data);
}

void test_opcode23(AEffect*effect) {
  size_t opcode = 23;
  int index = 0;

  t_fstPtrInt*buffer[8] = {0};
  printf("testing OP:%d\n", opcode);
  t_fstPtrInt result = dispatch(effect, opcode, index, 0, buffer, 0.f);
  printf("\tresult |\t%lu 0x%lX\n", result, result);
  if(*buffer) {
    printf("\tbuffer '%.*s'\n", 512, (char*)*buffer);
  }
  //print_hex(*buffer, result);
}

void test_opcode56(AEffect*effect) {
  size_t opcode = 56;
  const size_t bufsize = 1024;
  char*buffer[bufsize] = {0};
  for(size_t i=0; i<sizeof(bufsize); i++) {
    buffer[i] = 0;
  }

  printf("testing OP:%d\n", opcode);
  t_fstPtrInt result = dispatch_v(effect, opcode, 0, 0, buffer, 0.f);
  printf("\tresult |\t%lu 0x%lX\n", result, result);
  if(*buffer) {
    printf("\tbuffer '%.*s'\n", bufsize, (char*)buffer);
  }
  print_hex(buffer, 96);
}

void test_opcode29(AEffect*effect) {
  for(int i=0; i<effect->numPrograms; i++) {
    size_t opcode = 29; //effGetProgramNameIndexed;
    char buffer[200] = { 0 };
    t_fstPtrInt result = dispatch(effect, opcode, i, 0, buffer, 0.f);
    printf("opcode:%d index:%d -> %lu", opcode, i, result);
    if(*buffer) {
      printf("\tbuffer '%.*s'\n", 512, buffer);
    }
  }
}

void test_opcodesJUCE(AEffect*effect) {
 for(size_t opcode=16; opcode<63; opcode++) {
    if(skipOpcodeJUCE(opcode))continue;
    char buffer[200] = { 0 };
    t_fstPtrInt result = dispatch(effect, opcode, 0, 0, buffer, 0.f);
    if(result || *buffer)
      printf("tested %d", opcode);
    if(result)
      printf("\t|\t%lu 0x%lX", result, result);
    if(*buffer) {
      printf("\t|\tbuffer '%.*s'", 512, buffer);
      //print_hex(buffer, 16);
    }
    if(result || *buffer)
      printf("\n");
  }
}
void test_opcode25(AEffect*effect) {
  const unsigned char midi[4] = {0x90, 0x40, 0x7f, 0};
  VstEvents*ves = create_vstevents(midi);
  dispatch_v(effect, effProcessEvents, 0, 0, ves, 0.);
}
static float** makeFSamples(size_t channels, size_t frames) {
  float**samples = new float*[channels];
  for(size_t i=0; i<channels; i++) {
    samples[i] = new float[frames];
    float*samps=samples[i];
    for(size_t j=0; j<frames; j++) {
      samps[j] = 0.f;
    }
  }
  return samples;
}
void test_reaper(AEffect*effect) {
  t_fstPtrInt ret=0;
  char strbuf[1024];
  const int blockSize = 512;
  float**insamples = makeFSamples(effect->numInputs, blockSize);
  float**outsamples = makeFSamples(effect->numOutputs, blockSize);

  printf("vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
  dispatch_v(effect, effOpen, 0, 0, 000, 0.000000);
  dispatch_v(effect, effSetSampleRate, 0, 0, 000, 44100.000000);
  dispatch_v(effect, effSetBlockSize, 0, blockSize, 000, 0.000000);

  strbuf[0]=0;
  dispatch_v(effect, effGetEffectName, 0, 0, strbuf, 0.000000);
  printf("EffectName: %s\n", strbuf);
  strbuf[0] = 0;
  dispatch_v(effect, effGetVendorString, 0, 0, strbuf, 0.000000);
  printf("VendorString: %s\n", strbuf);

  snprintf(strbuf, 1023, "hasCockosNoScrollUI"); strbuf[1023] = 0;
  dispatch_v(effect, effCanDo, 0, 0, strbuf, 0.000000);
  snprintf(strbuf, 1023, "wantsChannelCountNotifications"); strbuf[1023] = 0;
  dispatch_v(effect, effCanDo, 0, 0, strbuf, 0.000000);
  test_opcode42(effect);
  snprintf(strbuf, 1023, "hasCockosExtensions"); strbuf[1023] = 0;
  dispatch_v(effect, effCanDo, 0, 0, strbuf, 0.000000);

  dispatch_v(effect, effGetVstVersion, 0, 0, 000, 0.000000);
  dispatch_v(effect, 12, 0, 1, 000, 0.000000);
  dispatch_v(effect, 71, 0, 0, 000, 0.000000);

  snprintf(strbuf, 1023, "receiveVstEvents"); strbuf[1023] = 0;
  dispatch_v(effect, effCanDo, 0, 0, strbuf, 0.000000);
  snprintf(strbuf, 1023, "receiveVstMidiEvent"); strbuf[1023] = 0;
  dispatch_v(effect, effCanDo, 0, 0, strbuf, 0.000000);

  dispatch_v(effect, 35, 0, 0, 000, 0.000000);

  snprintf(strbuf, 1023, "sendVstEvents"); strbuf[1023] = 0;
  dispatch_v(effect, effCanDo, 0, 0, strbuf, 0.000000);
  snprintf(strbuf, 1023, "sendVstMidiEvent"); strbuf[1023] = 0;
  dispatch_v(effect, effCanDo, 0, 0, strbuf, 0.000000);

  dispatch_v(effect, effGetProgram, 0, 0, 000, 0.000000);

  ret = dispatch_v(effect, effGetChunk, 0, 0, strbuf, 0.000000);
  do {
    void*dataptr = *((void**)strbuf);
    print_hex(dataptr, 8);
  } while(0);
  dispatch_v(effect, effSetProgram, 0, 1, 000, 0.000000);
  strbuf[0] = 0;
  dispatch_v(effect, effGetProgramName, 0, 0, strbuf, 0.000000);
  printf("ProgramName: '%s'\n", strbuf);
  test_opcode56(effect);

  dispatch_v(effect, effGetProgram, 0, 0, 000, 0.000000);

  printf("=============PROC==============================\n");
  effect->process(effect, insamples, outsamples, blockSize);
  test_opcode25(effect);
  effect->process(effect, insamples, outsamples, blockSize);
  printf("==============================================\n");
  dispatch_v(effect, 12, 0, 0, 000, 0.000000);
  dispatch_v(effect, 12, 0, 1, 000, 0.000000);
  printf("==============================================\n");
  dispatch_v(effect, 12, 0, 0, 000, 0.000000);
  dispatch_v(effect, 12, 0, 1, 000, 0.000000);
  dispatch_v(effect, 12, 0, 0, 000, 0.000000);
  dispatch_v(effect, 12, 0, 1, 000, 0.000000);
  dispatch_v(effect, 72, 0, 0, 000, 0.000000);
  dispatch_v(effect, 12, 0, 0, 000, 0.000000);
  dispatch_v(effect, effClose, 0, 0, 000, 0.000000);
  printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
}


int test_plugin(const char*filename) {
  t_fstMain*vstmain = load_plugin(filename);
  if(!vstmain)return printf("'%s' was not loaded\n", filename);
  AEffect*effect = vstmain(&dispatcher);
  printf("instantiated effect %p\n", effect);
  if(!effect)return printf("unable to instantiate plugin from '%s'\n", filename);
  //dump_data(filename, effect, 160);
  if(effect->magic != 0x56737450) return printf("magic failed: 0x%08X", effect->magic);
  print_aeffect(effect);
  test_reaper(effect); return 0;
  //test_opcode29(effect); return 0;
  //test_opcode3334(effect); return 0;
  //test_opcodesJUCE(effect);

  //test_opcodes(effect, 78);
  return 0;
}

int main(int argc, const char*argv[]) {
  for(int i=1; i<argc; i++) {
    test_plugin(argv[i]);
  }
  return 0;
}
