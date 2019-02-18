#include "fst.h"

static t_fstEffectDispatcher*dispatchHost = 0;

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
  dispatchHost = dispatch4host;
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
  eff->unqueID = 123456;
  eff->version = 666;

  eff->processReplacing = processReplacing;
  eff->processDoubleReplacing = processDoubleReplacing;
}
