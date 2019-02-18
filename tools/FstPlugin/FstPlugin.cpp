#include <stddef.h>
#include <cstdio>
#include "fst.h"

static t_fstEffectDispatcher*dispatch = 0;
static int curProgram = 0;

static float parameters[3];

void print_effect(AEffect*eff) {
  printf("AEffect @ %p", eff);
  if(!eff)return;
  printf("\n\tmagic=0x%X", eff->magic);
  printf("\n\tdispatcher=%p", eff->dispatcher);
  printf("\n\tprocess=%p", eff->process);
  printf("\n\tgetParameter=%p", eff->getParameter);
  printf("\n\tsetParameter=%p", eff->setParameter);

  printf("\n\tnumPrograms=%d", eff->numPrograms);
  printf("\n\tnumParams=%d", eff->numParams);
  printf("\n\tnumInputs=%d", eff->numInputs);
  printf("\n\tnumOutputs=%d", eff->numOutputs);

  printf("\n\tflags=0x%X", eff->flags);
  printf("\n\tresvd1=0x%X", eff->resvd1);
  printf("\n\tresvd2=0x%X", eff->resvd2);
  printf("\n\tinitialDelay=%d", eff->initialDelay);

  printf("\n\tobject=%p", eff->object);
  printf("\n\tuniqueID=%d", eff->uniqueID);
  printf("\n\tversion=%d", eff->version);

  printf("\n\tprocessReplacing=%p", eff->processReplacing);
  printf("\n\tprocessDoubleReplacing=%p", eff->processDoubleReplacing);
  printf("\n\n");
}

static t_fstPtrInt dispatcher(AEffect*eff, t_fstInt32 opcode, int index, t_fstPtrInt value, void* const object, float opt) {
  switch(opcode) {
  default: break;
  case 53: case 3:
    return 0;
  }
  switch(opcode) {
  default: break;
  case effSetProgram:
    curProgram = index,
    return 0;
  case effGetProgramName:
    snprintf((char*)object, 32, "FstProgram%d", curProgram);
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
  }
  printf("FstClient::dispatcher(%p, %d, %d, %d, %p, %f)\n", eff, opcode, index, value, object, opt);

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
AEffect*VSTPluginMain(t_fstEffectDispatcher*dispatch4host) {
  dispatch = dispatch4host;
  printf("FstPlugin::main(%p)\n", dispatch4host);
  for(size_t i=0; i<sizeof(parameters); i++)
    parameters[i] = 0.5;

  AEffect* eff = new AEffect;

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
  eff->uniqueID = 123456;
  eff->version = 666;

  eff->processReplacing = processReplacing;
  eff->processDoubleReplacing = processDoubleReplacing;
  print_effect(eff);

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
