#include "fst.h"
#include "fst_utils.h"
#include <stdio.h>
#include <dlfcn.h>

#include <string>
#include <string.h>

typedef AEffect* (t_fstMain)(AEffectDispatcherProc);


typedef struct FstProxy_ {
  AEffectDispatcherProc host2plugin;
  AEffectDispatcherProc plugin2host;
} FstProxy_t;
static FstProxy_t*s_proxy = 0;

static
t_fstPtrInt host2plugin (AEffect* effect, int opcode, int index, t_fstPtrInt ivalue, void*ptr, float fvalue) {
  if(!effect)
    return 0xDEAD;

  FstProxy_t*proxy = (FstProxy_t*)effect->object;
  if(proxy && proxy->host2plugin) {
    char effbuf[256] = {0};
    printf("Fst::host2plugin(%s, %d, %ld, %p, %f)",
           hostCode2string(opcode, effbuf, 255),
           index, ivalue, ptr, fvalue);
    t_fstPtrInt result = proxy->host2plugin(effect, opcode, index, ivalue, ptr, fvalue);
    printf(" => %ld\n", result);
    return result;
  }
  return 0xDEAD;
}

static
t_fstPtrInt plugin2host (AEffect* effect, int opcode, int index, t_fstPtrInt ivalue, void*ptr, float fvalue) {
  if(!effect)
    return 0xDEAD;

  FstProxy_t*proxy = (FstProxy_t*)effect->object;
  if(!proxy) {
    proxy = s_proxy;
    effect->object = proxy;
  }
  if(!proxy->host2plugin) {
    proxy->host2plugin = effect->dispatcher;
    effect->dispatcher = host2plugin;
  }

  char effbuf[256] = {0};
  printf("Fst::plugin2host(%s, %d, %ld, %p, %f)",
         effCode2string(opcode, effbuf, 255),
         index, ivalue, ptr, fvalue);
  t_fstPtrInt result = proxy->plugin2host(effect, opcode, index, ivalue, ptr, fvalue);
  printf(" => %ld\n", result);
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
  t_fstMain*plugMain = load_plugin(pluginfile);
  if(!plugMain)return 0;
  FstProxy_t*proxy = new FstProxy_t;
  s_proxy = proxy;
  proxy->plugin2host = dispatch4host;

  AEffect*eff = plugMain(plugin2host);
  eff->object = proxy;
  proxy->host2plugin = eff->dispatcher;
  eff->dispatcher = host2plugin;
}
