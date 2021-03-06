
/*
  GUIDO Library
  Copyright (C) 2020 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#pragma once

#include <string>

#include "ARMTParameter.h"
#include "ARPositionTag.h"


/*@mkdoc

@group:Notes

@tagname:\mrest
@tagtype:R
@tagnotation:when applied to a whole measure rest, indicates the measures count.
@tagdesc
@tagend

@params:
@param:count:integer:a measures count:*none*:false
@paramdesc
[Common parameters](../../tagsparams#common-parameters) are applied to the measures count only.
You can use the [\restFormat](#restformat) tag to control the rest appearance.

The tag is ignored when not applied to a rest.
When the rest is not a full measure rest, the rest appearance is preserved and the count is still displayed.
@paramend

*/


/** \brief intended to multiple measures rests
*/
class ARMMRest : public ARMTParameter,  public ARPositionTag
{		
	public:
				 ARMMRest();
		virtual ~ARMMRest()	{};

		virtual const char*	getParamsStr() const	{ return kARMMRestParams; };
		virtual const char*	getTagName () const		{ return "ARMMRest"; };
		virtual std::string getGMNName () const		{ return "\\mrest"; };
	
		int 	getMeasuresCount() const;
};
