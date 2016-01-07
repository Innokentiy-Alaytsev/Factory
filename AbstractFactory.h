/*
  The MIT License (MIT)

  Copyright (c) 2016 Innokentij Alajcev (Иннокентий Алайцев)

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

#ifndef ABSTRACTFACTORY
#define ABSTRACTFACTORY


#include "Factory.h"


/**
  @addtogpoup Factory
  @{
*/

namespace Factory {
	/**
	  @brief Abstract fastory class able to produce objects of any of registered classes.

	  @details May be used if someone would like not to use lots of different factories or when
	  minimization of number of includes is desired.
	*/
	class AbstractFactory {
	public:
#ifdef FACTORY_USE_VARIADIC_TEMPLATES
		template < class Interface, class... Arguments >
		static std::shared_ptr< Interface > createObject (QString i_type_id, Arguments... io_args) {
			return Factory< Interface >::createObject (i_type_id, io_args...);
		}
#else
		template < class Interface >
		static std::shared_ptr< Interface > createObject (QString i_type_id) {
			return Factory< Interface >::createObject (i_type_id);
		}
#endif
	};
}

/**
  @}
*/

#endif // ABSTRACTFACTORY
