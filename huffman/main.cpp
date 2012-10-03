//---------------------------------------------------------------------------

#include "Filework.h"
#include "Hufftree.h"
#include "Huff.h"
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma argsused
int main(int argn, char* argv[])
{
    if(argn < 2) {
        Help();
        return 0;
    }
    switch(Check(argv)) {
	    case _extract: {
            if(argn != 3 && argn != 4) {
		        printf("Usage: %s %s arhive [file].\n", argv[0], argv[1]);
                return 1;
            }
	        if(argn == 3) {
                Decode(NULL, argv[2]);
		        return 0;
	        }
            if(argn == 4) {
    	        Decode(argv[3], argv[2]);
	            return 0;
            }
	    }
    	case _view: {
	        if(argn != 3) {
		        printf("Usage: %s %s arhive\n", argv[0], argv[1]);
		        return 1;
	        }
	        List(argv[2]);
	        return 0;
    	}
	    case _remove: {
	        if(argn != 4) {
		        printf("Usage: %s %s arhive file\n", argv[0], argv[1]);
		        return 1;
            }
	        Delete(argv[2], argv[3]);
	        return 0;
	    }
    	case _add: {
    	    if(argn != 3 && argn != 4) {
		        printf("Usage: %s %s file [arhive]\n", argv[0], argv[1]);
		        return 1;
	        }
            if(argn == 4) {
	            Add(argv[2], argv[3]);
    	        return 0;
            }
            if(argn == 3) {
	            Add(argv[2], NULL);
    	        return 0;
            }
	    }
    }
    return 0;
}
//---------------------------------------------------------------------------
