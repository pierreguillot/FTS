
#include "fst.h"
#include "../print_aeffect.h"
#include <stdio.h>
#include <dlfcn.h>

#include <string>
#include <string.h>

typedef AEffect* (t_fstMain)(AEffectDispatcherProc);

#include <unistd.h>
void fstpause(float duration=1.0) {
  sleep(1);
}

static void hexprint(void*ptr, size_t length) {
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
    printf("AEffect.dispatch(%p, %s, %d, %llu, %p, %f)\n",
        effect, effCode2string(opcode, opcodestr, 255), index, ivalue, ptr, fvalue);
    t_fstPtrInt result = effect->dispatcher(effect, opcode, index, ivalue, ptr, fvalue);
    printf("AEffect.dispatch: %llu (0x%llX)\n", result, result);
    return result;
  }
  return 0xDEAD;
}

t_fstPtrInt dispatcher (AEffect* effect, int opcode, int index, t_fstPtrInt value, void*ptr, float opt) {
  printf("FstHost::dispatcher[%d]: ", effect?effect->resvd2:-1);
  switch(opcode) {
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
    printf("(%p, %d, %d, %d, %p, %f);\n", effect, opcode, index, value, ptr, opt);
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

void dumpdata(const char*basename, const void*data, size_t length) {
  const char*ptr = (const char*)data;
  std::string filename = std::string(basename);
  filename+=".bin";
  FILE*f = fopen(filename.c_str(), "w");
  for(size_t i=0; i<length; i++) {
    fprintf(f, "%c", *ptr++);
  }
  fclose(f);
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

void test_opcodes(AEffect*effect, size_t numopcodes=78) {
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
  //  numopcodes = 0xDEADF00D+1;
  for(size_t i=0; i<numopcodes; i++) {
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
      hexprint(buffer, 16);
    }
    fstpause();
  }
  do {
    char buffer[200000] = { 0 };
    t_fstPtrInt res = dispatch (effect, 5, 0, 0, buffer, 0);
    printf("gotProgName: %.*s\n", 20, buffer);
  } while(0);
  printf("tested dispatcher with %u (0x%X) opcodes\n", numopcodes, numopcodes);
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
  printf("index#%d: setChunk[%d] returned %ull\n", index, (int)effSetChunk, result);
  size = dispatch(effect, effGetChunk, index, 0, &data, 0.f);
  printf("index#%d: got %d bytes @ 0x%X\n", index, size, data);
}

void test_opcode23(AEffect*effect) {
  size_t opcode = 23;
  int index = 0;

  t_fstPtrInt*buffer[8] = {0};
  printf("testing OP:%d\n", opcode);
  t_fstPtrInt result = dispatch(effect, opcode, index, 0, buffer, 0.f);
  printf("\tresult |\t%llu 0x%llX\n", result, result);
  if(*buffer) {
    printf("\tbuffer '%.*s'\n", 512, (char*)*buffer);
  }
  //hexprint(*buffer, result);
}

int test_plugin(const char*filename) {
  t_fstMain*vstmain = load_plugin(filename);
  if(!vstmain)return printf("'%s' was not loaded\n", filename);
  AEffect*effect = vstmain(&dispatcher);
  printf("instantiated effect %p\n", effect);
  if(!effect)return printf("unable to instantiate plugin from '%s'\n", filename);
  //dumpdata(filename, effect, 160);
  if(effect->magic != 0x56737450) return printf("magic failed: 0x%08X", effect->magic);
  print_aeffect(effect);
  test_setChunk(effect);
  return 0;

  test_opcode23(effect);
  #if 0
  for(int i=0; i<effect->numPrograms; i++) {
    size_t opcode = 29;
    char buffer[200] = { 0 };
    t_fstPtrInt result = dispatch(effect, opcode, i, 0, buffer, 0.f);
    printf("opcode:%d index:%d -> %llu", opcode, i, result);
    if(*buffer) {
      printf("\tbuffer '%.*s'\n", 512, buffer);
    }
  }
  return 0;
  #endif

  for(size_t opcode=16; opcode<63; opcode++) {
    if(skipOpcodeJUCE(opcode))continue;
    char buffer[200] = { 0 };
    t_fstPtrInt result = dispatch(effect, opcode, 0, 0, buffer, 0.f);
    if(result || *buffer)
      printf("tested %d", opcode);
    if(result)
      printf("\t|\t%llu 0x%llX", result, result);
    if(*buffer) {
      printf("\t|\tbuffer '%.*s'", 512, buffer);
      //hexprint(buffer, 16);
    }
    if(result || *buffer)
      printf("\n");
  }

  //test_opcodes(effect, 78);
  return 0;
}

int main(int argc, const char*argv[]) {
  for(int i=1; i<argc; i++) {
    test_plugin(argv[i]);
  }
  return 0;
}
