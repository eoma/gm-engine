#pragma once

/**
 * @mainpage Totem Entity Development Kit - a component based entity system using properties
 *
 * @author Pål Trefall
 * @author Kenneth Gangstø
 *
 * @section INTRODUCTION
 * Totem is an Entity Development Kit designed from the idea of writing an intuitive 
 * API for Entity abilities and property definition through aggregation. An Entity 
 * consist of multiple components that defines it's abilities, while referencing 
 * necessary properties of the Entity required for the ability to function.
 *
 * The approach originated from the realization that the most common relationship 
 * between components in an Entity, is the requirement to either read from or write 
 * to the same properties of an Entity in order to define an ability, and the search 
 * for how to 100% decouple this relationship between components and at the same time 
 * avoid duplication of properties begun.
 *
 * And so the Totem EDK was born. Properties adds some extra logic on top 
 * of the data they represent, like a valueChanged signal that function slots can be 
 * registered to. Thus several components can "react" when a property is changed in some 
 * component, or from outside the Entity system.
 *
 * This adds a clean flow of events in the system, much more intuitive than in systems
 * where the components own the properties, and where other components that need access 
 * to that data either has to go through an event pipeline chain, or through direct coupling 
 * with other components.
 *
 * The realization is highly logical. An ability simply references and are limited by the 
 * properties that exist in the Entity they belong to. Whether multiple components require 
 * access to the same property is irrelevant for the existence of each individual component 
 * of ability.
 *
 * @section LICENSE
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 * 
 * Note: Some of the libraries Totem EDK may link to may have additional
 * requirements or restrictions.
 * 
 */