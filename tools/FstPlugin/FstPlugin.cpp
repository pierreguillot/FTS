#include <stddef.h>
#include <cstdio>
#include "fst.h"

static t_fstEffectDispatcher*dispatch = 0;

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
  printf("FstClient::dispatcher(%p, %d, %d, %d, %p, %f)\n", eff, opcode, index, value, object, opt);
  return 0;
}

static void setParameter(AEffect*eff, int index, float value) {
  printf("FstClient::setParameter(%p)[%d] -> %f\n", eff, index, value);
}
static float getParameter(AEffect*eff, int index) {
  printf("FstClient::getParameter(%p)[%d]\n", eff, index);
  return 0.5;
}
static void process(AEffect*eff, float**indata, float**outdata, int sampleframes) {
  printf("FstClient::process(%p, %p, %p, %d\n", eff, indata, outdata, sampleframes);
}
static void processReplacing(AEffect*eff, float**indata, float**outdata, int sampleframes) {
  printf("FstClient::process'(%p, %p, %p, %d\n", eff, indata, outdata, sampleframes);
}
static void processDoubleReplacing(AEffect*eff, double**indata, double**outdata, int sampleframes) {
  printf("FstClient::process2(%p, %p, %p, %d\n", eff, indata, outdata, sampleframes);
}


extern "C"
AEffect*VSTPluginMain(t_fstEffectDispatcher*dispatch4host) {
  dispatch = dispatch4host;
  printf("FstPlugin::main(%p)\n", dispatch4host);
  AEffect* eff = new AEffect;

  eff->magic = 0x56737450;
  eff->dispatcher = dispatcher;
  eff->process = process;
  eff->getParameter = getParameter;
  eff->setParameter = setParameter;

  eff->numPrograms = 1;
  eff->numParams = 3;
  eff->numInputs = 2;
  eff->numOutputs = 2;
  eff->float1 = 1.;
  eff->object = eff;
  eff->uniqueID = 123456;
  eff->version = 666;

  eff->processReplacing = processReplacing;
  eff->processDoubleReplacing = processDoubleReplacing;
  return eff;
}
