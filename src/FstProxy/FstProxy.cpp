#include "fst.h"
#include "fst_utils.h"
#include <stdio.h>
#include <dlfcn.h>

#include <string>
#include <string.h>

#include <map>

static std::map<AEffect*, AEffectDispatcherProc>s_host2plugin;
static std::map<AEffect*, AEffectDispatcherProc>s_plugin2host;
static AEffectDispatcherProc s_plug2host;

typedef AEffect* (t_fstMain)(AEffectDispatcherProc);

static
t_fstPtrInt host2plugin (AEffect* effect, int opcode, int index, t_fstPtrInt ivalue, void*ptr, float fvalue) {
  //printf("host2plugin:%d\n", opcode); fflush(stdout);
  switch(opcode) {
  case effGetVendorString:
    printf("getVendorString\n");
    snprintf((char*)ptr, 16, "ProxyVendor");
    return 1;
  case effGetEffectName:
    printf("getEffectName\n");
    snprintf((char*)ptr, 16, "ProxyEffect");
    return 1;
  default:
    break;
  }
  AEffectDispatcherProc h2p = s_host2plugin[effect];
  if(!h2p) {
    printf("Fst::host2plugin:: NO CALLBACK!\n");
    return 0xDEAD;
  }

  bool doPrint = true;
#ifdef FST_EFFKNOWN
  doPrint = !effKnown(opcode);
#endif
  if(doPrint) {
    char effbuf[256] = {0};
    printf("Fst::host2plugin(%s, %d, %ld, %p, %f)",
           effCode2string(opcode, effbuf, 255),
           index, ivalue, ptr, fvalue);
  }
  t_fstPtrInt result = h2p(effect, opcode, index, ivalue, ptr, fvalue);
  if(doPrint) {
    printf(" => %ld\n", result);
    fflush(stdout);
  }
  return result;
}

static
t_fstPtrInt plugin2host (AEffect* effect, int opcode, int index, t_fstPtrInt ivalue, void*ptr, float fvalue) {
  //printf("plugin2host:%d\n", opcode); fflush(stdout);
  AEffectDispatcherProc p2h = s_plugin2host[effect];

  if(!p2h)p2h = s_plug2host;
  if(effect && !s_host2plugin[effect]) {
    s_host2plugin[effect] = effect->dispatcher;
    effect->dispatcher = host2plugin;
  }

  bool doPrint = true;
#ifdef FST_HOSTKNOWN
  doPrint = !hostKnown(opcode);
#endif
  switch(opcode) {
  default: break;
  case audioMasterGetTime:
    doPrint = false;
    break;
  }
  if(doPrint) {
    char effbuf[256] = {0};
    printf("Fst::plugin2host(%s, %d, %ld, %p, %f)",
           hostCode2string(opcode, effbuf, 255),
           index, ivalue, ptr, fvalue);
  }
  t_fstPtrInt result = p2h(effect, opcode, index, ivalue, ptr, fvalue);
  if(doPrint) {
    printf(" -> %ld\n", result);
    fflush(stdout);
  }
  return result;
}

static
t_fstMain* load_plugin(const char* filename) {
  void*handle = dlopen(filename, RTLD_NOW | RTLD_GLOBAL);
  void*vstfun = 0;
  printf("Fst::loading %s as %p\n", filename, handle);
  if(!handle){printf("\t%s\n", dlerror()); return 0; }
  if(!vstfun)vstfun=dlsym(handle, "VSTPluginMain");
  if(!vstfun)vstfun=dlsym(handle, "main");
  if(!vstfun)dlclose(handle);
  printf("Fst::loaded '%s' @ %p: %p\n", filename, handle, vstfun);
  return (t_fstMain*)vstfun;
}

extern "C"
AEffect*VSTPluginMain(AEffectDispatcherProc dispatch4host) {
  char*pluginfile = getenv("FST_PROXYPLUGIN");
  if(!pluginfile)return 0;
  s_plug2host = dispatch4host;

  t_fstMain*plugMain = load_plugin(pluginfile);
  printf("plugMain: %p\n", plugMain);
  if(!plugMain)return 0;

  AEffect*plug = plugMain(plugin2host);
  printf("%p(%p) -> %p\n", plugMain, plugin2host, plug);
  if(!plug)
    return plug;

  s_plugin2host[plug] = dispatch4host;
  if(plug->dispatcher != host2plugin) {
    s_host2plugin[plug] = plug->dispatcher;
    plug->dispatcher = dispatch4host;
  }
  print_aeffect(plug);
  fflush(stdout);
  return plug;
}
