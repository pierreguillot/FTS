#ifndef PRINT_AEFFECT_H_
#define PRINT_AEFFECT_H_

#include "fst.h"
#include <stdio.h>

#define PRINT_AEFFECT__OPCODESTR(x)             \
  case x:                                       \
    if(x>98765)break;                           \
    snprintf(output, length, "%s", #x);         \
    output[length-1] = 0;                       \
    return output

static char*effCode2string(size_t opcode, char*output, size_t length) {
  switch(opcode) {
    PRINT_AEFFECT__OPCODESTR(effCanBeAutomated);
    PRINT_AEFFECT__OPCODESTR(effCanDo);
    PRINT_AEFFECT__OPCODESTR(effClose);
    PRINT_AEFFECT__OPCODESTR(effConnectInput);
    PRINT_AEFFECT__OPCODESTR(effConnectOutput);
    PRINT_AEFFECT__OPCODESTR(effEditClose);
    PRINT_AEFFECT__OPCODESTR(effEditDraw);
    PRINT_AEFFECT__OPCODESTR(effEditGetRect);
    PRINT_AEFFECT__OPCODESTR(effEditIdle);
    PRINT_AEFFECT__OPCODESTR(effEditMouse);
    PRINT_AEFFECT__OPCODESTR(effEditOpen);
    PRINT_AEFFECT__OPCODESTR(effEditSleep);
    PRINT_AEFFECT__OPCODESTR(effEditTop);
    PRINT_AEFFECT__OPCODESTR(effGetChunk);
    PRINT_AEFFECT__OPCODESTR(effGetCurrentMidiProgram);
    PRINT_AEFFECT__OPCODESTR(effGetEffectName);
    PRINT_AEFFECT__OPCODESTR(effGetInputProperties);
    PRINT_AEFFECT__OPCODESTR(effGetNumMidiInputChannels);
    PRINT_AEFFECT__OPCODESTR(effGetNumMidiOutputChannels);
    PRINT_AEFFECT__OPCODESTR(effGetOutputProperties);
    PRINT_AEFFECT__OPCODESTR(effGetParamDisplay);
    PRINT_AEFFECT__OPCODESTR(effGetParamLabel);
    PRINT_AEFFECT__OPCODESTR(effGetParamName);
    PRINT_AEFFECT__OPCODESTR(effGetPlugCategory);
    PRINT_AEFFECT__OPCODESTR(effGetProductString);
    PRINT_AEFFECT__OPCODESTR(effGetProgram);
    PRINT_AEFFECT__OPCODESTR(effGetProgramName);
    PRINT_AEFFECT__OPCODESTR(effGetProgramNameIndexed);
    PRINT_AEFFECT__OPCODESTR(effGetSpeakerArrangement);
    PRINT_AEFFECT__OPCODESTR(effGetTailSize);
    PRINT_AEFFECT__OPCODESTR(effGetVendorString);
    PRINT_AEFFECT__OPCODESTR(effGetVendorVersion);
    PRINT_AEFFECT__OPCODESTR(effGetVstVersion);
    PRINT_AEFFECT__OPCODESTR(effIdentify);
    PRINT_AEFFECT__OPCODESTR(effIdle);
    PRINT_AEFFECT__OPCODESTR(effKeysRequired);
    PRINT_AEFFECT__OPCODESTR(effMainsChanged);
    PRINT_AEFFECT__OPCODESTR(effOpen);
    PRINT_AEFFECT__OPCODESTR(effProcessEvents);
    PRINT_AEFFECT__OPCODESTR(effSetBlockSize);
    PRINT_AEFFECT__OPCODESTR(effSetBypass);
    PRINT_AEFFECT__OPCODESTR(effSetChunk);
    PRINT_AEFFECT__OPCODESTR(effSetProcessPrecision);
    PRINT_AEFFECT__OPCODESTR(effSetProgram);
    PRINT_AEFFECT__OPCODESTR(effSetProgramName);
    PRINT_AEFFECT__OPCODESTR(effSetSampleRate);
    PRINT_AEFFECT__OPCODESTR(effSetSpeakerArrangement);
    PRINT_AEFFECT__OPCODESTR(effSetTotalSampleToProcess);
    PRINT_AEFFECT__OPCODESTR(effShellGetNextPlugin);
    PRINT_AEFFECT__OPCODESTR(effStartProcess);
    PRINT_AEFFECT__OPCODESTR(effStopProcess);
    PRINT_AEFFECT__OPCODESTR(effString2Parameter);
    PRINT_AEFFECT__OPCODESTR(effVendorSpecific);
  default: break;
  }
  snprintf(output, length, "%d", opcode);
  return output;
}
static char*hostCode2string(size_t opcode, char*output, size_t length) {
  switch(opcode) {
    PRINT_AEFFECT__OPCODESTR(audioMasterAutomate);
    PRINT_AEFFECT__OPCODESTR(audioMasterVersion);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetVendorString);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetProductString);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetVendorVersion);
    PRINT_AEFFECT__OPCODESTR(audioMasterBeginEdit);
    PRINT_AEFFECT__OPCODESTR(audioMasterEndEdit);
    PRINT_AEFFECT__OPCODESTR(audioMasterCanDo);
    PRINT_AEFFECT__OPCODESTR(audioMasterCloseWindow);
    PRINT_AEFFECT__OPCODESTR(audioMasterCurrentId);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetAutomationState);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetBlockSize);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetCurrentProcessLevel);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetDirectory);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetInputLatency);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetLanguage);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetNextPlug);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetNumAutomatableParameters);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetOutputLatency);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetOutputSpeakerArrangement);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetParameterQuantization);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetPreviousPlug);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetSampleRate);
    PRINT_AEFFECT__OPCODESTR(audioMasterGetTime);
    PRINT_AEFFECT__OPCODESTR(audioMasterIdle);
    PRINT_AEFFECT__OPCODESTR(audioMasterIOChanged);
    PRINT_AEFFECT__OPCODESTR(audioMasterNeedIdle);
    PRINT_AEFFECT__OPCODESTR(audioMasterOfflineGetCurrentMetaPass);
    PRINT_AEFFECT__OPCODESTR(audioMasterOfflineGetCurrentPass);
    PRINT_AEFFECT__OPCODESTR(audioMasterOfflineRead);
    PRINT_AEFFECT__OPCODESTR(audioMasterOfflineStart);
    PRINT_AEFFECT__OPCODESTR(audioMasterOfflineWrite);
    PRINT_AEFFECT__OPCODESTR(audioMasterOpenWindow);
    PRINT_AEFFECT__OPCODESTR(audioMasterPinConnected);
    PRINT_AEFFECT__OPCODESTR(audioMasterProcessEvents);
    PRINT_AEFFECT__OPCODESTR(audioMasterSetIcon);
    PRINT_AEFFECT__OPCODESTR(audioMasterSetOutputSampleRate);
    PRINT_AEFFECT__OPCODESTR(audioMasterSetTime);
    PRINT_AEFFECT__OPCODESTR(audioMasterSizeWindow);
    PRINT_AEFFECT__OPCODESTR(audioMasterTempoAt);
    PRINT_AEFFECT__OPCODESTR(audioMasterUpdateDisplay);
    PRINT_AEFFECT__OPCODESTR(audioMasterVendorSpecific);
    PRINT_AEFFECT__OPCODESTR(audioMasterWantMidi);
    PRINT_AEFFECT__OPCODESTR(audioMasterWillReplaceOrAccumulate);
  default: break;
  }
  snprintf(output, length, "%d", opcode);
  return output;
}

