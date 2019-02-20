#ifndef PRINT_AEFFECT_H_
#define PRINT_AEFFECT_H_

#include "fst.h"
#include <stdio.h>

static void print_aeffect(AEffect*eff) {
  printf("AEffect @ %p", eff);
  if(!eff)return;
  if(eff->magic != 0x56737450)
    printf("\n\tmagic=0x%X", eff->magic);
  else
    printf("\n\tmagic=VstP");
  printf("\n\tdispatcher=%p", eff->dispatcher);
  printf("\n\tprocess=%p", eff->process);
  printf("\n\tgetParameter=%p", eff->getParameter);
  printf("\n\tsetParameter=%p", eff->setParameter);

  printf("\n\tnumPrograms=%d", eff->numPrograms);
  printf("\n\tnumParams=%d", eff->numParams);
  printf("\n\tnumInputs=%d", eff->numInputs);
  printf("\n\tnumOutputs=%d", eff->numOutputs);

  printf("\n\tflags=0x%X", eff->flags);
#define PRINT_AEFFECT__FLAG(x) if(effFlags##x) \
    {if(effFlags##x & eff->flags)printf("\n\t      %s", #x);} \
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

  printf("\n\tprocessReplacing=%p", eff->processReplacing);
  printf("\n\tprocessDoubleReplacing=%p", eff->processDoubleReplacing);
  printf("\n\n");
}

static void print_erect(ERect*rect) {
  printf("ERect[%p]", rect);
  if(rect)
    printf(" = %d|%d - %d|%d", rect->top, rect->left, rect->bottom, rect->right);
  printf("\n");
}

#endif /* PRINT_AEFFECT_H_ */
