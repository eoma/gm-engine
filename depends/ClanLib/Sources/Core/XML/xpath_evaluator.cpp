/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Thomas Gottschalk Larsen
*/

#include "Core/precomp.h"
#include "API/Core/XML/xpath_evaluator.h"
#include "API/Core/XML/xpath_exception.h"
#include "API/Core/XML/dom_node.h"
#include "xpath_evaluator_impl.h"
#include "xpath_token.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// XPathEvaluator Construction:

XPathEvaluator::XPathEvaluator()
: impl(new XPathEvaluator_Impl)
{
}

/////////////////////////////////////////////////////////////////////////////
// XPathEvaluator Operations:

XPathObject XPathEvaluator::evaluate(const std::string &expression, const DomNode &context_node) const
{
	XPathToken prev_token;
	std::vector<DomNode> nodelist(1, context_node);
	XPathEvaluateResult result = impl->evaluate(expression, nodelist, 0, prev_token);
	if (result.next_token.type != XPathToken::type_none)
		throw XPathException("Expected end of expression", expression, result.next_token);
	return result.result;
}

}
