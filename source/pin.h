#ifndef _TABLEPIN

#define _TABLEPIN

#include "stdafx.h"

class TablePin
    {
        public:

            TablePin(Vector3 pos);
            ~TablePin();

        protected:

            Body * _pinBody;
    };

#endif
