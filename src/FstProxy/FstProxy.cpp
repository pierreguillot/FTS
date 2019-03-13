#include "fst.h"
#include "fst_utils.h"
#include <stdio.h>

#include <string>
#include <string.h>

#include <map>

static std::map<AEffect*, AEffectDispatcherProc>s_host2plugin;
static std::map<AEffect*, AEffectDispatcherProc>s_plugin2host;
static AEffectDispatcherProc s_plug2host;

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
  case effCanDo:
    printf("Fst::pluginCanDo '%s'\n", (char*)ptr);
    break;
  case 26:
    printf("OPCODE26: %d\n", index);
    return (index<5);
  case 56:
    print_hex(ptr, 256);
    break;
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
  switch(opcode) {
  default: break;
  case effVendorSpecific:
    doPrint = false;
    break;
  }
  t_fstPtrInt result = 0;
  if(doPrint) {
    dispatch_effect(0, h2p, effect, opcode, index, ivalue, ptr, fvalue);
  } else {
    result = h2p(effect, opcode, index, ivalue, ptr, fvalue);
  }
  switch(opcode) {
  default: break;
  case 56:
    print_hex(ptr, 256);
    break;
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
  t_fstPtrInt result = -1;
  if(doPrint) {
    result = dispatch_host(0, p2h, effect, opcode, index, ivalue, ptr, fvalue);
  } else {
    result = p2h(effect, opcode, index, ivalue, ptr, fvalue);
  }
  return result;
}


extern "C"
AEffect*VSTPluginMain(AEffectDispatcherProc dispatch4host) {
  char*pluginfile = getenv("FST_PROXYPLUGIN");
  if(!pluginfile)return 0;
  s_plug2host = dispatch4host;

  t_fstMain*plugMain = fstLoadPlugin(pluginfile);
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
