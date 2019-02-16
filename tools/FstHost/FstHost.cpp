
#include <stdio.h>
#include <dlfcn.h>
#include "fst.h"

#include <string>

typedef AEffect* (t_fstMain)(t_fstEffectDispatcher*);

t_fstPtrInt dispatcher (AEffect* effect, int opcode, int index, t_fstPtrInt value, void*ptr, float opt) {
  printf("FstHost::dispatcher(%p, %d, %d, %d, %p, %f);\n", effect, opcode, index, value, ptr, opt);
  switch(opcode) {
  case audioMasterVersion: return 2400;
  default:  break;
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
  if(!handle){printf("\t%s\n", dlerror()); return 0; }
  if(!vstfun)vstfun=dlsym(handle, "VSTPluginMain");
  if(!vstfun)vstfun=dlsym(handle, "main");
  if(!vstfun)dlclose(handle);
  return (t_fstMain*)vstfun;
}

int test_plugin(const char*filename) {
  t_fstMain*vstmain = load_plugin(filename);
  if(!vstmain)return printf("'%s' was not loaded\n", filename);
  AEffect*effect = vstmain(&dispatcher);
  if(!effect)return printf("unable to instantiate plugin from '%s'\n", filename);
  //dumpdata(filename, effect, 160);
  if(effect->magic != 0x56737450) return printf("magic failed: 0x%08X", effect->magic);
  printf("testing dispatcher\n");
  for(int i=0; i<74; i++) {
    bool skip = false;
    switch(i) {
    case 1:
    case 4:
      //case 5: /* program name [?] */
    case 6:
    case 7:
    case 8:
      //case 10: /* AM_AudioMan::reset() */
      //case 11: /* blocksize */
      //case 12: /* AM_VST_base::suspend () */
    case 13: /* AM_VST_Editor::getRect 1200 x 600 */
    case 14: /* AM_VST_Editor::open */
      // case 15: /* closing editor */
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 29:
    case 33:
    case 34:
    case 35:
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

#if 0
      /* JUCE plugins */
    case 42:
    case 44:
#endif
#if 1
/* Digits plugin */
    case 3:
#endif
      skip = true;
      break;
    default: break;
#if 0
#endif
    }
    if(skip) {
      printf("skipping: %d\n", i);
      continue;
    }
    printf("trying: %d\n", i);
    char buffer[512] = { 0 };
    t_fstPtrInt res = effect->dispatcher (effect, i, 0, 0, buffer, 0);
    const char*str = (const char*)res;
    printf("%d[%d=0x%X]: %.*s\n", i, res, res, 32, str);
    if(*buffer)
      printf("\t'%.*s'\n", 16, buffer);
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