static void print_aeffect(AEffect*eff) {
  printf("AEffect @ %p", eff);
  if(!eff)return;
  if(eff->magic != 0x56737450)
    printf("\n\tmagic=0x%X", eff->magic);
  else
    printf("\n\tmagic=VstP");
  printf("\n\tdispatcherCB=%p", eff->dispatcher);
  printf("\n\tprocessCB=%p", eff->process);
  printf("\n\tgetParameterCB=%p", eff->getParameter);
  printf("\n\tsetParameterCB=%p", eff->setParameter);

  printf("\n\tnumPrograms=%d", eff->numPrograms);
  printf("\n\tnumParams=%d", eff->numParams);
  printf("\n\tnumInputs=%d", eff->numInputs);
  printf("\n\tnumOutputs=%d", eff->numOutputs);

  printf("\n\tflags=0x%X", eff->flags);
#define PRINT_AEFFECT__FLAG(x) if(effFlags##x)                  \
    {if(effFlags##x & eff->flags)printf("\n\t      %s", #x);}   \
  else printf("\n\t      ???%s???", #x)

  PRINT_AEFFECT__FLAG(HasEditor);
  PRINT_AEFFECT__FLAG(IsSynth);
  PRINT_AEFFECT__FLAG(CanDoubleReplacing);
  PRINT_AEFFECT__FLAG(CanReplacing);
  PRINT_AEFFECT__FLAG(NoSoundInStop);
  PRINT_AEFFECT__FLAG(ProgramChunks);

  printf("\n\tresvd1=0x%X", eff->resvd1);
  printf("\n\tresvd2=0x%X", eff->resvd2);
  printf("\n\tinitialDelay=%d", eff->initialDelay);

  printf("\n\tobject=%p", eff->object);
  printf("\n\tuniqueID=%d", eff->uniqueID);
  printf("\n\tversion=%d", eff->version);

  printf("\n\tprocessReplacingCB=%p", eff->processReplacing);
  printf("\n\tprocessDoubleReplacingCB=%p", eff->processDoubleReplacing);
  printf("\n\n");
}

static void print_erect(ERect*rect) {
  printf("ERect[%p]", rect);
  if(rect)
    printf(" = %d|%d - %d|%d", rect->top, rect->left, rect->bottom, rect->right);
  printf("\n");
}
static void print_hex(void*ptr, size_t length) {
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

#endif /* PRINT_AEFFECT_H_ */
