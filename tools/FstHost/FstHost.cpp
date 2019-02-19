
#include "fst.h"
#include "../print_aeffect.h"
#include <stdio.h>
#include <dlfcn.h>

#include <string>
#include <string.h>

typedef AEffect* (t_fstMain)(AEffectDispatcherProc);

#include <unistd.h>
void fstpause(float duration=1.0) {
  return;
  sleep(1);
}

static void hexprint(char*data, size_t length) {
  printf("DATA@%p [%d]", data, length);
  if(data) {
    for(size_t i=0; i<length; i++) {
      if(!(i%16))printf("\n\t");
      if(!(i% 8))printf(" ");
      printf(" %02X", *data++);
    }
  }
  printf("\n");
}
t_fstPtrInt dispatcher (AEffect* effect, int opcode, int index, t_fstPtrInt value, void*ptr, float opt) {
  printf("FstHost::dispatcher: ");
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
  default:
    printf("(%p, %d, %d, %d, %p, %f);\n", effect, opcode, index, value, ptr, opt);
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
int test_plugin(const char*filename) {
  t_fstMain*vstmain = load_plugin(filename);
  if(!vstmain)return printf("'%s' was not loaded\n", filename);
  AEffect*effect = vstmain(&dispatcher);
  printf("instantiated effect %p\n", effect);
  if(!effect)return printf("unable to instantiate plugin from '%s'\n", filename);
  //dumpdata(filename, effect, 160);
  if(effect->magic != 0x56737450) return printf("magic failed: 0x%08X", effect->magic);
  print_aeffect(effect);

  printf("testing dispatcher\n");
#if 0
  for(int i=0; i<effect->numPrograms; i++) {
   char buffer[512] = { 0 };
    t_fstPtrInt res = effect->dispatcher (effect, 5, i, i, buffer, i);
    const char*str = (const char*)res;
    printf("program#%d[%d=0x%X]: %.*s\n", i, res, res, 32, str);
    if(*buffer)
      printf("\t'%.*s'\n", 512, buffer);
  }
  return 0;
#endif
  size_t numopcodes = 78;
  for(size_t i=0; i<numopcodes; i++) {
    }
    if (skipOpcode(i)
#ifdef NOSKIP
        && (NOSKIP != i)
#endif
        ) {
      printf("skipping: %d\n", i);
      continue;
    }
    printf("\ntrying: %d\n", i);
    char buffer[512] = { 0 };
    t_fstPtrInt res = effect->dispatcher (effect, i, 1, 0, buffer, 0.5);
    if(res) {
      const char*str = (const char*)res;
      printf("\t[%d=0x%X]: %.*s\n", i, res, res, 32, str);
    }
    if(*buffer)
      printf("\t'%.*s'\n", 512, buffer);
    fstpause();
  }
  printf("tested dispatcher\n");
  return 0;
}

int main(int argc, const char*argv[]) {
  for(int i=1; i<argc; i++) {
    test_plugin(argv[i]);
  }
  return 0;
}
