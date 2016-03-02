/*
  The MIT License (MIT)

  Copyright (c) 2016 Innokentij Alajcev (Иннокентий Алайцев) <alaitsev@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef FACTORYHELPERMACROS
#define FACTORYHELPERMACROS


#include "Factory.h"


/**
   @addtogroup Factory
   @{
 */

/**
  @def FACTORY_ADD_PRODUCTION

  @brief Macro to simplify addition of new production classes to factory.

  @details Creates an object of Factory::AddProduction class with unique name to register class
  PRODUCTION as production for factory which produces objects of classes that implement INTERFACE.

  @param [in] INTERFACE Interface implemented by production class and used as template argument for
  Factory class instantiation.

  @param [in] PRODUCTION Class that implements Interface and IProducible and which objects are
  supposed to be created by factory.
*/
#define FACTORY_ADD_PRODUCTION(INTERFACE, PRODUCTION) Factory::AddProduction< \
	INTERFACE, \
	PRODUCTION > gs_FactoryAddProduction ## __COUNTER__;


/**
  @def FACTORY_PRODUCTION_CREATE_OBJECT

  @brief Macro to simplify createObject function implementation.

  @details Defines a static method for object creation.

  @note Only works with simple createObject() methods with no arguments.

  @param [in] INTERFACE Class used for Factory class instantiation.

  @param [in] PRODUCTION Class inherited from INTERFACE.
*/
#define FACTORY_PRODUCTION_CREATE_OBJECT(INTERFACE, PRODUCTION) \
std::shared_ptr< INTERFACE > PRODUCTION::createObject () { \
	return std::shared_ptr< INTERFACE > (new PRODUCTION); \
}


/**
  @def FACTORY_PRODUCTION_TYPEID

  @brief Macro to simplify typeId function implementation.

  @details Defines a static method that returns PRODUCTION class typed ID equal to TYPEID.

  @param [in] PRODUCTION Class for which typeId() method will be defined.

  @param [in] TYPEID Desired PRODUCTION class type ID value.
*/
#define FACTORY_PRODUCTION_TYPEID(PRODUCTION, TYPEID) \
	StringType PRODUCTION::typeId () { \
		static StringType s_typeId (TYPEID); \
	\
		return s_typeId; \
	}


#ifdef FACTORY_USE_QT
	/**
	  @def FACTORY_PRODUCTION_TYPEID_QT_METATYPE

	  @brief Macro to simplify typeId function implementation for classes that use
	  QMetaType::typeName() as type ID.

	  @details Defines a static method that returns PRODUCTION class typed ID equal to
	  QMetaType::typeName() of METATYPE.

	  @param [in] PRODUCTION Class for which typeId() method will be defined.

	  @param [in] METATYPE Class which QMetaType::typeName() will be used as PRODUCTION type ID.
	*/
	#define FACTORY_PRODUCTION_TYPEID_QT_METATYPE(PRODUCTION, METATYPE) \
		FACTORY_PRODUCTION_TYPEID ( \
			PRODUCTION, \
			QMetaType::typeName ( \
				qMetaTypeId< METATYPE >()))
#endif

/**
   @}
*/

#endif // FACTORYHELPERMACROS

