/*
 * nullelement.{cc,hh} -- do-nothing element
 * Eddie Kohler
 *
 * Copyright (c) 1999-2000 Massachusetts Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

#include <click/config.h>
#include "nullelement.hh"

NullElement::NullElement()
  : Element(1, 1)
{
  MOD_INC_USE_COUNT;
}

NullElement::~NullElement()
{
  MOD_DEC_USE_COUNT;
}

Packet *
NullElement::simple_action(Packet *p)
{
  return p;
}

PushNullElement::PushNullElement()
  : Element(1, 1)
{
  MOD_INC_USE_COUNT;
}

PushNullElement::~PushNullElement()
{
  MOD_DEC_USE_COUNT;
}

void
PushNullElement::push(int, Packet *p)
{
  output(0).push(p);
}

PullNullElement::PullNullElement()
  : Element(1, 1)
{
  MOD_INC_USE_COUNT;
}

PullNullElement::~PullNullElement()
{
  MOD_DEC_USE_COUNT;
}

Packet *
PullNullElement::pull(int)
{
  return input(0).pull();
}

EXPORT_ELEMENT(NullElement)
EXPORT_ELEMENT(PushNullElement)
EXPORT_ELEMENT(PullNullElement)
ELEMENT_MT_SAFE(NullElement)
ELEMENT_MT_SAFE(PushNullElement)
ELEMENT_MT_SAFE(PullNullElement)
