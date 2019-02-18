#ifndef PRINT_AEFFECT_H_
#define PRINT_AEFFECT_H_

#include "fst.h"
#include <stdio.h>

static void print_aeffect(AEffect*eff) {
  printf("AEffect @ %p", eff);
  if(!eff)return;
  if(effect->magic != 0x56737450)
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

#endif /* PRINT_AEFFECT_H_ */
